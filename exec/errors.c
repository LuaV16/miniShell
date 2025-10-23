/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:36:39 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 18:37:57 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

void	prin_cmd_not_found(char *name)
{
	char	*message;

	message = ": Command not found\n";
	write(2, name, ft_strlen(name));
	write(2, message, ft_strlen(message));
}
