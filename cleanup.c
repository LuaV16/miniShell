/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:20:31 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 18:24:17 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniShell.h"

void	cleanup_exec(t_exec *exec)
{
	int	p;

	if (exec->quote_type)
	{
		free(exec->quote_type);
		exec->quote_type = NULL;
	}
	if (exec->cmd_paths)
	{
		p = 0;
		while (exec->cmd_paths[p])
			free(exec->cmd_paths[p++]);
		free(exec->cmd_paths);
		exec->cmd_paths = NULL;
	}
	if (exec->pids)
	{
		free(exec->pids);
		exec->pids = NULL;
	}
	if (exec->envp)
	{
		free_resources(exec->envp);
		exec->envp = NULL;
	}
}
