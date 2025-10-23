/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:21:58 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 19:25:38 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

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
