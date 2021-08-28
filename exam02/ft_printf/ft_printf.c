#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

typedef enum	e_conv
{
	NONE 		= 0,
	STRING 		= 's',
	DECIMAL 	= 'd',
	HEXA		= 'x'
}				t_conv;

typedef struct	s_convgroup
{
	t_conv		c;
	int			width;
	int			prec;
	char		*str;
	int			strlen;
	bool		is_signed;
}				t_convgroup;

int		ft_strlen(const char *str)
{
	int	idx;
	int	len;

	idx = 0;
	len = 0;
	while (str[idx])
	{
		len++;
		idx++;
	}
	return (len);
}

bool	is_conversion(int c)
{
	return (c == STRING || c == DECIMAL || c == HEXA);
}

char	*ft_strdup(const char *str)
{
	char	*dup;
	int		idx;

	if (!str)
		return (NULL);
	dup = (char*)malloc(ft_strlen(str) * sizeof(char) + 1);
	if (!dup)
		return (NULL);
	idx = 0;
	while (str[idx])
	{
		dup[idx] = str[idx];
		idx++;
	}
	dup[idx] = '\0';
	return (dup);
}

char	*ft_strndup(const char *str, int n)
{
	char	*dup;
	int		idx;

	if (!str || n < 0)
		return (NULL);
	dup = (char*)malloc(n * sizeof(char) + 1);
	if (!dup)
		return (NULL);
	idx = 0;
	while (str[idx] && idx < n)
	{
		dup[idx] = str[idx];
		idx++;
	}
	dup[idx] = '\0';
	return (dup);
}

bool	ft_isdigit(int c)
{
	return (c >= 48 && c <= 57);
}

int		ft_atoi(const char *str)
{
	int	sign;
	int	num;
	int	idx;

	idx = 0;
	sign = 1;
	if (str[idx] == '-')
	{
		sign = -1;
		idx++;
	}
	num = 0;
	while (ft_isdigit(str[idx]))
	{
		num = num * 10 + (str[idx] - '0');
		idx++;
	}
	return (num * sign);
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

int		count_digits(int num)
{
	int	count;

	count = 1;
	while (num > 9)
	{
		num /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int num)
{
	char	*str;
	int		sign;
	int		idx;

	sign = 0;
	if (num < 0 || num == -0)
	{
		sign = 1;
		num = -num;
	}
	str = (char*)malloc((count_digits(num) + sign) * sizeof(char) + 1);
	if (!str)
		return (NULL);
	idx = 0;
	if (num == 0)
	{
		str[idx] = '0';
		return (str);
	}
	while (num > 0)
	{
		str[idx] = num % 10 + '0';
		num /= 10;
		idx++;
	}
	if (sign)
	{
		str[idx] = '-';
		idx++;
	}
	str[idx] = '\0';
	return (ft_strrev(str));
}

char	*ft_itoa_u(unsigned int num)
{
	char	*str;
	int		sign;
	int		idx;

	sign = 0;
	if (num < 0 || num == -0)
	{
		sign = 1;
		num = -num;
	}
	str = (char*)malloc((count_digits(num) + sign) * sizeof(char) + 1);
	if (!str)
		return (NULL);
	idx = 0;
	if (num == 0)
	{
		str[idx] = '0';
		return (str);
	}
	while (num > 0)
	{
		str[idx] = num % 16 + ((num % 16 > 9) ? 39 : 0) + '0';
		num /= 16;
		idx++;
	}
	if (sign)
	{
		str[idx] = '-';
		idx++;
	}
	str[idx] = '\0';
	return (ft_strrev(str));
}

int		real_printf(const char *fmt, va_list args)
{
	t_convgroup	group;
	ssize_t		ret;
	int			idx;
	int			len;
	int			prec, width;

	idx = 0;
	len = 0;	/* keep track of total amount of bytes written to STDOUT */
	while (fmt[idx])
	{
		if (fmt[idx] == '%')
		{
			/* scan for flags */
			/* [WIDTH][PRECISION][CONVERSION] */
			int i = 1;
			group = (t_convgroup){NONE, -1, -1, NULL, 0, false};
			while (fmt[idx + i] && !is_conversion(fmt[idx + i]))
			{
				/* found width */
				if (ft_isdigit(fmt[idx + i]))
				{
					group.width = ft_atoi(fmt + idx + i);
					i += count_digits(group.width);
				}
				/* found precision */
				if (fmt[idx + i] == '.')
				{
					if (is_conversion(fmt[idx + i + 1]))
					{
						group.prec = 0;
						i++;
					}
					else
					{
						group.prec = ft_atoi(fmt + idx + i + 1);
						i += count_digits(group.prec) + 1;
					}
				}
			}
			idx += i;

			/* get conversion */
			group.c = fmt[idx];
			switch (group.c)
			{
				case STRING:
					group.str = va_arg(args, char *);
					if (!group.str && (group.prec == 0 || group.prec == -1 || group.prec >= 6))
						group.str = ft_strdup("(null)");
					else if (!group.str && (group.prec > 0 && group.prec < 6))
						group.str = ft_strdup("");
					else
						group.str = ft_strdup(group.str);
					if (!group.str)
						return (-1);
					group.strlen = ft_strlen(group.str);
					
					/*
					** precision cuts of the string if smaller then its length
					** width just prints empty spaces
					*/
					if (group.prec != -1 && group.prec < group.strlen)
					{
						char *tmp = ft_strndup(group.str, group.prec);
						free(group.str);
						group.str = tmp;
						if (!group.str)
							return (-1);
						group.strlen = ft_strlen(group.str);
					}
					
					/* print the width */
					width = (group.width > group.strlen) ? group.width - group.strlen : 0;
					for (int i = 0; i < width; i++)
					{
						ret = write(STDOUT_FILENO, " ", 1);
						len++;
					}

					/* print the conversion */
					ret = write(STDOUT_FILENO, group.str, group.strlen);
					len += group.strlen;
					break ;

				case DECIMAL:
					group.str = ft_itoa(va_arg(args, int));
					if (!group.str)
						return (-1);
					group.strlen = ft_strlen(group.str);
					if (*group.str == '-')
						group.is_signed = true;
					if (group.prec == 0 && *group.str == '0')
						group.strlen = 0;
					
					/*
					** precision prints '0' characters if bigger than string
					** width just prints empty spaces
					*/
					/* print the width */
					width = (group.width > group.strlen) ? group.width - group.strlen : 0;
					if (group.prec > group.strlen)
						width -= group.prec - group.strlen;
					for (int i = 0; i < width; i++)
					{
						ret = write(STDOUT_FILENO, " ", 1);
						len++;
					}

					/* print the precision */
					int strlen = (group.is_signed) ? group.strlen - 1 : group.strlen;
					prec = (group.prec > strlen) ? group.prec - strlen : 0;
					if (prec > 0)
					{
						if (group.is_signed)
						{
							ret = write(STDOUT_FILENO, "-", 1);
							len++;
						}
						for (int i = 0; i < prec; i++)
						{
							ret = write(STDOUT_FILENO, "0", 1);
							len++;
						}
					}

					/* print the conversion */
					if (!(group.prec == 0 && *group.str == '0'))
					{
						if (prec > 0 && group.is_signed)
						{
							ret = write(STDOUT_FILENO, group.str + 1, group.strlen - 1);
							len += group.strlen - 1;
						}
						else
						{
							ret = write(STDOUT_FILENO, group.str, group.strlen);
							len += ft_strlen(group.str);
						}
					}
					break ;

				case HEXA:
					group.str = ft_itoa_u(va_arg(args, unsigned int));
					if (!group.str)
						return (-1);
					group.strlen = ft_strlen(group.str);
					if (group.prec == 0 && *group.str == '0')
						group.strlen = 0;

					/*
					** precision prints '0' characters if bigger than string
					** width just prints empty spaces
					*/
					/* print the width */
					width = (group.width > group.strlen) ? group.width - group.strlen : 0;
					if (group.prec > group.strlen)
						width -= group.prec - group.strlen;
					for (int i = 0; i < width; i++)
					{
						ret = write(STDOUT_FILENO, " ", 1);
						len++;
					}

					/* print the precision */
					prec = (group.prec > group.strlen) ? group.prec - group.strlen : 0;
					for (int i = 0; i < prec; i++)
					{
						ret = write(STDOUT_FILENO, "0", 1);
						len++;
					}

					/* print the conversion */
					if (!(group.prec == 0 && *group.str == '0'))
					{
						ret = write(STDOUT_FILENO, group.str, group.strlen);
						len += group.strlen;
					}
					break ;

				default:
					break ;
			}
			idx++;

		}
		else
		{
			ret = write(STDOUT_FILENO, fmt + idx, 1);
			idx++;
			len++;
		}
	}
	return ((ret < 0) ? -1 : len);
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

#ifndef REAL
# define F	r += ft_printf
#else
# define F	r += printf
#endif

int	main(void)
{
	int	r;

	r = 0;
	F("Simple test\n");
	F("");
	F("--Format---");
	F("\n");
	F("%d", 0);
	F("%d", 42);
	F("%d", 1);
	F("%d", 5454);
	F("%d", (int)214748364);
	F("%d", (int)214748364);
	F("%d", (int)-214748364);
	F("%d", (int)-214748364);
	F("\n");
	F("%x", 0);
	F("%x", 42);
	F("%x", 1);
	F("%x", 5454);
	F("%x", (int)214748364);
	F("%x", (int)214748364);
	F("%x", (int)-214748364);
	F("%x", (int)-214748364);
	F("%s", "");
	F("%s", "toto");
	F("%s", "wiurwuyrhwrywuier");
	F("%s", NULL);
	F("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL);
	F("\n--Mixed---\n");
	F("%d%x%d%x%d%x%d%x\n", 0, 0, 42, 42, 214748364, 214748364, (int)-214748364, (int)-214748364);
	F("-%d-%x-%d-%x-%d-%x-%d-%x-\n", 0, 0, 42, 42, 214748364, 214748364, (int)-214748364, (int)-214748364);
	F("\n");
	F("%s%s%s%s\n", "", "toto", "wiurwuyrhwrywuier", NULL);
	F("-%s-%s-%s-%s-\n", "", "toto", "wiurwuyrhwrywuier", NULL);
	F("--1 Flag--\n");
	F("d0w %0d %0d %0d %0d %0d %0d %0d %0d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("d4w %4d %4d %4d %4d %4d %4d %4d %4d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("d10w %10d %10d %10d %10d %10d %10d %10d %10d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("d0p %.0d %.0d %.0d %.0d %.0d %.0d %.0d %.0d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("d4p %.4d %.4d %.4d %.4d %.4d %.4d %.4d %.4d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("d10p %.10d %.10d %.10d %.10d %.10d %.10d %.10d %.10d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x0w %0x %0x %0x %0x %0x %0x %0x %0x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x4w %4x %4x %4x %4x %4x %4x %4x %4x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x10w %10x %10x %10x %10x %10x %10x %10x %10x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x0p %.0x %.0x %.0x %.0x %.0x %.0x %.0x %.0x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x4p %.4x %.4x %.4x %.4x %.4x %.4x %.4x %.4x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x10p %.10x %.10x %.10x %.10x %.10x %.10x %.10x %.10x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("s0p ~%.0s` ~%.0s` ~%.0s` ~%.0s` ~%.0s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	F("s4w ~%4s` ~%4s` ~%4s` ~%4s` ~%4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	F("s4p ~%.4s` ~%.4s` ~%.4s` ~%.4s` ~%.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	F("s10w ~%10s` ~%10s` ~%10s` ~%10s` ~%10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	F("s10p ~%.10s` ~%.10s` ~%.10s` ~%.10s` ~%.6s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	F("--2 Flags--\n");
	F("d0w0p %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d %0.0d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x0w0p %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x %0.0x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("--Precision--\n");
	F("d0w4p %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d %0.4d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("d0w10p %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d %0.10d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x0w4p %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x %0.4x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x0w10p %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x %0.10x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("--Width--\n");
	F("d4w0p %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d %4.0d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("d10w0p %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d %10.0d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x4w0p %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x %4.0x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x10w0p %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x %10.0x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("s4w0p ~%4.s` ~%4.s` ~%4.s` ~%4.s` ~%4.s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	F("s10w0p ~%10.0s` ~%10.0s` ~%10.0s` ~%10.0s` ~%10.0s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	F("--Width and Precision--\n");
	F("d10w4p %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d %10.4d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("d10w10p %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d %10.10d\n", 0, 1, 42, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("d4w4p %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d %4.4d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("d4w10p %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d %4.10d\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x10w4p %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x %10.4x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x10w10p %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x %10.10x\n", 0, 1, 42, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x4w4p %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x %4.4x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("x4w10p %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x %4.10x\n", 0, 42, 1, 4554, 214748364, (int)214748364, (int)-214748364, (int)-214748364);
	F("s10w4p ~%10.4s` ~%10.4s` ~%10.4s` ~%10.4s` ~%10.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	F("s10w10p ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	F("s4w4p ~%4.4s` ~%4.4s` ~%4.4s` ~%4.4s` ~%4.4s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	F("s4w10p ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s` ~%10.10s`\n", "", "toto", "0123456789", "tjehurthteutuiehteute", NULL);
	printf("written: %d\n", r);
	return (0);
}