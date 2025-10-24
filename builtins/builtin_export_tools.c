/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_tools.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:35:37 by lvargas-          #+#    #+#             */
/*   Updated: 2025/10/24 12:35:56 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

char	**dup_envp(char **envp)
{
	int		i;
	char	**copy;

	i = 0;
	if (!envp)
		return (NULL);
	while (envp[i])
		i++;
	copy = malloc(sizeof(char *) * (i + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

int	check_valid_identifier(char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (!(ft_isalpha((int)s[0]) || s[0] == '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	new_env(t_exec *exec, char *entry, int *i)
{
	char	**newenv;
	int		j;
	int		count;

	newenv = malloc(sizeof(char *) * (*i + 2));
	if (!newenv)
		return ;
	j = 0;
	count = *i;
	while (j < count)
	{
		newenv[j] = exec -> envp[j];
		j++;
	}
	newenv[j] = ft_strdup(entry);
	newenv[j + 1] = NULL;
	free(exec -> envp);
	exec->envp = newenv;
}

int	replace_env(t_exec *exec, char *entry, int name_len, int *i)
{
	char	*eq_in_entry;

	*i = 0;
	eq_in_entry = ft_strchr(entry, '=');
	while (exec->envp && exec->envp[*i])
	{
		if (ft_strncmp(exec->envp[*i], entry, name_len) == 0
			&& (exec->envp[*i][name_len] == '='
			|| exec->envp[*i][name_len] == '\0'))
		{
			if (eq_in_entry)
			{
				free(exec->envp[*i]);
				exec->envp[*i] = ft_strdup(entry);
			}
			return (1);
		}
		(*i)++;
	}
	return (0);
}

void	add_or_replace_envp(t_exec *exec, char *entry)
{
	int		i;
	int		name_len;
	char	*eq;

	if (!entry || !exec)
		return ;
	eq = ft_strchr((char *)entry, '=');
	if (eq != NULL)
		name_len = eq - entry;
	else
		name_len = ft_strlen(entry);
	if (exec->envp)
	{
		if (replace_env(exec, entry, name_len, &i) == 1)
			return ;
		if (eq != NULL)
			new_env(exec, entry, &i);
		return ;
	}
	exec->envp = malloc(sizeof(char *) * 2);
	if (!exec->envp)
		return ;
	exec->envp[0] = ft_strdup(entry);
	exec->envp[1] = NULL;
}
