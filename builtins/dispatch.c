/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dispatch.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvargas- <lvargas-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:48:12 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/27 16:50:29 by lvargas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	is_builtin_name(t_cmd *cmds)
{
	if (ft_strncmp(cmds -> command, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmds -> command, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmds -> command, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmds -> command, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmds -> command, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmds -> command, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(cmds -> command, "exit", 5) == 0)
		return (1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_exec *exec)
{
	if (ft_strncmp(cmd -> command, "cd", 3) == 0)
		return (builtin_cd(cmd));
	else if (ft_strncmp(cmd -> command, "pwd", 4) == 0)
		return (builtin_pwd());
	else if (ft_strncmp(cmd -> command, "echo", 5) == 0)
		return (builtin_echo(cmd));
	else if (ft_strncmp(cmd -> command, "export", 7) == 0)
		return (builtin_export(cmd, exec));
	else if (ft_strncmp(cmd -> command, "unset", 5) == 0)
		return (builtin_unset(cmd, exec));
	else if (ft_strncmp(cmd -> command, "env", 4) == 0)
		return (builtin_env(exec));
	else if (ft_strncmp(cmd -> command, "exit", 5) == 0)
		return (builtin_exit(exec));
	return (0);
}
