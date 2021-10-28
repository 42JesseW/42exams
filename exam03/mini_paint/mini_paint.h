#ifndef MINI_PAINT_H

# define MINI_PAINT_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <ctype.h>
# include <math.h>

typedef struct s_circle
{
	char			filled;
	float			x;
	float			y;
	float			radius;
	char			back_char;
	struct s_circle	*next;
}	t_circle;

typedef struct s_config
{
    char        *drawing;
	int			width;
	int			height;
	char		back_char;
	t_circle	*circles;
}	t_config;

#endif