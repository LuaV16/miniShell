/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:06:53 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/09/22 14:23:54 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

static t_cmd *new_cmd(void)
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
	cmd->args = malloc(sizeof(char *) * 64);// TODO: poner la cantidad de argumentos necesarios no 64
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
	
	if (!tokenized[*index])
		return (0);
	if (tokenized[*index][0] == '|' && tokenized[*index][1] == '\0')
	{
		new_command = new_cmd();
		if (!new_command)
			return (0);
		if (*current)
		{
			(*current)->next = new_command;
		}
		else 
		{
			*cmds = new_command;
		}
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
	
	if (!tokenized[*index] || !tokenized[(*index) + 1])
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

int	handle_in_redirection(t_cmd **current, char **tokenized, int *index)
{
	int	fd;
	
	if (!tokenized[*index] || !tokenized[(*index) + 1])
		return (0);
	if (ft_strncmp(tokenized[*index], "<", ft_strlen(tokenized[*index])) == 0)
	{
		fd = open(tokenized[(*index) + 1], O_RDONLY);
		if (fd < 0)
		{
			perror("open infile");
			exit(1);
		}
		if (*current)
			(*current )-> infile = fd;
		(*index)+= 2;
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
		curr->prev_fd = 0; // Inicializar a stdin por defecto
		curr = curr->next;
	}
}

void	handle_argument(t_cmd **cmds, t_cmd **current, t_cmd **last, int *arg_pos, char **tokenized, int *index)
{
	if (!tokenized[*index])
		return ;
	if (!(*current))
	{
		*current = new_cmd();
		if (!(*cmds))
		{
			*cmds = *current;
			*last = *current;
		}
		*arg_pos = 0;
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
	if (!tokenized)
		return (NULL);
	while (tokenized[index])
	{
		if (handle_pipe(&cmds, &current, &last, &arg_pos, tokenized, &index))
			continue;
		else if (handle_in_redirection(&current, tokenized, &index))
			continue;
		else if (handle_out_redirection(&current, tokenized, &index))
			continue;
		else
			handle_argument(&cmds, &current, &last, &arg_pos, tokenized, &index);
	}
	if (cmds)
		set_prev_fd(cmds);	
	return (cmds);
}
