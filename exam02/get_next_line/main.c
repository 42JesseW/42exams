#include "get_next_line.h"

int		main(void)
{
	char	*line;
	
	line = get_next_line(0);
	while (line != NULL)
	{
		write(STDOUT_FILENO, line, ft_strlen(line));
		free(line);
		line = get_next_line(0);
	}
	return (0);
}