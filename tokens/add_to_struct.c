/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:06:53 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/16 21:44:38 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	count_args(char **tokenized, int start)
{
	int	count;

	count = 0;
	while (tokenized[start])
	{
		if (tokenized[start][0] == '|')
			break ;
		if ((tokenized[start][0] == '<' || tokenized[start][0] == '>')
			&& tokenized[start][1] == '\0' && tokenized[start + 1])
		{
			start += 2;
			continue ;
		}
		if ((tokenized[start][0] == '<' || tokenized[start][1] == '<')
			&& tokenized[start][2] == '\0' && tokenized[start + 1])
		{
			start += 2;
			continue ;
		}
		count++;
		start++;
	}
	return (count);
}

static t_cmd	*new_cmd(int argc)
{
	int		index;
	t_cmd	*cmd;

	index = 0;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_bzero(cmd, sizeof(t_cmd));
	cmd->infile = 0;
	cmd->outfile = 1;
	cmd->prev_fd = 0;
	cmd->command = NULL;
	cmd->next = NULL;
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	cmd->heredoc_fd = -1;
	cmd->heredoc_lim = NULL;
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	while (index < (argc + 1))
		cmd->args[index++] = NULL;
	return (cmd);
}

int	handle_pipe(t_cmd **cmds, t_cmd **current, t_cmd **last, t_pipe_ctx *ctx)
{
	t_cmd	*new_command;

	if (!ctx->tok[(ctx->index)] || ctx->tok[(ctx->index)][0] != '|'
		|| ctx->tok[(ctx->index)][1] != '\0' || !ctx->tok[(ctx->index) + 1]
		|| (ctx->tok[(ctx->index) + 1][0] == '|' && ctx->tok[(ctx->index)
			+ 1][1] == '\0'))
		return (0);
	new_command = new_cmd(count_args(ctx->tok, (ctx->index) + 1));
	if (!new_command)
		return (0);
	if (*current)
		(*current)->next = new_command;
	else
		*cmds = new_command;
	*last = new_command;
	*current = new_command;
	if (ctx->arg_pos)
		(ctx->arg_pos) = 0;
	if (ctx->p_argc)
		(ctx->p_argc) = count_args(ctx->tok, (ctx->index) + 1);
	((ctx->index))++;
	return (1);
}

int	handle_out_redirection(t_cmd **current, t_pipe_ctx *ctx)
{
	int	fd;

	if (!ctx->tok[ctx->index] || !ctx->tok[(ctx->index) + 1] || !(*current))
		return (0);
	if (ft_strncmp(ctx->tok[ctx->index],
			">", ft_strlen(ctx->tok[ctx->index])) == 0)
	{
		if (!ctx->tok[ctx->index + 1])
			return (0);
		fd = open(ctx->tok[ctx->index + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror(ctx->tok[ctx->index + 1]);
			(*current)->outfile = -1;
			ctx->index += 2;
			return (1);
		}
		(*current)->outfile = fd;
		ctx->index += 2;
		return (1);
	}
	return (0);
}

int	handle_in_redirection(t_cmd **current, t_pipe_ctx *ctx)
{
	int	fd;

	if (!ctx->tok[ctx->index] || !ctx->tok[ctx->index + 1] || !(*current))
		return (0);
	if (ft_strncmp(ctx->tok[ctx->index],
			"<", ft_strlen(ctx->tok[ctx->index])) == 0)
	{
		fd = open(ctx->tok[(ctx->index) + 1], O_RDONLY);
		if (fd < 0)
		{
			perror(ctx->tok[(ctx->index) + 1]);
			(*current)->infile = -1;
			ctx->index += 2;
			return (1);
		}
		(*current)->infile = fd;
		ctx->index += 2;
		return (1);
	}
	return (0);
}

t_exec	create_heredoc_exec(t_exec exec, int has_quotes)
{
	t_exec	temp_exec;

	temp_exec = exec;
	temp_exec.quote_type = malloc(sizeof(int) * 2);
	if (!temp_exec.quote_type)
		return (exec);
	if (has_quotes)
	{
		temp_exec.quote_type[0] = 1;
		temp_exec.quote_type[1] = 1;
	}
	else
	{
		temp_exec.quote_type[0] = 0;
		temp_exec.quote_type[1] = 0;
	}
	return (temp_exec);
}

char	*expnad_heredoc_line(char *line, t_exec exec, int has_quotes)
{
	char		**line_tokens;
	char		*expanded;
	t_exec		temp_exec;
	t_pipe_ctx	ctx_local;

	line_tokens = malloc(sizeof(char *) * 2);
	if (!line_tokens)
		return (ft_strdup(line));
	line_tokens[0] = ft_strdup(line);
	line_tokens[1] = NULL;
	temp_exec = create_heredoc_exec(exec, has_quotes);
	if (!temp_exec.quote_type)
	{
		free_resources(line_tokens);
		return (ft_strdup(line));
	}
	ctx_local.tok = line_tokens;
	ctx_local.index = 0;
	handle_dolar(&ctx_local, temp_exec);
	expanded = ft_strdup(line_tokens[0]);
	free(temp_exec.quote_type);
	free_resources(line_tokens);
	if (expanded)
		return (expanded);
	return (ft_strdup(line));
}

static void	set_heredoc_fd(t_cmd *current, int fd, char *lim)
{
	if (fd < 0)
		current->infile = -1;
	else
	{
		if (current->heredoc_fd > 0)
			close(current->infile);
		if (current->heredoc_lim)
			free(current->heredoc_lim);
		current->heredoc_fd = fd;
		current->heredoc_lim = ft_strdup(lim);
		current->infile = fd;
	}
}

int	build_heredoc(char *lim, t_exec exec, int has_quotes)
{
	int		fd[2];
	char	*line;
	char	*expanded;

	if (pipe(fd) < 0)
		return (-1);
	while (1)
	{
		line = readline(">");
		if (!line || ft_strncmp(line, lim, ft_strlen(lim) + 1) == 0)
		{
			free(line);
			break ;
		}
		if (has_quotes)
			expanded = ft_strdup(line);
		else
			expanded = expnad_heredoc_line(line, exec, 0);
		write(fd[1], expanded, ft_strlen(expanded));
		write(fd[1], "\n", 1);
		free(expanded);
		free(line);
	}
	close(fd[1]);
	return (fd[0]);
}

int	handel_heredoc(t_cmd **current, t_pipe_ctx *ctx, t_exec exec)
{
	int	fd;
	int	has_quotes;

	if (!ctx->tok[ctx->index] || !ctx->tok[(ctx->index) + 1] || !(*current))
		return (0);
	if (ctx->tok[ctx->index][0] == '<' && ctx->tok[ctx->index][1] == '<')
	{
		has_quotes = 0;
		if (exec.quote_type)
			has_quotes = exec.quote_type[ctx->index + 1];
		fd = build_heredoc(ctx->tok[ctx->index + 1], exec, has_quotes);
		set_heredoc_fd(*current, fd, ctx->tok[ctx->index + 1]);
		ctx->index += 2;
		return (1);
	}
	return (0);
}

int	replace_exit_code(char **tokenized, int index, t_exec exec)
{
	char	*exit_s;

	exit_s = ft_itoa(exec.exit);
	if (!exit_s)
		return (0);
	free(tokenized[index]);
	tokenized[index] = exit_s;
	return (1);
}

int	expand_dollar_next(t_pipe_ctx *ctx)
{
	char	*env;
	int		i;

	if (!ctx->tok[ctx->index + 1])
		return (0);
	env = getenv(ctx->tok[ctx->index + 1]);
	free(ctx->tok[ctx->index]);
	if (env)
		ctx->tok[ctx->index] = ft_strdup(env);
	else
		ctx->tok[ctx->index] = ft_strdup("");
	free(ctx->tok[ctx->index + 1]);
	i = ctx->index + 1;
	while (ctx->tok[i])
	{
		ctx->tok[i] = ctx->tok[i + 1];
		i++;
	}
	return (1);
}

int	expand_dollar_var(t_pipe_ctx *ctx)
{
	char	*var;
	char	*env;

	var = malloc(ft_strlen(ctx->tok[ctx->index]));
	if (!var)
		return (0);
	ft_strlcpy(var, ctx->tok[ctx->index] + 1, ft_strlen(ctx->tok[ctx->index]));
	env = getenv(var);
	free(ctx->tok[ctx->index]);
	if (env)
		ctx->tok[ctx->index] = ft_strdup(env);
	else
		ctx->tok[ctx->index] = ft_strdup("");
	free(var);
	return (1);
}

int	handle_dolar(t_pipe_ctx *ctx, t_exec exec)
{
	if (!ctx->tok[ctx->index])
		return (0);
	if (exec.quote_type[ctx->index] == 1)
		return (0);
	if (ctx->tok[ctx->index][0] == '$' && ctx->tok[ctx->index][1] == '?')
		return (replace_exit_code(ctx->tok, ctx->index, exec));
	if (ft_strncmp(ctx->tok[ctx->index],
			"$", ft_strlen(ctx->tok[ctx->index])) == 0)
		return (expand_dollar_next(ctx));
	if (ctx->tok[ctx->index][0] == '$')
		return (expand_dollar_var(ctx));
	return (0);
}

void	set_prev_fd(t_cmd *cmds)
{
	t_cmd	*curr;

	curr = cmds;
	while (curr)
	{
		curr->prev_fd = 0;
		curr = curr->next;
	}
}

void	handle_argument(t_cmd **current, t_pipe_ctx *ctx)
{
	char	*tok;

	if (!ctx->tok[ctx->index])
		return ;
	tok = ctx->tok[ctx->index];
	if (!(*current)->command && (ctx->arg_pos) == 0)
	{
		(*current)->command = ft_strdup(tok);
		if (!(*current)->command)
			return ;
		(*current)->args[0] = ft_strdup(tok);
		if (!(*current)->args[0])
			return ;
		(ctx->arg_pos) = 1;
		(*current)->args[1] = NULL;
	}
	else if ((ctx->arg_pos) < (ctx->p_argc))
	{
		(*current)->args[(ctx->arg_pos)] = ft_strdup(tok);
		((ctx->arg_pos))++;
		(*current)->args[(ctx->arg_pos)] = NULL;
	}
	((ctx->index))++;
}

int	first_char_is_special(t_pipe_ctx *ctx,
		char **pending_infile, char **pending_outfile)
{
	if (ctx->tok[ctx->index] && ctx->tok[ctx->index + 1]
		&& ft_strncmp(ctx->tok[ctx->index],
			"<", ft_strlen(ctx->tok[ctx->index])) == 0)
	{
		*pending_infile = ft_strdup(ctx->tok[ctx->index + 1]);
		ctx->index += 2;
		return (1);
	}
	if (ctx->tok[ctx->index] && ctx->tok[ctx->index + 1]
		&& ft_strncmp(ctx->tok[ctx->index],
			">", ft_strlen(ctx->tok[ctx->index])) == 0)
	{
		*pending_outfile = ft_strdup(ctx->tok[ctx->index + 1]);
		ctx->index += 2;
		return (1);
	}
	return (0);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmds;
	while (current)
	{
		if (current->command)
			free(current->command);
		if (current->args)
			free_resources(current->args);
		if (current->heredoc_lim)
			free(current->heredoc_lim);
		if (current->infile > 0)
			close(current->infile);
		if (current->outfile > 1)
			close(current->outfile);
		current = current->next;
	}
	while (cmds)
	{
		next = cmds->next;
		free(cmds);
		cmds = next;
	}
}

static int	leading_in(t_pipe_ctx *ctx, t_cmd **cmds, t_cmd **cur)
{
	int	fd;

	if (*cur || !ctx->tok[(ctx->index)] || !ctx->tok[(ctx->index) + 1])
		return (0);
	if (ft_strncmp(ctx->tok[(ctx->index)], "<", 1) != 0)
		return (0);
	(ctx->p_argc) = count_args(ctx->tok, (ctx->index));
	*cur = new_cmd((ctx->p_argc));
	if (!*cmds)
		*cmds = *cur;
	fd = open(ctx->tok[(ctx->index) + 1], O_RDONLY);
	if (fd < 0)
	{
		perror(ctx->tok[(ctx->index) + 1]);
		(*cur)->infile = -1;
		(ctx->index) += 2;
		return (1);
	}
	(*cur)->infile = fd;
	(ctx->index) += 2;
	return (1);
}

static int	leading_out(t_pipe_ctx *ctx, t_cmd **cmds, t_cmd **cur)
{
	int	fd;

	if (*cur || !ctx->tok[(ctx->index)] || !ctx->tok[(ctx->index) + 1])
		return (0);
	if (ft_strncmp(ctx->tok[(ctx->index)], ">", 1) != 0)
		return (0);
	(ctx->p_argc) = count_args(ctx->tok, (ctx->index));
	*cur = new_cmd((ctx->p_argc));
	if (!*cmds)
		*cmds = *cur;
	fd = open(ctx->tok[(ctx->index) + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	(*cur)->outfile = fd;
	(ctx->index) += 2;
	return (1);
}

static int	do_step(t_cmd **cmds, t_cmd **current, t_pipe_ctx *ctx, t_exec exec)
{
	if (!*current && (leading_in(ctx, cmds, current) || leading_out(ctx, cmds,
				current)))
		return (1);
	if (!*current)
	{
		(ctx->p_argc) = count_args(ctx->tok, (ctx->index));
		*current = new_cmd((ctx->p_argc));
		if (!*cmds)
			*cmds = *current;
		(ctx->arg_pos) = 0;
	}
	if (handle_pipe(cmds, current, current, ctx)
		|| handel_heredoc(current, ctx, exec)
		|| handle_in_redirection(current, ctx)
		|| handle_out_redirection(current, ctx)
		|| handle_dolar(ctx, exec))
		return (1);
	handle_argument(current, ctx);
	return (1);
}

t_cmd	*add_to_struct(char **tokenized, t_exec exec, t_pipe_ctx *ctx)
{
	t_cmd	*cmds;
	t_cmd	*current;

	cmds = NULL;
	current = NULL;
	ctx->index = 0;
	if (!tokenized)
		return (NULL);
	ctx->tok = tokenized;
	while (tokenized[ctx->index])
		if (do_step(&cmds, &current, ctx, exec))
			continue ;
	if (cmds)
		set_prev_fd(cmds);
	return (cmds);
}
