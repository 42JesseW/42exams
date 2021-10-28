#ifndef MICRO_PAINT_H

# define MICRO_PAINT_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <ctype.h>
# include <math.h>

typedef struct s_rectangle
{
	char				filled;
	float				x_tl;
	float				y_tl;
	float				x_br;
	float				y_br;
	float				width;
	float				height;
	char				back_char;
	struct s_rectangle	*next;
}	t_rectangle;

typedef struct s_config
{
	char		*drawing;
	int			width;
	int			height;
	char		back_char;
	t_rectangle	*rectangles;
}	t_config;

#endif