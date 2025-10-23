/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:36:04 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 19:00:19 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	is_directory(t_cmd *cmd, t_exec *exec)
{
	struct stat	path_stat;

	if (stat(cmd->command, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		write(2, cmd->command, ft_strlen(cmd->command));
		write(2, ": Is a directory\n", 17);
		exec -> exit = 126;
		exec -> sticky_exit = 126;
		return (1);
	}
	return (0);
}
