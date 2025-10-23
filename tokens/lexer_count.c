/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_count.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:07:21 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 19:10:30 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

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
		if (is_special_char(rl[index]))
		{
			skip_special_token(rl, &index);
			count++;
		}
		else
		{
			skip_normal_token(rl, &index);
			count++;
		}
	}
	return (count);
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
