/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:16:49 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/24 12:08:59 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniShell.h"

static void	init_ctx(t_pipe_ctx *ctx)
{
	ctx -> index = 0;
	ctx -> p_argc = 0;
	ctx -> arg_pos = 0;
}

int	process_tokens(char *rl, t_exec *exec, char **envp)
{
	char		**tokenized;
	t_cmd		*cmds;
	t_pipe_ctx	ctx;

	exec->sticky_exit = 0;
	tokenized = ft_token(rl, exec);
	if (!tokenized)
		return (0);
	init_ctx(&ctx);
	cmds = add_to_struct(tokenized, exec, &ctx);
	init_exec_struct(cmds, exec, envp);
	exec->cmds = cmds;
	if (!cmds)
	{
		free_resources(tokenized);
		return (0);
	}
	execute_commands(exec, tokenized);
	wait_processes(exec);
	free_resources(tokenized);
	cleanup_exec(exec);
	if (cmds)
		free_cmds(cmds);
	return (1);
}
