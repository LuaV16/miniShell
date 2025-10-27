/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvargas- <lvargas-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 13:19:24 by lvargas-          #+#    #+#             */
/*   Updated: 2025/10/27 17:02:36 by lvargas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniShell.h"

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

void	bucle(t_exec *exec, char **envp)
{
	char	*rl;
	char	*res;

	while (1)
	{
		res = ft_strjoin(getenv("USER"), "@minishell$> ");
		rl = readline(res);
		free(res);
		if (handle_null_input(rl))
			break ;
		process_input(&rl);
		if (!rl)
			continue ;
		if (!process_tokens(rl, exec, envp))
		{
			free(rl);
			continue ;
		}
		free(rl);
		if (exec->sticky_exit == 1)
			break ;
	}
}

int	main(int argc, char *argv[], char **envp)
{
	t_exec	exec;

	(void)argc;
	(void)argv;
	init_shell(&exec);
	exec.envp = NULL;
	bucle(&exec, envp);
	if (exec.envp)
		free_resources(exec.envp);
	return (exec.exit);
}
