#include "../miniShell.h"
void	null_content(char c, char quote)
{
	if (c == quote)
	{
		strerror(errno);
		printf("error contenido nulo");
		exit(1);
	}
}

void	close_quotes(char c)
{
	if (c == '\0')
	{
		strerror(errno);
		printf("error debes cerrar las comillas");
		exit(1);
	}
}