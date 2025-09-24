/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_exec_struct.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 10:27:15 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/09/24 14:03:18 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

void	free_resources(char **str)
{
	int	index;

	if (!str)
		return ;
	index = 0;
	while (str[index])
	{
		free(str[index]);
		index++;
	}
	free(str);
}

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

char	*find_command(char **path, char *command)
{
	int	j;
	char	*full_path;

	j = 0;
	while (path[j])
	{
		full_path = (char*)malloc(sizeof(char) * (ft_strlen(path[j]) + ft_strlen(command) + 2));
		ft_strlcpy(full_path, path[j], ft_strlen(path[j]) + 2);
		ft_strlcat(full_path, "/", ft_strlen(path[j]) + 2);
		ft_strlcat(full_path, command, ft_strlen(command) + ft_strlen(path[j]) + 2);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		j++;
	}
	return (ft_strdup(""));
}

char	*init_cmd_path(t_exec exec, char *command)
{
	int	index;
	char	*path_envp;
	char	**path;
	char	*result;

	index = 0;
	path_envp = NULL;
	while (exec.envp[index] && ft_strncmp(exec.envp[index], "PATH=", ft_strlen("PATH=")) != 0)
		index++;
	if (!exec.envp[index])
		return (NULL);
	path_envp = exec.envp[index] + 5;
	if (!*path_envp)
		return (NULL);
	path = ft_split(path_envp, ':');
	result = find_command(path, command);
	free_resources(path);
	return (result);
}
char **get_cmd_paths(t_exec exec, t_cmd *cmds)
{
	int	total_cmds;
	int	index;
	char	**cmd_paths;
	t_cmd	*current;

	total_cmds = exec.count_cmds;
	index = 0;
	cmd_paths = (char**)malloc(sizeof(char*) * (total_cmds + 1));
	if (!cmd_paths)
		return (NULL);
	current = cmds;
	while (index < total_cmds && current)
	{
		cmd_paths[index] = init_cmd_path(exec, current->command);
		current = current -> next;
		index++;
	}
	cmd_paths[index] = NULL;
	return (cmd_paths);
	
}
void	init_exec_struct(t_cmd *cmds, t_exec *exec, char **envp)
{
	exec -> cmds  = cmds;
	exec -> count_cmds = cmds_size(exec -> cmds);
	exec -> envp = envp;
	exec -> pids = (pid_t*)malloc(sizeof(pid_t) * exec -> count_cmds);
	if (!exec -> pids)
		exit(EXIT_FAILURE);
	exec -> cmd_paths = get_cmd_paths(*exec, cmds);
}
int	is_last(int index, t_exec exec)
{
	if (index == (exec.count_cmds - 1))
		return (1);
	return (0);
}

void	init_pipe(t_exec *exec)
{
	if (pipe(exec->pipefd) == -1)
	{
		perror("pipe error");
		exit(EXIT_FAILURE);
	}
}

void	redirect_stdio(t_exec exec, t_cmd *cmd, int index)
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
			dup2(exec.pipefd[1], STDOUT_FILENO);
	}
}

void	clean_child(int index, t_exec exec, t_cmd *cmd)
{
	if (cmd->prev_fd > 0)
		close(cmd->prev_fd);
	if (!is_last(index, exec))
	{
		close(exec.pipefd[0]);
		close(exec.pipefd[1]);
	}
	else if (cmd->outfile > 1)
		close(cmd->outfile);
	if (cmd->infile > 0)
		close(cmd->infile);
}

void	free_context(t_exec exec, t_cmd *cmds, int exit_flags, char **tokenized)
{
	t_cmd	*current;

	current = cmds;
	while (current)
	{
		if (current -> args)
			free_resources(current -> args);
		current = current -> next;
	}
	if (exec.pids)
		free(exec.pids);
	if (exec.cmd_paths)
	{
		free_resources(exec.cmd_paths);
		free_resources(tokenized);
	}
	if (exit_flags > 0)
		exit(127);
	free_resources(tokenized);
}

void	exec_child(t_cmd *cmds, t_exec exec, int index, char **tokenized)
{
	if (execve(exec.cmd_paths[index], cmds->args, exec.envp) == -1)
	{
		printf("Command '%s' not found\n", cmds -> command);
		exit(EXIT_FAILURE);
		free_context(exec, cmds, 1, tokenized);
	}
}

void	clean_parent(int index, t_exec exec, t_cmd *cmds)
{
	if (cmds->prev_fd > 0)
		close(cmds->prev_fd);	
	if (!is_last(index, exec))
	{
		close(exec.pipefd[1]);
		cmds->next->prev_fd = exec.pipefd[0];
	}
	else if (cmds->outfile > 1)
		close(cmds->outfile);
}

pid_t	fork_procces(int index, t_exec *exec, t_cmd *cmd, char **tokenized)
{
	pid_t	pid;
	
	if (!is_last(index, *exec))
		init_pipe(exec);
	if (!cmd -> command || !exec->cmd_paths)
	{
		perror("Command not found");
		free_context(*exec, exec->cmds, 0, tokenized);
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == 0)
	{
		redirect_stdio(*exec, cmd, index);
		clean_child(index, *exec, cmd);
		exec_child(cmd, *exec, index, tokenized);
	}
	else if(pid < 0)
	{
		perror("pid error");
		free_context(*exec, exec->cmds, 0, tokenized);
		exit(STDERR_FILENO);
	}
	clean_parent(index, *exec, cmd);
	return (pid);
}
