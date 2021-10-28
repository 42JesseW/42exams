#include "micro_paint.h"
#include <stdio.h>

bool in_range(int num, int min, int max)
{
	return ((num - min) * (num - max) <= 0);
}


void		rectangle_print_all(t_rectangle *root)
{
	t_rectangle	*n;

	n = root;
	while (n)
	{
		printf("rectangle: %c %f %f %f %f %f %f %c\n", n->filled, n->x_tl, n->y_tl, n->x_br, n->y_br, n->width, n->height, n->back_char);
		n = n->next;
	}
}

void		config_print(t_config *config)
{
	printf("width: %d | height: %d | background_char: %c\n", config->width, config->height, config->back_char);
	rectangle_print_all(config->rectangles);
}

void	rectangle_destroy_list(t_rectangle **root)
{
	t_rectangle	*node;
	t_rectangle	*tmp;

	node = *root;
	while (node)
	{
		tmp = node;
		node = node->next;
		free(tmp);
	}
	*root = NULL;
}

void		rectangle_add_back(t_rectangle **root, t_rectangle *rectangle)
{
	t_rectangle	*traverse_node;
	
	if (!root)
		return ;
	if (!(*root))
		*root = rectangle;
	else
	{
		traverse_node = *root;
		while (traverse_node)
		{
			if (!traverse_node->next)
			{
				traverse_node->next = rectangle;
				break ;
			}
			traverse_node = traverse_node->next;
		}
	}
}

t_rectangle	*rectangle_new(void)
{
	t_rectangle	*rectangle;

	rectangle = (t_rectangle*)malloc(sizeof(t_rectangle));
	if (!rectangle)
		return (NULL);
	rectangle->next = NULL;
	return (rectangle);
}

int parse_operation_file(FILE *file, t_config *config)
{
	t_rectangle	*rectangle;
	int			items_matched;

	while (true)
	{
		rectangle = rectangle_new();
		if (!rectangle)
		{
			rectangle_destroy_list(&config->rectangles);
			return (0);
		}
		items_matched = fscanf(file, "%c %f %f %f %f %c\n",
			&rectangle->filled,
			&rectangle->x_tl,
			&rectangle->y_tl,
			&rectangle->width,
			&rectangle->height,
			&rectangle->back_char
		);
		if (items_matched == EOF)
		{
			free(rectangle);
			break ;
		}
		// TODO edge case "c 15.5 12.5 10.2 O" same as "c 15.5 12.5 10.2O"
		if (!(rectangle->width > 0 && rectangle->height > 0 && (rectangle->filled == 'r' || rectangle->filled == 'R')))
		{
			rectangle_destroy_list(&config->rectangles);
			return (0);
		}
		rectangle->y_br = rectangle->y_tl + rectangle->height;
		rectangle->x_br = rectangle->x_tl + rectangle->width;
		rectangle_add_back(&config->rectangles, rectangle);
	}
	return (fclose(file) == 0);
}

int	init_config(FILE *file, t_config *config)
{
	fscanf(file, "%d %d %c\n", &config->width, &config->height, &config->back_char);
	if (!in_range(config->width, 0, 300) || !in_range(config->width, 0, 300))
		return (0);
	config->rectangles = NULL;
	config->drawing = (char*)malloc(sizeof(char) * (config->width * config->height) + 1);
	if (!config->drawing)
		return (0);
	config->drawing[config->width * config->height] = 0;
	return (1);
}

float	distance(float Xa, float Ya, float Xb, float Yb)
{
	return (sqrtf(powf(Xa - Xb, 2) + powf(Ya - Yb, 2)));
}

/*
** If a point is defined as (Xa, Ya)
** and a rectangle with a top left
** corner (Xtl, Ytl) and a bottom
** right corner (Xbr, Ybr).
**
** If Xtl <= Xa < Xbr && Xtl <= Ya <= Ybr
** then the point is in the rectangle.
*/
void	fill_drawing(t_config *config)
{
	t_rectangle	*rectangle;
	char		ch;
	int			x;
	int			y;

	y = 0;
	while (y < config->height)
	{
		x = 0;
		while (x < config->width)
		{
			ch = config->back_char;
			rectangle = config->rectangles;
			while (rectangle)
			{
				if ((x >= rectangle->x_tl && x <= rectangle->x_br)
					&& (y >= rectangle->y_tl && y <= rectangle->y_br))
				{
					// if distance to one of the edges is smaller then 1
					if (rectangle->filled == 'R' || (rectangle->filled == 'r'
						&& (distance(x, y, rectangle->x_tl, y) < 1.000000
							|| distance(x, y, x, rectangle->y_tl) < 1.000000
							|| distance(x, y, rectangle->x_br, y) < 1.000000
							|| distance(x, y, x, rectangle->y_br) < 1.000000)))
						ch = rectangle->back_char;
				}
			
				rectangle = rectangle->next;
			}
			config->drawing[x + (config->width * y)] = ch;
			x++;
		}
		y++;
	}
}

int	write_drawing(t_config *config)
{
	int	y;

	y = 0;
	while (y < config->height)
	{
		if (write(STDOUT_FILENO, config->drawing + (y * config->width), config->width) < 0)
			return (0);
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			return (0);
		y++;
	}
	return (1);
}

int exit_failure(const char *msg, int size)
{
	write(STDOUT_FILENO, msg, size);
	return (EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	t_config	config;
	FILE		*file;

	if (argc != 2)
		return (exit_failure("Error: arguments\n", 18));
	file = fopen(argv[1], "r");
	if (!file || !init_config(file, &config) || !parse_operation_file(file, &config))
		return (exit_failure("Error: Operation file corrupted\n", 33));
	fill_drawing(&config);
	if (!write_drawing(&config))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}