/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvargas- <lvargas-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 16:44:03 by lvargas-          #+#    #+#             */
/*   Updated: 2025/10/27 16:46:25 by lvargas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	builtin_exit(t_exec *exec)
{
	printf("exit\n");
	if (exec)
		exec->sticky_exit = 1;
	return (0);
}
