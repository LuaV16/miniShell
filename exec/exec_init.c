/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:29:28 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 19:00:29 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	cmds_size(t_cmd *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd -> next;
	}
	return (count);
}

void	init_exec_struct(t_cmd *cmds, t_exec *exec, char **envp)
{
	exec -> cmds = cmds;
	exec -> count_cmds = cmds_size(exec -> cmds);
	exec -> envp = dup_envp(envp);
	exec -> pids = (pid_t *)malloc(sizeof(pid_t) * exec -> count_cmds);
	if (!exec -> pids)
	{
		exec->exit = 127;
		exit(127);
	}
	exec -> cmd_paths = get_cmd_paths(*exec, cmds);
}
