/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:38:15 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/24 12:44:00 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

pid_t	fork_procces(int index, t_exec *exec, t_cmd *cmd, char **tokenized)
{
	pid_t	pid;
	int		result;

	result = handle_builtin_direct(exec, cmd, index);
	if (result)
		return (result);
	if (!is_last(index, exec))
		init_pipe(exec);
	result = validate_command(exec, cmd, index);
	if (result)
		return (result);
	pid = fork();
	if (pid == 0)
		exec_child_process(exec, cmd, index);
	else if (pid < 0)
	{
		perror("pid error");
		free_context(exec, exec->cmds, 0, tokenized);
		exec->exit = 127;
		exit(127);
	}
	clean_parent(index, exec, cmd);
	return (pid);
}

int	is_parent_only_builtin(t_cmd *cmd)
{
	if (ft_strncmp(cmd->command, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd->command, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd->command, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd->command, "exit", 5) == 0)
		return (1);
	return (0);
}

int	handle_builtin_direct(t_exec *exec, t_cmd *cmd, int idx)
{
	int	status;

	if (!is_builtin_name(cmd))
		return (0);
	if (is_parent_only_builtin(cmd) && is_last(idx, exec))
	{
		status = exec_builtin(cmd, exec);
		exec->exit = status;
		return (-1);
	}
	if (!is_last(idx, exec) || cmd->infile > 0
		|| cmd->outfile > 1 || cmd->prev_fd > 0)
		return (0);
	status = exec_builtin(cmd, exec);
	exec->exit = status;
	return (-1);
}

void	exec_child_process(t_exec *exec, t_cmd *cmd, int idx)
{
	int	status;	

	redirect_stdio(exec, cmd, idx);
	clean_child(idx, exec, cmd);
	if (is_builtin_name(cmd))
	{
		status = exec_builtin(cmd, exec);
		exec->exit = status;
		exit(status);
	}
	exec_child(cmd, exec, idx);
}

int	validate_command(t_exec *exec, t_cmd *cmd, int idx)
{
	if (cmd->infile == -1 || cmd->outfile == -1)
	{
		exec->exit = 127;
		exec->sticky_exit = 127;
		return (1);
	}
	if (!cmd->command)
	{
		exec->exit = 127;
		exec->sticky_exit = 127;
		return (1);
	}
	if (is_directory(cmd, exec))
		return (1);
	if (!is_builtin_name(cmd) && (!exec->cmd_paths
			|| !exec->cmd_paths[idx]))
	{
		prin_cmd_not_found(cmd -> command);
		exec->exit = 127;
		exec->sticky_exit = 127;
		return (1);
	}
	return (0);
}
