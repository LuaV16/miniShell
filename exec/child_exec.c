/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:34:13 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 18:35:07 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

void	exec_child(t_cmd *cmds, t_exec exec, int index)
{
	const char	*err_name;

	execve(exec.cmd_paths[index], cmds->args, exec.envp);
	err_name = "execve";
	if (cmds && cmds->args && cmds->args[0])
		err_name = cmds->args[0];
	perror(err_name);
	if (errno == ENOENT)
		exit(127);
	else
		exit(126);
}
