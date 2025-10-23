/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:05:43 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 19:07:06 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

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
	while (fill[index])
		process_clean_quotes(result, fill, &index, &j);
	result[j] = '\0';
	return (result);
}
