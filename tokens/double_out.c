/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   double_out.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 12:55:19 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/24 12:59:57 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	handle_double_out_redirection(t_cmd **current, t_pipe_ctx *ctx)
{
	int	fd;

	if (!ctx->tok[ctx->index] || !ctx->tok[(ctx->index) + 1] || !(*current))
		return (0);
	if (ft_strncmp(ctx->tok[ctx->index],
			">>", ft_strlen(ctx->tok[ctx->index])) == 0)
	{
		if (!ctx->tok[ctx->index + 1])
			return (0);
		fd = open(ctx->tok[ctx->index + 1],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
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

int	leading_double_out(t_pipe_ctx *ctx, t_cmd **cmds, t_cmd **cur)
{
	int	fd;

	if (*cur || !ctx->tok[(ctx->index)] || !ctx->tok[(ctx->index) + 1])
		return (0);
	if (ft_strncmp(ctx->tok[(ctx->index)], ">>", 2) != 0)
		return (0);
	(ctx->p_argc) = count_args(ctx->tok, (ctx->index));
	*cur = new_cmd((ctx->p_argc));
	if (!*cmds)
		*cmds = *cur;
	fd = open(ctx->tok[(ctx->index) + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
	(*cur)->outfile = fd;
	(ctx->index) += 2;
	return (1);
}
