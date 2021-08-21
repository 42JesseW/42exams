#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

typedef enum	e_conv
{
	NONE = 0,
	STRING = 's',
	NUMBER = 'd',
	HEXA = 'x'
}				t_conv;

typedef struct	s_convgroup
{
	t_conv		conversion;
	char		*str;
	int			str_len;
	int			precision;
	int			min_width;
	int			total_length;
}				t_convgroup;

int		count_digits(int num)
{
	int	count;

	if (num < 0)
		num = (num == INT_MIN) ? INT_MAX : -num;
	count = 1;
	while (num > 9)
	{
		num /= 10;
		count++;
	}
	return (count);
}

void	*ft_calloc(int amount, int size)
{
	char	*mem;
	int		len;
	int		idx;

	len = (amount * size);
	mem = (char*)malloc(len + 1);
	if (!mem)
		return (NULL);
	idx = 0;
	while (idx < len)
	{
		mem[idx] = '\0';
		idx++;
	}
	return (mem);
}

int		ft_strlen(char *str)
{
	int		idx;
	int		count;

	idx = 0;
	count = 0;
	while (str[idx])
	{
		count++;
		idx++;
	}
	return (count);
}

char	*ft_strrev(char *str)
{
	char	temp;
	int		i;
	int		j;

	i = 0;
	j = ft_strlen(str) - 1;
	while (i < j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
	return (str);
}

char	*ft_itoa(int n, int base)
{
	char	*a_str;
	int		idx;
	int		sign;

	sign = 0;
	if (n < 0 || n == -0)
	{
		sign = 1;
		n = -n;
	}
	a_str = (char*)ft_calloc((count_digits(n) + sign + 1), sizeof(char));
	if (!a_str)
		return (NULL);
	idx = 0;
	if (n == 0)
	{
		a_str[idx] = '0';
		return (a_str);
	}
	else
	{
		while (n > 0)
		{
			a_str[idx] = n % base + ((n % base) ? ('0' + 39) : '0');
			n /= 10;
			idx++;
		}
		if (sign)
			a_str[idx] = '-';
	}
	return (ft_strrev(a_str));
}

int		ft_atoi(const char *str)
{
	int	idx;
	int	num;
	int	sign;

	idx = 0;
	sign = 1;
	if (str[idx] == '-')
	{
		sign = -1;
		idx++;
	}
	num = 0;
	while (isdigit(str[idx]))
	{
		num = num * 10 + (str[idx] - '0');
		idx++;
	}
	return (num * sign);
}

bool	is_conversion(int c)
{
	return (c == STRING || c == NUMBER || c == HEXA);
}		

int		real_printf(const char *fmt, va_list args)
{
	int			len;
	int			idx;
	int			i;
	int			d;
	ssize_t		ret;
	t_convgroup	group;

	idx = 0;
	while (fmt[idx])
	{
		if (fmt[idx] == '%')
		{
			i = 1;
			group = (t_convgroup){NONE, NULL, 0, -1, -1, 0};
			/* scan the following string for flags and update index appropriately */
			while (fmt[idx + i] && !is_conversion(fmt[idx + i]))
			{
				/* found minimal width */
				if (isdigit(fmt[idx + i]))
				{
					group.min_width = ft_atoi(fmt + i);
					i += (count_digits(group.min_width) - 1);
				}
				/* found precision */
				if (fmt[idx + i] == '.')
				{
					/* if no precision digit but direct conv */
					if (is_conversion(fmt[idx + i + 1]))
					{
						group.conversion = fmt[idx + i + 1];
						group.precision = 0;
					}
					else
					{
						group.precision = ft_atoi(fmt + idx + i + 1);
						i += (count_digits(group.precision) - 1);
					}
				}
				i++;
			}

			if (group.conversion == NONE)
				group.conversion = fmt[idx + i + 1];

			/* determine conversion type and get corresponding string */
			switch (group.conversion)
			{
				case STRING:
					group.str = va_arg(args, char *);
					break ;
				
				case NUMBER:
					d = va_arg(args, int);
					group.str = ft_itoa(d, 10);
					break ;

				case HEXA:
					d = va_arg(args, int);
					group.str = ft_itoa(d, 16);
					break ;
			}

			if ((group.conversion == HEXA || group.conversion == NUMBER) && !group.str)
			{
				// error 
			}

			group.str_len = ft_strlen(group.str);
			group.total_length = (group.min_width > group.str_len) ? group.min_width : group.str_len;
			if (group.conversion == STRING && group.precision < group.str_len)
				group.str_len = group.precision;
			if (group.conversion == NUMBER || group.conversion == HEXA)
			{
				if ((group.str_len + group.precision) > group.total_length)
					group.total_length = group.str_len + group.precision;
			}

			int i = 0;
			for (int idx = 0; idx < group.total_length; idx++)
			{
				if (group.conversion == STRING)
				{
					if (idx < (group.total_length - group.str_len))
						ret = write(STDOUT_FILENO, " ", 1);
					else
					{
						ret = write(STDOUT_FILENO, group.str + i, 1);
						i++;
					}
				}
				else
				{
					if (idx < (group.total_length - group.str_len))
						ret = write(STDOUT_FILENO, " ", 1);
					else if (group.precision > group.str_len)
					{
						
					}
				}
			}
		}
		else
		{
			ret = write(STDOUT_FILENO, fmt + idx, 1);
			len++;
		}
		idx++;
	}
	return ((ret > 0) ? len : -1);
}

int		ft_printf(const char *fmt, ...)
{
	va_list	args;
	int		len;

	va_start(args, fmt);
	len = real_printf(fmt, args);
	va_end(args);
	return (len);
}

int		main(void)
{
	printf("number: %3.4x\n", 123);
	printf("number: %30.4x\n", 123);
	printf("number: %30.4d\n", 123);
	printf("string: %30.10s\n", "tjehurthteutuiehteute");
	return (0);
}
