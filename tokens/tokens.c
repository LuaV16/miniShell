/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 10:15:38 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/09/15 10:46:00 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

int	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	return (0);
}

void	quotes_count(char *rl, int *index, char *quote)
{
	if (rl[*index] == '"' || rl[*index] == '\'')
	{
		*quote = rl[*index];
		null_content(rl[*index + 1], *quote);
		close_quotes(rl[*index + 1]);
		(*index)++;
		while (rl[*index] && rl[*index] != *quote)
		{
			if (is_space(rl[*index]))
				(*index)++;
			close_quotes(rl[*index + 1]);
			(*index)++;
		}
	}
}

int	count_words(char *rl)
{
	int		index;
	int		count;
	char	quote;

	index = 0;
	count = 0;
	if (!rl)
		return (0);
	while (rl[index])
	{
		if (is_space(rl[index]) && !(is_space(rl[index + 1])
				&& rl[index + 1] != '\0'))
			count++;
		quotes_count(rl, &index, &quote);
		index++;
	}
	return (count + 1);
}

int	fill_quotes(char *rl, int *i, char **res, int *j)
{
	char	quote;
	int		start;

	if (rl[*i] == '"' || rl[*i] == '\'')
	{
		quote = rl[(*i)++];
		start = *i;
		while (rl[*i] && rl[*i] != quote)
			(*i)++;
		res[(*j)++] = ft_substr(rl, start, *i - start);
		if (rl[*i] == quote)
			(*i)++;
		return (1);
	}
	return (0);
}

int	fill_result(char *rl, char **res)
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
		if (!fill_quotes(rl, &i, res, &j))
		{
			start = i;
			while (rl[i] && !is_space(rl[i]) && rl[i] != '"' && rl[i] != '\'')
				i++;
			res[j++] = ft_substr(rl, start, i - start);
		}
	}
	res[j] = NULL;
	return (1);
}

char	**ft_token(char *rl)
{
	char	**split;

	split = (char **)malloc((count_words(rl) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	if (!fill_result(rl, split))
		return (NULL);
	return (split);
}
