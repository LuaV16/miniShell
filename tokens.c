#include "miniShell.h"

int	count_words(char *rl)
{
	int index;
	int	count;
	char	quote;

	index = 0;
	count = 1;
	if (!rl)
		return (0);
	while (rl[index])
	{
		if (rl[index] == ' ' && rl[index + 1] != ' ' && rl[index  + 1] != '\0')
			count++;
		if (rl[index] == '"' || rl[index] == '\'')
		{
			quote = rl[index];
			index++;
			while (rl[index] && rl[index] != quote)
			{
				if (rl[index] == ' ')
					index++;
				index++;
			}
		}
		index++;
	}
	return (count);
}
int	fill_result(char *rl, char **res)
{
	int	i;
	int	j;
	int	start;
	char	quote;

	i = 0;
	j = 0;
	while(rl[i])
	{
		while (rl[i] == ' ')
			i++;
		if (!rl[i])
			break ;
		if (rl[i] == '"' || rl[i] == '\'')
		{
			quote = rl[i++];
			start = i;
			while (rl[i] && rl[i] != quote)
				i++;
			res[j++] = ft_substr(rl, start, i - start);
			if (rl[i] == quote)
				i++;
		}
		else
		{
			start = i;
			while (rl[i] && rl[i] != ' ' && rl[i] != '"' && rl[i] != '\'')
				i++;
			res[j++] = ft_substr(rl, start, i - start);
		}
	}
	res[j] = NULL;
	return (1);
}

char **ft_token(char *rl)
{
	char **split;

	split = (char**)malloc((count_words(rl) + 1) * sizeof(char*));
	if (!split)
		return (NULL);
	if (!fill_result(rl, split))
		return(NULL);
	return (split);
}