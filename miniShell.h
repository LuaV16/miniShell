/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniShell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:05:38 by lvargas-          #+#    #+#             */
/*   Updated: 2025/10/23 19:48:06 by aldiaz-u         ###   ########.fr       */
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
# include <sys/types.h>
# include <sys/wait.h>
# include <linux/limits.h>
# include <sys/stat.h>

void	signal_setup(void);
void	null_content(char c, char quote);
void	close_quotes(char c);
void	sigint_setup(void);
void	sigquit_setup(void);

typedef struct s_cmd
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
}	t_cmd;

typedef struct s_exec
{
	int		count_cmds;
	pid_t	*pids;
	t_cmd	*cmds;
	int		pipefd[2];
	char	**envp;
	char	**cmd_paths;
	int		*quote_type;
	int		exit;
	int		sticky_exit;
}	t_exec;

typedef struct s_fill_ctx
{
	char	*rl;
	int		*i;
	char	**res;
	int		*j;
	int		*quote_type;
}	t_fill_ctx;

typedef struct s_pipe_ctx
{
	int		arg_pos;
	char	**tok;
	int		p_argc;
	int		index;
}	t_pipe_ctx;

typedef struct s_token_ctx
{
	char	**res;
	int		*quote_type;
	int		*j;
}	t_token_ctx;

char	**ft_token(char *rl, t_exec *exec);
t_cmd	*add_to_struct(char **tokenized, t_exec exec, t_pipe_ctx *ctx);
void	init_exec_struct(t_cmd *cmds, t_exec *exec, char **envp);
void	free_context(t_exec exec, t_cmd *cmds, int exit_flags,
			char **tokenized);
void	free_resources(char **str);
int		count_args(char **tokenized, int start);
pid_t	fork_procces(int index, t_exec *exec, t_cmd *cmd, char **tokenized);
void	free_cmds(t_cmd *cmds);
int		first_char_is_special(t_pipe_ctx *ctx, char **pending_infile,
			char **pending_outfile);
int		handle_dolar(t_pipe_ctx *ctx, t_exec exec);
int		expand_dollar_var(t_pipe_ctx *ctx);
int		expand_dollar_next(t_pipe_ctx *ctx);
void	handle_argument(t_cmd **current, t_pipe_ctx *ctx);
int		is_space(char c);
void	set_prev_fd(t_cmd *cmds);
int		is_special_char(char c);
int		check_valid_identifier(char *s);
char	**dup_envp(char **envp);
int		builtin_export(t_cmd *cmd, t_exec *exec);
void	add_or_replace_envp(t_exec *exec, char *entry);
int		builtin_unset(t_cmd *cmd, t_exec *exec);
int		builtin_env(t_exec *exec);
void	init_shell(t_exec *exec);
void	process_input(char **rl);
int		handle_null_input(char *rl);
int		process_tokens(char *rl, t_exec *exec, char **envp);
int		execute_commands(t_exec *exec, char **tokenized);
void	wait_processes(t_exec *exec);
void	cleanup_exec(t_exec *exec);
int		is_last(int index, t_exec exec);
void	init_pipe(t_exec *exec);
void	redirect_stdio(t_exec exec, t_cmd *cmd, int index);
void	clean_child(int index, t_exec exec, t_cmd *cmd);
void	clean_parent(int index, t_exec exec, t_cmd *cmd);
void	exec_child(t_cmd *cmds, t_exec exec, int index);
int		is_directory(t_cmd *cmd, t_exec *exec);
void	prin_cmd_not_found(char *name);
int		builtin_cd(t_cmd *cmd);
int		builtin_pwd(void);
int		builtin_echo(t_cmd *cmd);
int		is_builtin_name(t_cmd *cmds);
int		exec_builtin(t_cmd *cmd, t_exec *exec);
char	**get_cmd_paths(t_exec exec, t_cmd *cmds);
char	*init_cmd_path(t_exec exec, char *command);
char	*find_command(char **path, char *command);
int		handle_builtin_direct(t_exec *exec, t_cmd *cmd, int idx);
int		validate_command(t_exec *exec, t_cmd *cmd, int idx);
void	exec_child_process(t_exec *exec, t_cmd *cmd, int idx);
int		fill_special_chars(char *rl, int *i, char **res, int *j);
int		fill_quotes(t_fill_ctx *ctx);
char	*extract_token(char *rl, int *i);
int		has_any_quote(char *str);
char	*remove_quotes(char *s);
int		count_words(char *rl);
void	skip_normal_token(char *rl, int *index);
void	skip_special_token(char *rl, int *index);
void	process_quote_token(char *rl, int *index, int *has_quotes);
int		replace_exit_code(char **tokenized, int index, t_exec exec);
int		handle_dollar_only(t_pipe_ctx *ctx);
int		handle_dollar_var_case(t_pipe_ctx *ctx);
void	shift_tokens_left(char **tok, int start, int shift);
int		handle_pipe(t_cmd **cmds, t_cmd **current, t_cmd **prev,
			t_pipe_ctx *ctx);
int		handle_in_redirection(t_cmd **current, t_pipe_ctx *ctx);
int		handle_out_redirection(t_cmd **current, t_pipe_ctx *ctx);
int		leading_in(t_pipe_ctx *ctx, t_cmd **cmds, t_cmd **current);
int		leading_out(t_pipe_ctx *ctx, t_cmd **cmds, t_cmd **current);
int		handel_heredoc(t_cmd **current, t_pipe_ctx *ctx, t_exec exec);
t_exec	create_heredoc_exec(t_exec exec, int has_quotes);
void	set_heredoc_fd(t_cmd *current, int fd, char *lim);
int		build_heredoc(char *lim, t_exec exec, int has_quotes);
t_cmd	*new_cmd(int argc);

#endif
