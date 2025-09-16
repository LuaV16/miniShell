/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniShell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 16:05:38 by lvargas-          #+#    #+#             */
/*   Updated: 2025/09/16 17:24:09 by aldiaz-u         ###   ########.fr       */
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

extern volatile sig_atomic_t	g_signal;

void							signal_setup(void);
char							**ft_token(char *rl);
void							null_content(char c, char quote);
void							close_quotes(char c);
typedef struct	s_cmd
{
	char			*command;
	char			**args;
	char			*infile;
	char			*outfile;
	struct s_cmd	*next;
}				t_cmd;
t_cmd							*add_to_struct(char **tokenized);
#endif
