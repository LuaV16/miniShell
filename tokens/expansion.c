/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:28:42 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/28 12:19:28 by aldiaz-u         ###   ########.fr       */
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
		return (handle_dollar_var_case(ctx, exec));
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

int	expand_dollar_var(t_pipe_ctx *ctx, t_exec *exec)
{
	char	*var;
	char	*env;
	char	*eq;
	char	*varname;

	var = ft_strdup(ctx->tok[ctx->index] + 1);
	if (!var)
		return (0);
	varname = get_var_name(var, &eq);
	env = find_exec_env(exec, varname);
	if (!env)
		env = getenv(varname);
	free(varname);
	free(ctx->tok[ctx->index]);
	if (env)
		ctx->tok[ctx->index] = ft_strdup(env);
	else if (eq)
		ctx->tok[ctx->index] = ft_strdup(eq);
	else
		ctx->tok[ctx->index] = ft_strdup("");
	free(var);
	return (1);
}

int	handle_dollar_only(t_pipe_ctx *ctx)
{
	int	had_value;

	had_value = expand_dollar_next(ctx);
	if (!had_value)
	{
		free(ctx->tok[ctx->index]);
		ctx->tok[ctx -> index] = ft_strdup("$");
		if (!ctx->tok[ctx->index])
			ctx->tok[ctx->index] = ft_strdup("");
		return (0);
	}
	return (0);
}

int	handle_dollar_var_case(t_pipe_ctx *ctx, t_exec *exec)
{
	int	had_value;

	had_value = expand_dollar_var(ctx, exec);
	if (!had_value)
		return (1);
	return (0);
}
