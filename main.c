/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvargas- <lvargas-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:19:24 by lvargas-          #+#    #+#             */
/*   Updated: 2025/09/12 13:47:07 by lvargas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

int	main(int argc, char **argv, char **envp)
{
	char	*rl;

	while (1)
	{
		rl = readline("miniShell$> ");
		if (rl != NULL && *rl)
			add_history(rl);
		rl = NULL;
	}
	return (0);
}
