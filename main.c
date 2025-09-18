/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:19:24 by lvargas-          #+#    #+#             */
/*   Updated: 2025/09/18 14:14:12 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniShell.h"

int	main(void)
{
	char	*rl;
	char	**tokenized;
	t_cmd	*cmds;

	signal_setup();
	while (1)
	{
		rl = readline("miniShell$> ");
		if (rl != NULL && *rl)
			add_history(rl);
		tokenized = ft_token(rl);
		cmds = add_to_struct(tokenized);
		print_cmds(cmds);
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
