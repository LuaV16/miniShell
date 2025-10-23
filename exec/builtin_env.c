/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 13:13:02 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 13:22:20 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	builtin_env(t_exec *exec)
{
	int	index;

	index = 0;
	if (!exec || !exec -> envp)
		return (1);
	while (exec -> envp[index])
	{
		printf("%s\n", exec -> envp[index]);
		index++;
	}
	return (0);
}
