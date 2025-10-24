/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 13:00:00 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/24 13:05:03 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

void	cleanup_exec(t_exec *exec)
{
	if (!exec)
		return ;
	if (exec->pids)
	{
		free(exec->pids);
		exec->pids = NULL;
	}
	if (exec->cmd_paths)
	{
		free_resources(exec->cmd_paths);
		exec->cmd_paths = NULL;
	}
	if (exec->quote_type)
	{
		free(exec->quote_type);
		exec->quote_type = NULL;
	}
}

void	free_context(t_exec *exec, t_cmd *cmds,
			int exit_flags, char **tokenized)
{
	t_cmd	*current;

	current = cmds;
	while (current)
	{
		if (current->args)
			free_resources(current->args);
		current = current->next;
	}
	if (exec->pids)
		free(exec->pids);
	if (exec->cmd_paths)
		free_resources(exec->cmd_paths);
	if (tokenized && tokenized != exec->cmd_paths)
		free_resources(tokenized);
	if (exec->quote_type)
		free(exec->quote_type);
	if (exec->envp)
		free_resources(exec->envp);
	if (exit_flags > 0)
	{
		exec->exit = 127;
		exit(127);
	}
}
