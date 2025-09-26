/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:15:38 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/09/26 16:10:56 by aldiaz-u         ###   ########.fr       */
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

int	fill_quotes(char *rl, int *i, char **res, int *j, int *quote_type)
{
	char	quote;
	int		start;

	if (rl[*i] == '"' || rl[*i] == '\'')
	{
		quote = rl[(*i)++];
		start = *i;
		while (rl[*i] && rl[*i] != quote)
			(*i)++;
		res[*j] = ft_substr(rl, start, *i - start);
		if (quote == '"')
			quote_type[*j] = 2;
		else
			quote_type[*j] = 1;
		(*j)++;
		if (rl[*i] == quote)
			(*i)++;
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

int	fill_result(char *rl, char **res, int *quote_type)
{
	int	i;
	int	j;
	int	start;

	i = 0;
	j = 0;
	while (rl[i])
	{
		while (is_space(rl[i]))
			i++;
		if (!rl[i])
			break ;
		if (!fill_quotes(rl, &i, res, &j, quote_type) && !fill_special_chars(rl, &i, res, &j))
		{
			start = i;
			while (rl[i] && !is_space(rl[i]) && rl[i] != '"' && rl[i] != '\''
				&& !is_special_char(rl[i]))
				i++;
			res[j++] = ft_substr(rl, start, i - start);
		}
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
	exec -> quote_type = (int*)ft_calloc(sizeof(int), (cw + 1));
	if (!exec->quote_type)
	{
		free(split);
		return (NULL);
	}
	if (!fill_result(rl, split, exec -> quote_type))
	{
		free_resources(split);
		free(exec -> quote_type);
		return (NULL);
	}
	return (split);
}
