#include "get_next_line.h"
#include <stdio.h>

int		main(void)
{
	char	*line;
	int		idx;

	idx = 0;
	line = get_next_line(0);
	while (line != NULL)
	{
		printf("[%d]: %s\n", idx, line);
		free(line);
		line = get_next_line(0);
		idx++;
	}
	return (0);
}