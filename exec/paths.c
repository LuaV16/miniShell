/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:30:45 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 18:31:36 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

char	*find_command(char **path, char *command)
{
	int		j;
	char	*full_path;
	size_t	path_len_j;
	size_t	command_len;

	j = 0;
	command_len = ft_strlen(command);
	while (path[j])
	{
		path_len_j = ft_strlen(path[j]);
		full_path = malloc(sizeof(char) * (path_len_j + command_len + 2));
		ft_strlcpy(full_path, path[j], ft_strlen(path[j]) + 2);
		ft_strlcat(full_path, "/", ft_strlen(path[j]) + 2);
		ft_strlcat(full_path, command,
			ft_strlen(command) + ft_strlen(path[j]) + 2);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		j++;
	}
	return (NULL);
}

char	*init_cmd_path(t_exec exec, char *command)
{
	int		index;
	char	*path_envp;
	char	**path;
	char	*result;

	index = 0;
	path_envp = NULL;
	while (exec.envp[index]
		&& ft_strncmp(exec.envp[index], "PATH=", ft_strlen("PATH=")) != 0)
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

char	**get_cmd_paths(t_exec exec, t_cmd *cmds)
{
	int		total_cmds;
	int		index;
	char	**cmd_paths;
	t_cmd	*current;

	total_cmds = exec.count_cmds;
	index = 0;
	cmd_paths = (char **)malloc(sizeof(char *) * (total_cmds + 1));
	if (!cmd_paths)
		return (NULL);
	current = cmds;
	while (index < total_cmds && current)
	{
		if (current -> command && ft_strchr(current -> command, '/'))
			cmd_paths[index] = ft_strdup(current -> command);
		else
			cmd_paths[index] = init_cmd_path(exec, current->command);
		current = current -> next;
		index++;
	}
	cmd_paths[index] = NULL;
	return (cmd_paths);
}
