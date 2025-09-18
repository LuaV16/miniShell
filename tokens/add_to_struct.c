/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:06:53 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/09/18 14:13:11 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

static t_cmd *new_cmd(void)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_bzero(cmd, sizeof(t_cmd));
	cmd -> args = malloc(sizeof(char *) * 64);// TODO: poner la cantida de argumentos necesarios no 64
	return (cmd);
}

int	handle_pipe(t_cmd **cmds, t_cmd **current, t_cmd **last, int *arg_pos, char **tokenized, int *index)
{
	t_cmd	*new;
	
	new = new_cmd();
	if (ft_strncmp(tokenized[*index] , "|", ft_strlen(tokenized[*index])) == 0)
	{
			if (*last)
				(*last) -> next = new;
			else
				*cmds = new;
			*current = new;
			*last = new;
			*arg_pos = 0;
			(*index)++;
			return (1);
	}
	return (0);
}

int	handle_out_redirection(t_cmd **current, char **tokenized, int *index)
{
	int	fd;
	
	if (ft_strncmp(tokenized[*index], ">", ft_strlen(tokenized[*index])) == 0)
	{
		fd = open(tokenized[(*index) + 1], O_RDONLY | O_CREAT);
		if (*current)
			(*current) -> outfile = fd;
		(*index)+= 2;
		return (1);
	}
	return (0);
}

int	handle_in_redirection(t_cmd **current, char **tokenized, int *index)
{
	int	fd;
	
	if (ft_strncmp(tokenized[*index], "<", ft_strlen(tokenized[*index])) == 0)
	{
		fd = open(tokenized[(*index) + 1], O_RDONLY | O_CREAT);
		if (*current)
			(*current )-> infile = fd;
		(*index)+= 2;
		return (1);
	}
	return (0);
}

void	handle_argument(t_cmd **cmds, t_cmd **current, t_cmd **last, int *arg_pos, char **tokenized, int *index)
{
	if (!(*current)) {
		*current = new_cmd();
		if (!(*cmds))
		{
			*cmds = *current;
			*last = *current;
		}
	}
	if (!(*current) -> command)
		(*current) -> command = tokenized[*index];
	(*current) -> args[(*arg_pos)++] = tokenized[*index];
	(*current) -> args[(*arg_pos)] = NULL;
	(*index)++;
}

t_cmd	*add_to_struct(char **tokenized)
{
	int	index;
	int	arg_pos;
	t_cmd	*cmds;
	t_cmd	*current;
	t_cmd	*last;

	index = 0;
	arg_pos = 0;
	cmds = NULL;
	current = NULL;
	last = NULL;
	while (tokenized[index])
	{
		if (handle_pipe(&cmds, &current, &last, &arg_pos, tokenized, &index))
			continue ;
		else if (handle_in_redirection(&current, tokenized, &index))
			continue ;
		else if (handle_out_redirection(&current, tokenized, &index))
			continue ;
		else
			handle_argument(&cmds, &current, &last, &arg_pos, tokenized, &index);
	}
	return (cmds);
}
