/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:15:38 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/09/26 16:23:53 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

int	is_special_char(char c)
{
	if (c == '>' || c == '<' || c == '|')
		return (1);
	return (0);
}

int	quotes_count(char *rl, int *index, int *count)
{
	char	quote;

	if (rl[*index] == '"' || rl[*index] == '\'')
	{
		quote = rl[(*index)++];
		null_content(rl[*index], quote);
		close_quotes(rl[*index]);
		while (rl[*index] && rl[*index] != quote)
			(*index)++;
		if (rl[*index] == quote)
			(*index)++;
		(*count)++;
		return (1);
	}
	return (0);
}

int	special_char_counts(char *rl, int *index, int *count)
{
	if (is_special_char(rl[*index]))
	{
		while (rl[*index] && is_special_char(rl[*index])
			&& rl[*index] == rl[(*index) + 1])
			(*index)++;
		(*index)++;
		(*count)++;
		return (1);
	}
	return (0);
}

int	count_words(char *rl)
{
	int	index;
	int	count;

	index = 0;
	count = 0;
	while (rl && rl[index])
	{
		while (rl[index] && is_space(rl[index]))
			index++;
		if (!rl[index])
			break ;
		if (!quotes_count(rl, &index, &count) && !special_char_counts(rl,
				&index, &count))
		{
			while (rl[index] && !is_space(rl[index]) && rl[index] != '"'
				&& rl[index] != '\'' && !is_special_char(rl[index]))
				index++;
			count++;
		}
	}
	return (count);
}

int	fill_quotes(t_fill_ctx *ctx)
{
	t_fill_ctx	*c;
	char		quote;
	int			start;

	c = ctx;
	if (c->rl[*(c->i)] == '"' || c->rl[*(c->i)] == '\'')
	{
		quote = c->rl[(*(c->i))++];
		start = *(c->i);
		while (c->rl[*(c->i)] && c->rl[*(c->i)] != quote)
			(*(c->i))++;
		c->res[*(c->j)] = ft_substr(c->rl, start, *(c->i) - start);
		if (quote == '"')
			c->quote_type[*(c->j)] = 2;
		else
			c->quote_type[*(c->j)] = 1;
		(*(c->j))++;
		if (c->rl[*(c->i)] == quote)
			(*(c->i))++;
		return (1);
	}
	return (0);
}

int	fill_special_chars(char *rl, int *i, char **res, int *j)
{
	int	start;

	if (is_special_char(rl[*i]))
	{
		start = *i;
		while (rl[*i] && is_special_char(rl[*i]) && rl[*i] == rl[(*i) + 1])
			(*i)++;
		(*i)++;
		res[(*j)++] = ft_substr(rl, start, *i - start);
		return (1);
	}
	return (0);
}

static void	add_word(char *rl, int *i, char **res, int *j)
{
	int	start;

	start = *i;
	while (rl[*i] && !is_space(rl[*i]) && rl[*i] != '"' && rl[*i] != '\''
		&& !is_special_char(rl[*i]))
		(*i)++;
	res[(*j)++] = ft_substr(rl, start, *i - start);
}

int	fill_result(char *rl, char **res, int *quote_type)
{
	int			i;
	int			j;
	t_fill_ctx	ctx;

	i = 0;
	j = 0;
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
			add_word(rl, &i, res, &j);
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
