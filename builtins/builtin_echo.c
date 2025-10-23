/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:47:13 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 18:47:51 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	builtin_echo(t_cmd *cmd)
{
	int	index;
	int	has_n;

	index = 1;
	has_n = 1;
	if (cmd -> args[1] && ft_strncmp(cmd -> args[1], "-n", 3) == 0)
	{
		has_n = 0;
		index = 2;
	}
	while (cmd -> args && cmd -> args[index])
	{
		write (STDOUT_FILENO, cmd -> args[index],
			ft_strlen(cmd -> args[index]));
		if (cmd -> args[index + 1])
			write(STDOUT_FILENO, " ", 1);
		index++;
	}
	if (has_n)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
