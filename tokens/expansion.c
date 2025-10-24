/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:28:42 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/24 12:04:04 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	handle_dolar(t_pipe_ctx *ctx, t_exec *exec)
{
	if (!ctx->tok[ctx->index])
		return (0);
	if (exec->quote_type[ctx->index] == 1)
		return (0);
	if (ctx->tok[ctx->index][0] == '$' && ctx->tok[ctx->index][1] == '?')
	{
		replace_exit_code(ctx->tok, ctx->index, exec);
		return (0);
	}
	if (ft_strncmp(ctx->tok[ctx->index],
			"$", ft_strlen(ctx->tok[ctx->index])) == 0)
		return (handle_dollar_only(ctx));
	if (ctx->tok[ctx->index][0] == '$')
		return (handle_dollar_var_case(ctx));
	return (0);
}

int	replace_exit_code(char **tokenized, int index, t_exec *exec)
{
	char	*exit_s;

	exit_s = ft_itoa(exec->exit);
	if (!exit_s)
		return (0);
	free(tokenized[index]);
	tokenized[index] = exit_s;
	return (1);
}

int	expand_dollar_var(t_pipe_ctx *ctx)
{
	char	*var;
	char	*env;
	int		i;

	var = malloc(ft_strlen(ctx->tok[ctx->index]));
	if (!var)
		return (0);
	ft_strlcpy(var, ctx->tok[ctx->index] + 1, ft_strlen(ctx->tok[ctx->index]));
	env = getenv(var);
	free(ctx->tok[ctx->index]);
	if (env)
	{
		ctx->tok[ctx->index] = ft_strdup(env);
		free(var);
		return (1);
	}
	i = ctx->index;
	while (ctx->tok[i])
	{
		ctx->tok[i] = ctx->tok[i + 1];
		i++;
	}
	free(var);
	return (0);
}

int	handle_dollar_only(t_pipe_ctx *ctx)
{
	int	had_value;

	had_value = expand_dollar_next(ctx);
	if (!had_value)
		return (1);
	return (0);
}

int	handle_dollar_var_case(t_pipe_ctx *ctx)
{
	int	had_value;

	had_value = expand_dollar_var(ctx);
	if (!had_value)
		return (1);
	return (0);
}
