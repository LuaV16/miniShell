/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:45:21 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 18:45:49 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	builtin_cd(t_cmd *cmd)
{
	char	*target;

	if (!cmd -> args[1] || cmd -> args[1][0] == '\0')
	{
		target = getenv("HOME");
		if (!target)
			return (perror("cd : HOME nor set"), 1);
	}
	else
		target = cmd -> args[1];
	if (chdir(target) != 0)
		return (perror("cd"), 1);
	return (0);
}
