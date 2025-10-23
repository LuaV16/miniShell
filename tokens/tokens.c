/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:15:38 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 19:12:58 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

static void	add_word(char *rl, int *i, t_token_ctx *ctx)
{
	char	*tmp;

	tmp = extract_token(rl, i);
	ctx -> res[*(ctx -> j)] = remove_quotes(tmp);
	if (has_any_quote(tmp))
		ctx -> quote_type[*(ctx -> j)] = 1;
	else
		ctx -> quote_type[*(ctx -> j)] = 0;
	free(tmp);
	(*(ctx -> j))++;
}

void	init_token_ctx(t_token_ctx *token_ctx,
			char **res, int *j, int *quote_type)
{
	token_ctx -> res = res;
	token_ctx -> j = j;
	token_ctx ->quote_type = quote_type;
}

int	fill_result(char *rl, char **res, int *quote_type)
{
	int			i;
	int			j;
	t_fill_ctx	ctx;
	t_token_ctx	token_ctx;

	i = 0;
	j = 0;
	init_token_ctx(&token_ctx, res, &j, quote_type);
	while (rl[i])
	{
		while (is_space(rl[i]))
			i++;
		if (!rl[i])
			break ;
		ctx.rl = rl;
		ctx.i = &i;
		ctx.res = res;
		ctx.j = &j;
		ctx.quote_type = quote_type;
		if (!fill_quotes(&ctx) && !fill_special_chars(rl, &i, res, &j))
			add_word(rl, &i, &token_ctx);
	}
	res[j] = NULL;
	return (1);
}

char	**ft_token(char *rl, t_exec *exec)
{
	char	**split;
	int		cw;

	if (!rl || !*rl)
		return (NULL);
	cw = count_words(rl);
	split = (char **)malloc((cw + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	exec->quote_type = (int *)ft_calloc(sizeof(int), (cw + 1));
	if (!exec->quote_type)
	{
		free(split);
		return (NULL);
	}
	if (!fill_result(rl, split, exec->quote_type))
	{
		free_resources(split);
		free(exec->quote_type);
		return (NULL);
	}
	return (split);
}
