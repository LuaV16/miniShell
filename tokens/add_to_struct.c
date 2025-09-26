/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:06:53 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/09/26 18:34:28 by aldiaz-u         ###   ########.fr       */
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
			&& tokenized[start][1] == '\0')
		{
			if (tokenized[start + 1])
			{
				start += 2;
				continue ;
			}
		}
		start++;
		count++;
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

	if (!ctx->tok[*(ctx->index)] || ctx->tok[*(ctx->index)][0] != '|'
		|| ctx->tok[*(ctx->index)][1] != '\0' || !ctx->tok[*(ctx->index) + 1]
		|| (ctx->tok[*(ctx->index) + 1][0] == '|' && ctx->tok[*(ctx->index)
			+ 1][1] == '\0'))
		return (0);
	new_command = new_cmd(count_args(ctx->tok, *(ctx->index) + 1));
	if (!new_command)
		return (0);
	if (*current)
		(*current)->next = new_command;
	else
		*cmds = new_command;
	*last = new_command;
	*current = new_command;
	*(ctx->arg_pos) = 0;
	if (ctx->p_argc)
		*(ctx->p_argc) = count_args(ctx->tok, *(ctx->index) + 1);
	(*(ctx->index))++;
	return (1);
}

int	handle_out_redirection(t_cmd **current, char **tokenized, int *index)
{
	int	fd;

	if (!tokenized[*index] || !tokenized[(*index) + 1] || !(*current)
		|| !(*current)->command)
		return (0);
	if (!*current)
		return (0);
	if (ft_strncmp(tokenized[*index], ">", ft_strlen(tokenized[*index])) == 0)
	{
		if (!tokenized[(*index + 1)])
			return (0);
		fd = open(tokenized[(*index) + 1], O_RDWR | O_CREAT, 0644);
		if (*current)
			(*current)->outfile = fd;
		(*index) += 2;
		return (1);
	}
	return (0);
}

int	handle_in_redirection(t_cmd **current, char **tokenized, int *index)
{
	int	fd;

	if (!tokenized[*index] || !tokenized[(*index) + 1] || !(*current)
		|| !(*current)->command)
		return (0);
	if (ft_strncmp(tokenized[*index], "<", ft_strlen(tokenized[*index])) == 0)
	{
		fd = open(tokenized[(*index) + 1], O_RDONLY);
		if (fd < 0)
		{
			perror("open infile");
			exit(127);
		}
		(*current)->infile = fd;
		(*index) += 2;
		return (1);
	}
	return (0);
}

int	replace_exit_code(char **tokenized, int *index, t_exec exec)
{
	char	*exit_s;

	exit_s = ft_itoa(exec.exit);
	if (!exit_s)
		return (0);
	free(tokenized[*index]);
	tokenized[*index] = exit_s;
	return (1);
}

int	expand_dollar_next(char **tokenized, int *index)
{
	char	*env;
	int		i;

	if (!tokenized[(*index) + 1])
		return (0);
	env = getenv(tokenized[(*index) + 1]);
	free(tokenized[*index]);
	if (env)
		tokenized[*index] = ft_strdup(env);
	else
		tokenized[*index] = ft_strdup("");
	free(tokenized[(*index) + 1]);
	i = (*index) + 1;
	while (tokenized[i])
	{
		tokenized[i] = tokenized[i + 1];
		i++;
	}
	return (1);
}

int	expand_dollar_var(char **tokenized, int *index)
{
	char	*var;
	char	*env;

	var = malloc(ft_strlen(tokenized[*index] + 1) + 1);
	if (!var)
		return (0);
	ft_strlcpy(var, tokenized[*index] + 1, ft_strlen(tokenized[*index] + 1)
		+ 1);
	env = getenv(var);
	free(tokenized[*index]);
	tokenized[*index] = env;
	if (env)
		tokenized[*index] = ft_strdup(env);
	else
		tokenized[*index] = ft_strdup("");
	free(var);
	return (1);
}

int	handle_dolar(char **tokenized, int *index, t_exec exec)
{
	if (!tokenized[*index])
		return (0);
	if (exec.quote_type[*index] == 1)
		return (0);
	if (tokenized[*index][0] == '$' && tokenized[*index][1] == '?')
		return (replace_exit_code(tokenized, index, exec));
	if (ft_strncmp(tokenized[*index], "$", ft_strlen(tokenized[*index])) == 0)
		return (expand_dollar_next(tokenized, index));
	if (tokenized[*index][0] == '$')
		return (expand_dollar_var(tokenized, index));
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

	if (!ctx->tok[*ctx->index])
		return ;
	if (is_special_char(ctx->tok[*ctx->index][0]))
	{
		if (ctx->tok[(*ctx->index) + 1])
		{
			(*(ctx->index))++;
			return ;
		}
	}
	tok = ctx->tok[*ctx->index];
	if (!(*current)->command && *(ctx->arg_pos) == 0)
	{
		(*current)->command = ft_strdup(tok);
		if (!(*current)->command)
			return ;
	}
	if (*(ctx->arg_pos) < *(ctx->p_argc))
	{
		(*current)->args[*(ctx->arg_pos)] = ft_strdup(tok);
		(*(ctx->arg_pos))++;
		(*current)->args[*(ctx->arg_pos)] = NULL;
	}
	(*(ctx->index))++;
}

int	first_char_is_special(char **tokenized, int *index, char **pending_infile,
		char **pending_outfile)
{
	if (tokenized[*index] && tokenized[*index + 1]
		&& ft_strncmp(tokenized[*index], "<",
			ft_strlen(tokenized[*index])) == 0)
	{
		*pending_infile = ft_strdup(tokenized[(*index) + 1]);
		(*index) += 2;
		return (1);
	}
	if (tokenized[*index] && tokenized[*index + 1]
		&& ft_strncmp(tokenized[*index], ">",
			ft_strlen(tokenized[*index])) == 0)
	{
		*pending_outfile = ft_strdup(tokenized[(*index) + 1]);
		(*index) += 2;
		return (1);
	}
	return (0);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*current;
	int		index;
	t_cmd	*next;

	current = cmds;
	while (current)
	{
		index = 0;
		if (current->command)
			free(current->command);
		if (current->args)
			free_resources(current->args);
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

t_cmd	*add_to_struct(char **tokenized, t_exec exec)
{
	int			index;
	int			arg_pos;
	t_cmd		*cmds;
	t_cmd		*current;
	t_cmd		*last;
	char		*pending_infile;
	char		*pending_outfile;
	int			argc;
	t_pipe_ctx	pipe_ctx;

	index = 0;
	arg_pos = 0;
	cmds = NULL;
	current = NULL;
	last = NULL;
	pending_infile = NULL;
	pending_outfile = NULL;
	argc = 0;
	if (!tokenized)
		return (NULL);
	while (tokenized[index])
	{
		if (!current && first_char_is_special(tokenized, &index,
				&pending_infile, &pending_outfile))
			continue ;
		if (!(current))
		{
			argc = count_args(tokenized, index);
			current = new_cmd(argc);
			if (!(cmds))
			{
				cmds = current;
				last = current;
			}
			arg_pos = 0;
		}
		pipe_ctx.arg_pos = &arg_pos;
		pipe_ctx.tok = tokenized;
		pipe_ctx.p_argc = &argc;
		pipe_ctx.index = &index;
		if (handle_pipe(&cmds, &current, &last, &pipe_ctx))
			continue ;
		else if (handle_in_redirection(&current, tokenized, &index))
			continue ;
		else if (handle_out_redirection(&current, tokenized, &index))
			continue ;
		else if (handle_dolar(tokenized, &index, exec))
			continue ;
		else
		{
			handle_argument(&current, &pipe_ctx);
			if (pending_infile && current && current->infile == 0)
			{
				current->infile = open(pending_infile, O_RDONLY);
				if (current->infile < 0)
				{
					perror("open infile");
					exit(127);
				}
				pending_infile = NULL;
			}
			if (pending_outfile && current && current->outfile == 1)
			{
				current->outfile = open(pending_outfile,
						O_WRONLY | O_CREAT | O_TRUNC, 0644);
				pending_outfile = NULL;
			}
		}
	}
	if (pending_infile)
		free(pending_infile);
	if (pending_outfile)
		free(pending_outfile);
	if (cmds)
		set_prev_fd(cmds);
	return (cmds);
}
