/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 18:28:24 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/24 13:42:23 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

void	free_resources(char **str)
{
	int	index;

	if (!str)
		return ;
	index = 0;
	while (str[index])
	{
		free(str[index]);
		index++;
	}
	free(str);
}

char	*get_var_name(char *var, char **eq)
{
	*eq = ft_strchr(var, '=');
	if (*eq)
		return (ft_substr(var, 0, *eq - var));
	return (ft_strdup(var));
}
