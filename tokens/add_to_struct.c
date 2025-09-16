/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_struct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 17:06:53 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/09/16 17:32:11 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

static t_cmd *new_cmd(void)
{
	t_cmd *cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_bzero(cmd, sizeof(t_cmd));
	cmd -> args = malloc(sizeof(char *) * 64);
	return (cmd);
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
		if (ft_strncmp(tokenized[index] , "|", ft_strlen(tokenized[index])) == 0)
		{
			current = new_cmd();
			last -> next = current;
			last = current;
			arg_pos = 0;
			index++;
			continue;
		}
		else if(ft_strncmp(tokenized[index], "<", ft_strlen(tokenized[index])) == 0)
		{
			if (current)
				current -> infile = tokenized[index + 1];
			index += 2;;
			continue;
		}
		else if (ft_strncmp(tokenized[index], ">", ft_strlen(tokenized[index])) == 0)
		{
			if (current)
				current -> outfile = tokenized[index + 1];
			index += 2;;
			continue;
		}
		else
		{
			if (!current)
			{
				current = new_cmd();
				cmds = current;
				last = current;
				arg_pos = 0;
			}
			if (!current -> command)
				current -> command = tokenized[index];
			current -> args[arg_pos++] = tokenized[index];
			current -> args[arg_pos] = NULL;
			index++;
		}
	}
	return (cmds);
}
