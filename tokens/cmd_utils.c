/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:33:52 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 19:43:31 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

t_cmd	*new_cmd(int argc)
{
	int		index;
	t_cmd	*cmd;

	index = 0;
	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_bzero(cmd, sizeof(t_cmd));
	cmd->infile = 0;
	cmd->outfile = 1;
	cmd->prev_fd = 0;
	cmd->command = NULL;
	cmd->next = NULL;
	cmd->args = malloc(sizeof(char *) * (argc + 1));
	cmd->heredoc_fd = -1;
	cmd->heredoc_lim = NULL;
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	while (index < (argc + 1))
		cmd->args[index++] = NULL;
	return (cmd);
}

void	set_prev_fd(t_cmd *cmds)
{
	t_cmd	*curr;

	curr = cmds;
	while (curr)
	{
		curr->prev_fd = 0;
		curr = curr->next;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*current;
	t_cmd	*next;

	current = cmds;
	while (current)
	{
		if (current->command)
			free(current->command);
		if (current->args)
			free_resources(current->args);
		if (current->heredoc_lim)
			free(current->heredoc_lim);
		if (current->infile > 0)
			close(current->infile);
		if (current->outfile > 1)
			close(current->outfile);
		current = current->next;
	}
	while (cmds)
	{
		next = cmds->next;
		free(cmds);
		cmds = next;
	}
}
