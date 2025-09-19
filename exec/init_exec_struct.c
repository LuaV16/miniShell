/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec_struct.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:27:15 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/09/19 10:53:29 by aldiaz-u         ###   ########.fr       */
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
	exec -> cmds  = cmds;
	exec -> count_cmds = cmds_size(exec -> cmds);
	exec -> envp = envp;
	exec -> pids = (pid_t*)malloc(sizeof(pid_t) * exec -> count_cmds);
	if (!exec -> pids)
		exit(EXIT_FAILURE);
}