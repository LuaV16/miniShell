/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:15:38 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/08 13:22:00 by aldiaz-u         ###   ########.fr       */
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

void	process_quote_token(char *rl, int *index, int *has_quotes)
{
	char	quote;

	while (rl[*index] && (rl[*index] == '"' || rl[*index] == '\''
			|| (!is_space(rl[*index]) && !is_special_char(rl[*index]))))
	{
		if (rl[*index] == '"' || rl[*index] == '\'')
		{
			quote = rl[(*index)++];
			*has_quotes = 1;
			null_content(rl[*index], quote);
			close_quotes(rl[*index]);
			while (rl[*index] && rl[*index] != quote)
				(*index)++;
			if (rl[*index] == quote)
				(*index)++;
		}
		else
		{
			while (rl[*index] && !is_space(rl[*index])
				&& rl[*index] != '"'
				&& rl[*index] != '\'' && !is_special_char(rl[*index]))
				(*index)++;
		}
	}
}

int	quotes_count(char *rl, int *index, int *count)
{
	int		has_quotes;

	has_quotes = 0;
	process_quote_token(rl, index, &has_quotes);
	if (has_quotes)
	{
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

int	mix_quotes(char *fill)
{
	int		index;
	char	quote;
	char	other_quote;

	index = 0;
	while (fill[index])
	{
		quote = fill[index];
		if (quote == '"')
			other_quote = '\'';
		else
			other_quote = '"';
		while (fill[index] && fill[index] != quote)
		{
			if (fill[index] == other_quote)
				return (1);
			index++;
		}
		if (fill[index] == quote)
			index++;
		else
			index++;
	}
	return (0);
}

void	process_clean_quotes(char *result, char *fill, int *index, int *j)
{
	int		in_quotes;
	char	quote;

	in_quotes = 0;
	quote = 0;
	if ((fill[*index] == '"' || fill[*index] == '\'') && !in_quotes)
	{
		quote = fill[*index];
		in_quotes = 1;
		(*index)++;
	}
	else if (fill[*index] == quote && in_quotes)
	{
		in_quotes = 0;
		quote = 0;
		(*index)++;
	}
	else
	{
		result[*j] = fill[*index];
		(*index)++;
		(*j)++;
	}
}

char	*remove_quotes(char *fill)
{
	int		index;
	char	*result;
	int		j;

	if (mix_quotes(fill))
		return (ft_strdup(fill));
	result = (char *)malloc((ft_strlen(fill) + 1) * sizeof(char));
	index = 0;
	j = 0;
	while (fill[index] && fill[index + 1])
		process_clean_quotes(result, fill, &index, &j);
	result[j] = '\0';
	return (result);
}

void	process_quote_segment(t_fill_ctx *c, char *quote, int *has_quote)
{
	while (c->rl[*(c->i)]
		&& (c->rl[*(c->i)] == '"' || c->rl[*(c->i)] == '\''
			|| (!is_space(c->rl[*(c->i)])
				&& !is_special_char(c->rl[*(c->i)]) && *has_quote)))
	{
		if (c->rl[*(c->i)] == '"' || c->rl[*(c->i)] == '\'')
		{
			*quote = c->rl[(*(c->i))++];
			*has_quote = 1;
			while (c->rl[*(c->i)] && c->rl[*(c->i)] != *quote)
				(*(c->i))++;
			if (c->rl[*(c->i)] == *quote)
				(*(c->i))++;
		}
		else
		{
			while (c->rl[*(c->i)]
				&& !is_space(c->rl[*(c->i)]) && c->rl[*(c->i)] != '"'
				&& c->rl[*(c->i)] != '\'' && !is_special_char(c->rl[*(c->i)]))
				(*(c->i))++;
		}
	}
}

int	fill_quotes(t_fill_ctx *ctx)
{
	t_fill_ctx	*c;
	char		quote;
	int			start;
	int			has_quotes;
	char		*fill;

	has_quotes = 0;
	c = ctx;
	start = *(c->i);
	process_quote_segment(c, &quote, &has_quotes);
	if (has_quotes)
	{
		fill = ft_substr(c->rl, start, *(c->i) - start);
		c -> res[*(c->j)] = remove_quotes(fill);
		free(fill);
		c->quote_type[*(c->j)] = 2;
		(*(c->j))++;
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
