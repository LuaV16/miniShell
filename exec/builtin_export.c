/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvargas- <lvargas-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 19:34:52 by lvargas-          #+#    #+#             */
/*   Updated: 2025/10/09 20:33:30 by lvargas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

void get_name_and_entry(char *arg, char **name, char **entry)
{
    char *eq;
    
    if (!arg || !name || !entry)
        return ;
    *name = NULL;
    *entry = NULL;
    eq = ft_strchr(arg, '=');
    if (eq)
    {
        *name = ft_substr(arg, 0, (int)(eq - arg));
        *entry = ft_strdup(arg);
    }
    else
    {
        *name = ft_strdup(arg);
        *entry = malloc(ft_strlen(*name) + 2);
        if (*entry)
        {
            ft_strlcpy(*entry, *name, ft_strlen(*name) + 2);
            ft_strlcat(*entry, "=", ft_strlen(*name) + 2);
        }
    }
}

void check_identifier_error(t_cmd *cmd, t_exec *exec)
{
    int i;
    char	*arg;
    char	*name;
    char	*entry;

    i = 1;
    while (cmd->args[i])
    {
        arg = cmd->args[i];
        get_name_and_entry(arg, &name, &entry);
        if (!check_valid_identifier(name))
        {
            printf("export: `%s': not a valid identifier\n", arg);
            free(name);
            free(entry);
            exec->exit = 1;
            i++;
            continue ;
        }
        add_or_replace_envp(exec, entry);
        free(name);
        free(entry);
        i++;
    }
}

int builtin_export(t_cmd *cmd, t_exec *exec)
{
    int		i;

    if (!cmd || !exec)
        return (1);
    if (!cmd->args[1])
    {
        if (!exec->envp)
            return (0);
        i = 0;
        while (exec->envp[i])
            printf("declare -x %s\n", exec->envp[i++]);
        return (0);
    }
    check_identifier_error(cmd, exec);
    return (0);
}
