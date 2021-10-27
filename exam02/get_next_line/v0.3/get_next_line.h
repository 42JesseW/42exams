#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 32
# endif
# define STATIC_BUFFSIZE 500000000

# include <stdlib.h>
# include <string.h>

char    *get_next_line(int fd);

#endif
