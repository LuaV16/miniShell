/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvargas- <lvargas-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 17:48:09 by lvargas-          #+#    #+#             */
/*   Updated: 2025/10/16 17:48:38 by lvargas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

void remove_env_entry(char ***envp, char *name)
{
    int i;
    int j;
    size_t n;

    if (!envp || !*envp || !name || !*name)
        return ;
    n = ft_strlen(name);
    i = 0;
    while ((*envp)[i])
    {
        if (ft_strncmp((*envp)[i], name, n) == 0 && (*envp)[i][n] == '=')
        {
            free((*envp)[i]);
            j = i;
            while ((*envp)[j + 1])
            {
                (*envp)[j] = (*envp)[j + 1];
                j++;
            }
            (*envp)[j] = NULL;
            return ;
        }
        i++;
    }
}

int builtin_unset(t_cmd *cmd, t_exec *exec)
{
    int i;
    
    if (!cmd || !exec)
        return (1);
    if (!cmd->args[1])
        return (0);
    i = 1;
    while (cmd->args[i])
    {
        remove_env_entry(&exec->envp, cmd->args[i]);
        i++;
    }
    return (0);
}
