/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:18:53 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 18:24:10 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniShell.h"

int	execute_commands(t_exec *exec, char **tokenized)
{
	t_cmd	*current;
	int		index;
	pid_t	rc;

	current = exec->cmds;
	index = 0;
	while (current)
	{
		rc = fork_procces(index, exec, current, tokenized);
		if (rc > 0)
			exec->pids[index] = rc;
		else
			exec->pids[index] = -1;
		if (rc == 127)
			exec->exit = 127;
		index++;
		current = current->next;
	}
	return (index);
}

void	wait_processes(t_exec *exec)
{
	int	index;
	int	status;

	index = 0;
	while (index < exec->count_cmds)
	{
		if (exec->pids[index] > 0)
		{
			waitpid(exec->pids[index], &status, 0);
			if (!exec->sticky_exit && WIFEXITED(status))
				exec->exit = WEXITSTATUS(status);
		}
		index++;
	}
}
