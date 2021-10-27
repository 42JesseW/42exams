#include "get_next_line.h"

char	*ft_strcpy(char *dst, const char *src)
{
	int		idx;

	if (!dst || !src)
		return (NULL);
	idx = 0;
	while (src[idx])
	{
		dst[idx] = src[idx];
		idx++;
	}
	return (dst);
}

int		ft_strlen(char *str)
{
	int	len;
	int	idx;

	len = 0;
	idx = 0;
	while (str[idx])
	{
		len++;
		idx++;
	}
	return (len);
}

void	*ft_calloc(int count, int size)
{
	char	*ptr;
	int		len;
	int		idx;

	len = (count * size);
	ptr = (char*)malloc(sizeof(char) * len);
	if (!ptr)
		return (NULL);
	idx = 0;
	while (idx < len)
	{
		ptr[idx] = '\0';
		idx++;
	}
	return (ptr);
}

char	*ft_strdup(char *str)
{
	char	*dup;
	int		idx;

	if (!str)
		return (NULL);
	dup = (char*)malloc(strlen(str) * sizeof(char) + 1);
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

char	*ft_strndup(char *str, int n)
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

char	*ft_strchr(char *str, int c)
{
	int	idx;

	if (!str)
		return (NULL);
	idx = 0;
	while (str[idx])
	{
		if (str[idx] == c)
			return (str + idx);
		idx++;
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*s_linebuffer = NULL;
	char		*buffer;
	char		*tmp;
	char		*p;
	ssize_t		ret;

	if (s_linebuffer && (p = ft_strchr(s_linebuffer, '\n')))
	{
		/* if there is still a line in static buffer return this first */
		buffer = ft_strndup(s_linebuffer, (p - s_linebuffer) + 1);
		if (!buffer)
			return (NULL);
		tmp = ft_strdup(s_linebuffer + (p - s_linebuffer) + 1);
		if (!tmp)
		{
			free(buffer);
			return (NULL);
		}
		free(s_linebuffer);
		s_linebuffer = tmp;
		return (buffer);
	}

	/* allocate buffer of BUFFER_SIZE and read BUFFER_SIZE bytes into it */
	buffer = (char*)ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!buffer)
		return (NULL);
	ret = read(fd, buffer, BUFFER_SIZE);
	if (ret < 0 || (ret == 0 && !s_linebuffer))
	{
		free(buffer);
		return (NULL);
	}
	
	/* if no line found in buffer read into buffer untill found */
	if (!(p = ft_strchr(buffer, '\n')))
	{
		for (int count = 2; !(p = ft_strchr(buffer, '\n')) && ret != 0; count++)
		{
			tmp = (char*)ft_calloc((BUFFER_SIZE * count) + 1, sizeof(char));
			if (!tmp)
				return (NULL);
			ft_strcpy(tmp, buffer);
			free(buffer);
			buffer = tmp;
			ret = read(fd, buffer + (BUFFER_SIZE * (count - 1)), BUFFER_SIZE);
			if (ret < 0)
			{
				free(buffer);
				return (NULL);
			}
		}
	}

	/* if remainder in static buffer, add this to current buffer */
	if (s_linebuffer)
	{
		int	len = ft_strlen(s_linebuffer);
		tmp = (char*)ft_calloc(len + ft_strlen(buffer) + 1, sizeof(char));
		if (!tmp)
		{
			free(buffer);
			return (NULL);
		}
		ft_strcpy(tmp, s_linebuffer);
		ft_strcpy(tmp + len, buffer);
		free(s_linebuffer);
		free(buffer);
		buffer = tmp;
		s_linebuffer = NULL;
	}

	/* if remainder in current buffer, split this into static buffer */
	if ((p = ft_strchr(buffer, '\n')))
	{
		s_linebuffer = ft_strdup(buffer + (p - buffer) + 1);
		if (!s_linebuffer)
		{
			free(buffer);
			return (NULL);
		}
		tmp = ft_strndup(buffer, (p - buffer) + 1);
		free(buffer);
		buffer = tmp;
		if (!buffer)
		{
			free(s_linebuffer);
			return (NULL);
		}
	}
	return (buffer);
}
