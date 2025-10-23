/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:06:53 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 19:35:02 by aldiaz-u         ###   ########.fr       */
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
