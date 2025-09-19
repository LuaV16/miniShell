/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:19:24 by lvargas-          #+#    #+#             */
/*   Updated: 2025/09/19 12:08:32 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniShell.h"

void	print_values(t_exec exec)
{
	int	index;

	index = 0;
	printf("Cantidad de comnados: %d\n", exec.count_cmds);
	while (exec.envp[index])
	{
		printf("ENVP[%i]%s\n", index, exec.envp[index]);
		index++;
	}
	index = 0;
	while (exec.cmds)
	{
		printf("COMANDO[%i] %s\n", index, exec.cmds -> command);
		printf("PREV_INFILE[%i] %i\n", index, exec.cmds -> prev_fd);
		printf("INFILE[%i] %i\n", index, exec.cmds -> infile);
		printf("OUTFILE[%i] %i\n", index, exec.cmds -> outfile);
		int	j = 0;
		while (exec.cmds -> args[j])
		{
				printf("ARGS[%i] %s", index, exec.cmds -> args[j]);
				j++;
		}
		if ((exec.cmds -> args[j]))
			printf("\n");
		exec.cmds = exec.cmds -> next;
		printf("\n");
	}
}

int	main(int argc, char *argv[], char **envp)
{
	char	*rl;
	char	**tokenized;
	t_cmd	*cmds;
	t_exec	exec;
	(void)argc;
	(void)argv;

	signal_setup();
	while (1)
	{
		rl = readline("miniShell$> ");
		if (rl != NULL && *rl)
			add_history(rl);
		tokenized = ft_token(rl);
		cmds = add_to_struct(tokenized);
		init_exec_struct(cmds, &exec, envp);
		print_values(exec);
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
