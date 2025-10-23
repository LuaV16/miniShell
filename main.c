/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:19:24 by lvargas-          #+#    #+#             */
/*   Updated: 2025/10/23 18:23:16 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniShell.h"

/*void	print_values(t_exec exec)
{
	int	index;
	int	j;

	index = 0;
	printf("=== DEBUG INFO ===\n");
	printf("Cantidad de comandos: %d\n", exec.count_cmds);
	while (exec.cmds)
	{
		printf("COMANDO[%i]: %s\n", index, exec.cmds->command);
		j = 0;
		printf("ARGS[%i]: ", index);
		while (exec.cmds->args && exec.cmds->args[j])
		{
			printf("'%s' ", exec.cmds->args[j]);
			j++;
		}
		printf("\n");
		exec.cmds = exec.cmds->next;
		index++;
	}
	printf("==================\n");
}*/

void	process_input(char **rl)
{
	if (*rl != NULL && **rl)
		add_history(*rl);
	if (!**rl)
	{
		free(*rl);
		*rl = NULL;
	}
}

int	handle_null_input(char *rl)
{
	if (rl == NULL)
	{
		if (!isatty(STDIN_FILENO))
			return (1);
		printf("exit\n");
		return (1);
	}
	return (0);
}

void	init_shell(t_exec *exec)
{
	exec->exit = 0;
	exec->sticky_exit = 0;
	sigint_setup();
	sigquit_setup();
}

int	main(int argc, char *argv[], char **envp)
{
	char	*rl;
	t_exec	exec;

	(void)argc;
	(void)argv;
	init_shell(&exec);
	exec.envp = NULL;
	while (1)
	{
		rl = readline("miniShell$> ");
		if (handle_null_input(rl))
			break ;
		process_input(&rl);
		if (!rl)
			continue ;
		if (!process_tokens(rl, &exec, envp))
		{
			free(rl);
			continue ;
		}
		free(rl);
	}
	if (exec.envp)
		free_resources(exec.envp);
	return (exec.exit);
}
