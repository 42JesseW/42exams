#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4
# endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char	*get_next_line(int fd);
int		ft_strlen(char *str);

#endif