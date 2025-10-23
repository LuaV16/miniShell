/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:26:54 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 19:44:35 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

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

void	set_heredoc_fd(t_cmd *current, int fd, char *lim)
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
