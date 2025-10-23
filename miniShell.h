/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniShell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:05:38 by lvargas-          #+#    #+#             */
/*   Updated: 2025/10/23 13:16:11 by aldiaz-u         ###   ########.fr       */
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
void							null_content(char c, char quote);
void							close_quotes(char c);
void 	sigint_setup(void);
void	sigquit_setup(void);

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
	int				heredoc_fd;
	char			*heredoc_lim;
}				t_cmd;

typedef struct	s_exec
{
	int				count_cmds;
	pid_t			*pids;
	t_cmd			*cmds;
	int				pipefd[2];
	char			**envp;
	char			**cmd_paths;
	int				*quote_type;
	int				exit;
	int				sticky_exit; // when set, keep exit code regardless of child statuses
}				t_exec;

typedef struct s_fill_ctx
{
	char	*rl;
	int	*i;
	char	**res;
	int	*j;
	int	*quote_type;
}				t_fill_ctx;

typedef struct	s_pipe_ctx
{
	int arg_pos;
	char **tok;
	int p_argc;
	int index;
}				t_pipe_ctx;

typedef struct s_token_ctx
{
	char	**res;
	int		*quote_type;
	int		*j;
}				t_token_ctx;


char							**ft_token(char *rl, t_exec *exec);
t_cmd					*add_to_struct(char **tokenized, t_exec exec, t_pipe_ctx *ctx);
void					init_exec_struct(t_cmd *cmds, t_exec *exec, char **envp);
void					free_context(t_exec exec, t_cmd *cmds, int exit_flags, char **tokenized);
void					free_resources(char **str);
int						count_args(char **tokenized, int start);
pid_t					fork_procces(int index, t_exec *exec, t_cmd *cmd, char **tokenized);
void					free_cmds(t_cmd *cmds);
int					first_char_is_special(t_pipe_ctx *ctx, char **pending_infile, char **pending_outfile);
int					handle_dolar(t_pipe_ctx *ctx, t_exec exec);
int					expand_dollar_var(t_pipe_ctx *ctx);
int					expand_dollar_next(t_pipe_ctx *ctx);
void					handle_argument(t_cmd **current, t_pipe_ctx *ctx);
int						is_space(char c);
void					set_prev_fd(t_cmd *cmds);
int	is_special_char(char c);
int check_valid_identifier(char *s);
char **dup_envp(char **envp);
int builtin_export(t_cmd *cmd, t_exec *exec);
void add_or_replace_envp(t_exec *exec, char *entry);
int builtin_unset(t_cmd *cmd, t_exec *exec);
int	builtin_env(t_exec *exec);
#endif
