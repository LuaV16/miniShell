/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniShell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:05:38 by lvargas-          #+#    #+#             */
/*   Updated: 2025/09/24 10:30:42 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void							signal_setup(void);
char							**ft_token(char *rl);
void							null_content(char c, char quote);
void							close_quotes(char c);
void 	sigint_setup(void);
void	sigquit_setup(void);
int	is_special_char(char c);


typedef struct	s_cmd
{
	char			*command;
	char			**args;
	int				infile;
	char			*pending_infile;
	char			*pending_outfile;
	int				outfile;
	int				prev_fd;
	struct s_cmd	*next;
}				t_cmd;

typedef struct	s_exec
{
	int				count_cmds;
	pid_t			*pids;
	t_cmd			*cmds;
	int				pipefd[2];
	char			**envp;
	char			**cmd_paths;
}				t_exec;		
t_cmd					*add_to_struct(char **tokenized);
void					init_exec_struct(t_cmd *cmds, t_exec *exec, char **envp);
void					free_context(t_exec exec, t_cmd *cmds, int exit_flags);
pid_t					fork_procces(int index, t_exec *exec, t_cmd *cmd);
#endif
