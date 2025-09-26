/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:19:24 by lvargas-          #+#    #+#             */
/*   Updated: 2025/09/26 13:29:19 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniShell.h"

void	print_values(t_exec exec)
{
	int	index;
	int	j;

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
		printf("COMANDO[%i] %s\n", index, exec.cmds->command);
		printf("PREV_INFILE[%i] %i\n", index, exec.cmds->prev_fd);
		printf("INFILE[%i] %i\n", index, exec.cmds->infile);
		printf("OUTFILE[%i] %i\n", index, exec.cmds->outfile);
		j = 0;
		while (exec.cmds->args[j])
		{
			printf("ARGS[%i] %s", index, exec.cmds->args[j]);
			j++;
		}
		j = 0;
		while (exec.cmd_paths[j])
		{
			printf("PATHS[%i] %s", j, exec.cmd_paths[j]);
			j++;
		}
		if ((exec.cmds->args[j]))
			printf("\n");
		exec.cmds = exec.cmds->next;
		printf("\n");
	}
}

int	main(int argc, char *argv[], char **envp)
{
	char *rl;
	char **tokenized;
	t_cmd *cmds;
	t_exec exec;
	t_cmd *current;
	int index;
	int status;

	/* inicializar exit en exec */
	exec.exit = 0;
	/* last_arg feature removed */
	(void)argc;
	(void)argv;

	sigint_setup();
	sigquit_setup();
	while (1)
	{
		rl = readline("miniShell$> ");
		if (rl == NULL)
		{
			if (!isatty(STDIN_FILENO))
				break ;
			printf("exit\n");
			break ;
		}

		if (rl != NULL && *rl)
			add_history(rl);

		if (!*rl) // Línea vacía
		{
			free(rl);
			continue ;
		}
		
		tokenized = ft_token(rl, &exec);
		
		if (!tokenized)
		{
			free(rl);
			continue ;
		}
		
		cmds = add_to_struct(tokenized, exec);
		init_exec_struct(cmds, &exec, envp);
		if (!cmds)
		{
			// Liberar tokenized
			int i = 0;
			while (tokenized[i])
				free(tokenized[i++]);
			free(tokenized);
			free(rl);
			continue ;
		}

	/* tokenized strings will be freed together after execution */

		current = exec.cmds;
		index = 0;
		while (current)
		{
			pid_t rc = fork_procces(index, &exec, current, tokenized);
			if (rc > 0)
				exec.pids[index] = rc;
			else
				exec.pids[index] = -1;
			/* preserve exit code on immediate errors */
			if (rc == 127)
				exec.exit = 127;
			index++;
			current = current->next;
		}
		index = 0;
		while (index < exec.count_cmds)
		{
			if (exec.pids[index] > 0)
			{
				waitpid(exec.pids[index], &status, 0);
				if (WIFEXITED(status))
					exec.exit = WEXITSTATUS(status);
			}
			index++;
		}

	/* free tokenized array and its strings */
	free_resources(tokenized);

		if (exec.quote_type)
		{
			free(exec.quote_type);
			exec.quote_type = NULL;
		}

		/* liberar exec.cmd_paths (cada string + array) */
		if (exec.cmd_paths)
		{
			int p = 0;
			while (exec.cmd_paths[p])
				free(exec.cmd_paths[p++]);
			free(exec.cmd_paths);
			exec.cmd_paths = NULL;
		}

		/* liberar exec.pids si fue allocado en init_exec_struct */
		if (exec.pids)
		{
			free(exec.pids);
			exec.pids = NULL;
		}

		free(rl);

		if (cmds)
			free_cmds(cmds);
	}
	return (0);
}