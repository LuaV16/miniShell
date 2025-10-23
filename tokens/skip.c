/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldiaz-u <aldiaz-u@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 19:11:01 by aldiaz-u          #+#    #+#             */
/*   Updated: 2025/10/23 19:11:58 by aldiaz-u         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../miniShell.h"

void	skip_normal_token(char *rl, int *index)
{
	char	quote;

	while (rl[*index] && !is_space(rl[*index]) && !is_special_char(rl[*index]))
	{
		if (rl[*index] == '"' || rl[*index] == '\'')
		{
			quote = rl[(*index)++];
			while (rl[*index] && rl[*index] != quote)
				(*index)++;
			if (rl[*index] == quote)
				(*index)++;
		}
		else
			(*index)++;
	}
}

void	skip_special_token(char *rl, int *index)
{
	while (rl[*index]
		&& is_special_char(rl[*index]) && rl[*index] == rl[*index + 1])
		(*index)++;
	(*index)++;
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
