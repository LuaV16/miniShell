/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:06:53 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/09/24 10:49:29 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	count_args(char **tokenized, int start)
{
	int	count;

	count = 0;
	while (tokenized[start])
	{
		if (tokenized[start][0] == '|' || tokenized[start][0] == '<'|| tokenized[start][0] == '>')
			break;
		start++;
		count++;
	}
	return (count);
}

static t_cmd *new_cmd(int argc)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_bzero(cmd, sizeof(t_cmd));
	cmd->infile = 0;
	cmd->outfile = 1;
	cmd->prev_fd = 0;
	cmd->command = NULL;
	cmd->next = NULL;
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	return (cmd);
}

int	handle_pipe(t_cmd **cmds, t_cmd **current, t_cmd **last, int *arg_pos, char **tokenized, int *index)
{
	t_cmd	*new_command;
	int		argc;
	
	argc = count_args(tokenized, (*index) + 1);
	if (!tokenized[*index])
		return (0);
	if (tokenized[*index][0] == '|' && tokenized[*index][1] == '\0')
	{
		new_command = new_cmd(argc);
		if (!new_command)
			return (0);
		if (*current)
			(*current)->next = new_command;
		else 
			*cmds = new_command;
		*last = new_command;
		*current = new_command;
		*arg_pos = 0;
		(*index)++;
		return (1);
	}
	return (0);
}

int	handle_out_redirection(t_cmd **current, char **tokenized, int *index)
{
	int	fd;
	
	if (!tokenized[*index] || !tokenized[(*index) + 1] || !(*current) || !(*current) -> command)
		return (0);
	if (!*current)
		return (0);
	if (ft_strncmp(tokenized[*index], ">", ft_strlen(tokenized[*index])) == 0)
	{
		fd = open(tokenized[(*index) + 1], O_RDWR | O_CREAT, 0644);
		if (*current)
			(*current) -> outfile = fd;
		(*index)+= 2;
		return (1);
	}
	return (0);
}

int	handle_in_redirection(t_cmd **current, char **tokenized, int *index, t_cmd **cmds)
{
	int	fd;
	int	argc;

	argc = count_args(tokenized, (*index) + 1);
	if (!tokenized[*index] || !tokenized[(*index) + 1] || !(*cmds) || !(*cmds) -> command)
		return (0);
	if (ft_strncmp(tokenized[*index], "<", ft_strlen(tokenized[*index])) == 0)
	{
		fd = open(tokenized[(*index) + 1], O_RDONLY);
		if (fd < 0)
		{
			perror("open infile");
			exit(1);
		}
		(*current )-> infile = fd;
		(*index)+= 2;
		return (1);
	}
	return (0);
}

int	handle_dolar(char **tokenized, int *index, t_cmd **current)
{
	char	*env;
	int		i;
	
	if (!tokenized[*index] || !tokenized[(*index) + 1] || !(*current) || !(*current) -> command)
		return (0);
	if (ft_strncmp(tokenized[*index], "$", ft_strlen(tokenized[*index])) == 0)
	{
		env = getenv(tokenized[(*index) + 1]);
		if (env)
			tokenized[*index] = ft_strdup(env);
		else
			tokenized[*index] = ft_strdup("");
		i = (*index) + 1;
		while (tokenized[i])
		{
			tokenized[i] = tokenized[i + 1]	;
			i++;
		}
		return (1);
	}
	return (0);
}

void set_prev_fd(t_cmd *cmds)
{
	t_cmd	*curr;

	curr = cmds;
	while (curr)
	{
		curr->prev_fd = 0;
		curr = curr->next;
	}
}

void	handle_argument(t_cmd **cmds, t_cmd **current, t_cmd **last, int *arg_pos, char **tokenized, int *index)
{
	int	argc;

	if (is_special_char(tokenized[*index][0]))
	{
		(*index)++;
		return ;
	}
	argc = count_args(tokenized, *index);
	if (!tokenized[*index])
		return ;
	if (!(*current))
	{
		*current = new_cmd(argc);
		if (!(*cmds))
		{
			*cmds = *current;
			*last = *current;
		}
		*arg_pos = 0;
	}
	if (!(*current) -> command && *arg_pos == 0)
		(*current) -> command = tokenized[*index];
	if (*arg_pos < argc)
	{
		(*current) -> args[(*arg_pos)++] = tokenized[*index];
		(*current) -> args[(*arg_pos)] = NULL;
	}
	(*index)++;
}

int	first_char_is_special(char **tokenized, int *index, char	**pending_infile, char	**pending_outfile)
{
	if (*tokenized[*index] && *tokenized[*index + 1] && ft_strncmp(tokenized[*index], "<", ft_strlen(tokenized[*index])))
	{
		*pending_infile = tokenized[(*index) + 1];
		(*index) += 2;
		return (1);
	}

	if (*tokenized[*index] && *tokenized[*index + 1] && ft_strncmp(tokenized[*index], ">", ft_strlen(tokenized[*index])))
	{
		*pending_outfile = tokenized[(*index) + 1];
		(*index) += 2;
		return (1);
	}
	return (0);
}
t_cmd	*add_to_struct(char **tokenized)
{
	int	index;
	int	arg_pos;
	t_cmd	*cmds;
	t_cmd	*current;
	t_cmd	*last;
	char	*pending_infile;
	char	*pending_outfile;

	index = 0;
	arg_pos = 0;
	cmds = NULL;
	current = NULL;
	last = NULL;
	pending_infile = NULL;
	pending_outfile = NULL;
	if (!tokenized)
		return (NULL);
	while (tokenized[index])
	{
		if (first_char_is_special(tokenized, &index, &pending_infile, &pending_outfile) && !current)
			continue;
		if (handle_pipe(&cmds, &current, &last, &arg_pos, tokenized, &index))
			continue;
		else if (handle_in_redirection(&current, tokenized, &index, &cmds))
			continue;
		else if (handle_out_redirection(&current, tokenized, &index))
			continue;
		else if (handle_dolar(tokenized, &index, &cmds))
			continue;
		else
		{
			handle_argument(&cmds, &current, &last, &arg_pos, tokenized, &index);
			if (pending_infile && current && current -> infile == 0)
			{
				current -> infile = open(pending_infile, O_RDONLY);
				pending_infile = NULL;
			}
			if (pending_outfile && current && current -> outfile == 1)
			{
				current -> outfile = open(pending_outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				pending_outfile = NULL;
			}
		}
	}
	if (cmds)
		set_prev_fd(cmds);	
	return (cmds);
}
