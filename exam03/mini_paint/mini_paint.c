#include "mini_paint.h"

void	circle_destroy_list(t_circle **root)
{
	t_circle	*node;
	t_circle	*tmp;

	node = *root;
	while (node)
	{
		tmp = node;
		node = node->next;
		free(tmp);
	}
	*root = NULL;
}

void	circle_add_back(t_circle **root, t_circle *circle)
{
	t_circle	*traverse_node;
	
	if (!root)
		return ;
	if (!(*root))
		*root = circle;
	else
	{
		traverse_node = *root;
		while (traverse_node)
		{
			if (!traverse_node->next)
			{
				traverse_node->next = circle;
				break ;
			}
			traverse_node = traverse_node->next;
		}
	}
}

void		circle_print_all(t_circle *root)
{
	t_circle	*n;

	n = root;
	while (n)
	{
		printf("circle: %c %f %f %f %c\n", n->filled, n->x, n->y, n->radius, n->back_char);
		n = n->next;
	}
}

void		config_print(t_config *config)
{
	printf("width: %d | height: %d | background_char: %c\n", config->width, config->height, config->back_char);
	circle_print_all(config->circles);
}

t_circle	*circle_new(void)
{
	t_circle	*circle;

	circle = (t_circle*)malloc(sizeof(t_circle));
	if (!circle)
		return (NULL);
	circle->next = NULL;
	return (circle);
}

bool in_range(int num, int min, int max)
{
	return ((num - min) * (num - max) <= 0);
}

int parse_operation_file(FILE *file, t_config *config)
{
	t_circle	*circle;
	int			items_matched;

	while (true)
	{
		circle = circle_new();
		if (!circle)
		{
			circle_destroy_list(&config->circles);
			return (0);
		}
		items_matched = fscanf(file, "%c %f %f %f %c\n",
			&circle->filled,
			&circle->x,
			&circle->y,
			&circle->radius,
			&circle->back_char
		);
		if (items_matched == EOF)
		{
			free(circle);
			break ;
		}
		// TODO edge case "c 15.5 12.5 10.2 O" same as "c 15.5 12.5 10.2O"
		if (!(circle->radius > 0 && (circle->filled == 'c' || circle->filled == 'C')))
		{
			circle_destroy_list(&config->circles);
			return (0);
		}
		circle_add_back(&config->circles, circle);
	}
	return (fclose(file) == 0);
}

void	fill_drawing(t_config *config)
{
	t_circle	*circle;
	float		distance;
	char		ch;
	int			y;
	int			x;

	y = 0;
	while (y < config->height)
	{
		x = 0;
		while (x < config->width)
		{
			ch = config->back_char;
			circle = config->circles;
			while (circle)
			{
				distance = sqrtf(powf(x - circle->x, 2.) + powf(y - circle->y, 2.));
				if (distance <= circle->radius)
				{
					if (circle->filled == 'C' || (circle->filled == 'c' && (circle->radius - distance) > 0 && (circle->radius - distance) < 1))
						ch = circle->back_char;
				}
				circle = circle->next;
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

int	init_config(FILE *file, t_config *config)
{
	fscanf(file, "%d %d %c\n", &config->width, &config->height, &config->back_char);
	if (!in_range(config->width, 0, 300) || !in_range(config->width, 0, 300))
		return (0);
	config->circles = NULL;
	config->drawing = (char*)malloc(sizeof(char) * (config->width * config->height) + 1);
	if (!config->drawing)
		return (0);
	config->drawing[config->width * config->height] = 0;
	return (1);
}

int exit_failure(const char *msg, int size)
{
	write(STDOUT_FILENO, msg, size);
	return (EXIT_FAILURE);
}

/*
**	RULES
**	1. Circles will draw over each other in order given by the operations file
**	2. Distance between two points (Xa, Ya) and (Xb, Yb) is sqrt((Xa - Xb)^2 + (Ya - Yb)^2)
*/

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
