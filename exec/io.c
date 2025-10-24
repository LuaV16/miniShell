/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:32:15 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/24 12:32:25 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	is_last(int index, t_exec *exec)
{
	if (index == (exec->count_cmds - 1))
		return (1);
	return (0);
}

void	init_pipe(t_exec *exec)
{
	if (pipe(exec->pipefd) == -1)
	{
		perror("pipe error");
		exec -> exit = 127;
		exit(127);
	}
}

void	redirect_stdio(t_exec *exec, t_cmd *cmd, int index)
{
	if (cmd->prev_fd > 0)
		dup2(cmd->prev_fd, STDIN_FILENO);
	if (cmd->infile > 0)
		dup2(cmd->infile, STDIN_FILENO);
	if (is_last(index, exec))
	{
		if (cmd->outfile > 1)
			dup2(cmd->outfile, STDOUT_FILENO);
	}
	else
	{
		if (cmd->outfile > 1)
			dup2(cmd->outfile, STDOUT_FILENO);
		else
			dup2(exec->pipefd[1], STDOUT_FILENO);
	}
}

void	clean_child(int index, t_exec *exec, t_cmd *cmd)
{
	if (cmd->prev_fd > 0)
		close(cmd->prev_fd);
	if (!is_last(index, exec))
	{
		close(exec->pipefd[0]);
		close(exec->pipefd[1]);
	}
	else if (cmd->outfile > 1)
		close(cmd->outfile);
	if (cmd->infile > 0)
		close(cmd->infile);
}

void	clean_parent(int index, t_exec *exec, t_cmd *cmds)
{
	if (cmds->prev_fd > 0)
		close(cmds->prev_fd);
	if (!is_last(index, exec))
	{
		close(exec->pipefd[1]);
		cmds->next->prev_fd = exec->pipefd[0];
	}
	else if (cmds->outfile > 1)
		close(cmds->outfile);
}
