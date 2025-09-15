/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:19:24 by lvargas-          #+#    #+#             */
/*   Updated: 2025/09/15 14:06:45 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniShell.h"

int	main(void)
{
	char	*rl;
	char	**tokenized;
	int		index;

	signal_setup();
	while (1)
	{
		rl = readline("miniShell$> ");
		if (rl != NULL && *rl)
			add_history(rl);
		tokenized = ft_token(rl);
		index = 0;
		while (tokenized[index])
		{
			printf("%s\n", tokenized[index]);
			index++;
		}
		rl = NULL;
		if (g_signal == SIGINT)
		{
			rl_replace_line("", 0);
			rl_on_new_line();
			write(1, "\n", 1);
			rl_redisplay();
			g_signal = 0;
		}
		// else if (g_signal == SIGQUIT)
	}
	return (0);
}
