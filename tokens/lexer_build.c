/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:12:17 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 19:13:02 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

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
		if (quote == '"')
			c->quote_type[*(c->j)] = 2;
		else if (quote == '\'')
			c->quote_type[*(c->j)] = 1;
		else
			c->quote_type[*(c->j)] = 0;
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

char	*extract_token(char *rl, int *i)
{
	int		start;
	char	quote;

	start = *i;
	while (rl[*i] && !is_space(rl[*i]) && !is_special_char(rl[*i]))
	{
		if (rl[*i] == '"' || rl[*i] == '\'')
		{
			quote = rl[(*i)++];
			while (rl[*i] && rl[*i] != quote)
				(*i)++;
			if (rl[*i] == quote)
				(*i)++;
		}
		else
			(*i)++;
	}
	return (ft_substr(rl, start, *i - start));
}
