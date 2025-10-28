/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:38:06 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/28 09:37:26 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

void	shift_tokens_left(char **tok, int start, int shift)
{
	int	i;

	i = start;
	while (tok[i])
	{
		tok[i] = tok[i + shift];
		i++;
	}
}

int	expand_dollar_next(t_pipe_ctx *ctx)
{
	char	*env;

	if (!ctx->tok[ctx->index + 1])
		return (0);
	env = getenv(ctx->tok[ctx->index + 1]);
	free(ctx->tok[ctx->index]);
	free(ctx->tok[ctx->index + 1]);
	if (env)
	{
		ctx->tok[ctx->index] = ft_strdup(env);
		shift_tokens_left(ctx->tok, ctx->index + 1, 1);
		return (1);
	}
	shift_tokens_left(ctx->tok, ctx->index, 2);
	return (0);
}

char	*find_exec_env(t_exec *exec, const char *name)
{
	int		index;
	char	*eq;
	int		name_len;

	if (!exec || !exec->envp || !name)
		return (NULL);
	name_len = ft_strlen(name);
	index = 0;
	while (exec -> envp[index])
	{
		if (ft_strncmp(exec->envp[index], name, name_len) == 0
			&& (exec -> envp[index][name_len] == '='
			|| exec -> envp[index][name_len] == '\0'))
		{
			eq = ft_strchr(exec -> envp[index], '=');
			if (eq)
				return (eq + 1);
			return (NULL);
		}
		index++;
	}
	return (NULL);
}
