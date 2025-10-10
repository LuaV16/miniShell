/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:19:24 by lvargas-          #+#    #+#             */
/*   Updated: 2025/10/10 10:56:28 by aldiaz-u         ###   ########.fr       */
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

static void	process_input(char **rl)
{
	if (*rl != NULL && **rl)
		add_history(*rl);
	if (!**rl)
	{
		free(*rl);
		*rl = NULL;
	}
}

static int	execute_commands(t_exec *exec, char **tokenized)
{
	t_cmd	*current;
	int		index;
	pid_t	rc;

	current = exec->cmds;
	index = 0;
	while (current)
	{
		rc = fork_procces(index, exec, current, tokenized);
		if (rc > 0)
			exec->pids[index] = rc;
		else
			exec->pids[index] = -1;
		if (rc == 127)
			exec->exit = 127;
		index++;
		current = current->next;
	}
	return (index);
}

static void	wait_processes(t_exec *exec)
{
	int	index;
	int	status;

	index = 0;
	while (index < exec->count_cmds)
	{
		if (exec->pids[index] > 0)
		{
			waitpid(exec->pids[index], &status, 0);
			if (!exec->sticky_exit && WIFEXITED(status))
				exec->exit = WEXITSTATUS(status);
		}
		index++;
	}
}

static int	handle_null_input(char *rl)
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

static void	cleanup_exec(t_exec *exec)
{
	int	p;

	if (exec->quote_type)
	{
		free(exec->quote_type);
		exec->quote_type = NULL;
	}
	if (exec->cmd_paths)
	{
		p = 0;
		while (exec->cmd_paths[p])
			free(exec->cmd_paths[p++]);
		free(exec->cmd_paths);
		exec->cmd_paths = NULL;
	}
	if (exec->pids)
	{
		free(exec->pids);
		exec->pids = NULL;
	}
	if (exec->envp)
	{
		free_resources(exec->envp);
		exec->envp = NULL;
	}
}

static int	process_tokens(char *rl, t_exec *exec, char **envp)
{
	char		**tokenized;
	t_cmd		*cmds;
	t_pipe_ctx	ctx;

	exec->sticky_exit = 0;
	tokenized = ft_token(rl, exec);
	if (!tokenized)
		return (0);
	cmds = add_to_struct(tokenized, *exec, &ctx);
	init_exec_struct(cmds, exec, envp);
	exec->cmds = cmds;
	if (!cmds)
	{
		free_resources(tokenized);
		return (0);
	}
	execute_commands(exec, tokenized);
	wait_processes(exec);
	free_resources(tokenized);
	cleanup_exec(exec);
	if (cmds)
		free_cmds(cmds);
	return (1);
}

static void	init_shell(t_exec *exec)
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
