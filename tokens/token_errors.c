/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 14:09:22 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/09/26 11:42:17 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

void	null_content(char c, char quote)
{
	if (c == quote)
	{
		strerror(errno);
		printf("error contenido nulo");
		exit(127);
	}
}

void	close_quotes(char c)
{
	if (c == '\0')
	{
		strerror(errno);
		printf("error debes cerrar las comillas");
		exit(127);
	}
}
