#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H
# define BUFF_SIZE 4
# define GNL_EXIT_SUCCESS 1
# define GNL_EXIT_FAILURE -1
# define GNL_EXIT_EOF 0

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

int		get_next_line(char **line);

#endif
