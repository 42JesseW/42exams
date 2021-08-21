#include "get_next_line.h"

int		ft_strlen(const char *str)
{
	int	count;
	int	idx;

	if (!str || !(*str))
		return (0);
	idx = 0;
	count = 0;
	while (str[idx])
	{
		count++;
		idx++;
	}
	return (count);
}

char	*ft_strcpy(char *dst, const char *src)
{
	int	idx;

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

char	*ft_strchr(char *src, int c)
{
	int	idx;

	if (!src)
		return (NULL);
	idx = 0;
	while (src[idx])
	{
		if (src[idx] == c)
			return (src + idx);
		idx++;
	}
	return (NULL);
}

/*
** Read only form STDIN_FILENO
** 1. EOF -> CTRL + D
** 2. Read chunk with multiple new lines
** 3. Read chunk without any new lines
*/

int		get_next_line(char **line)
{
	static char	*s_linebuff = NULL;
	char		*buff;
	char		*tmp;
	char		*p;
	
	if (s_linebuff)
	{
		/* if line in buffer retrieve this first */
		if ((p = strchr(s_linebuff, '\n')))
		{
			/* create copy and assign to *line */
			*line = strndup(s_linebuff, (p - s_linebuff));

			/* reassign s_linebuff */
			free(s_linebuff);
			s_linebuff = strdup(s_linebuff + (p - s_linebuff) + 1);
			return ((!!*line && !!s_linebuff) ? GNL_EXIT_SUCCESS : GNL_EXIT_FAILURE);
		}
	}

	/* create new buffer based on BUFF_SIZE and read BUFF_SIZE bytes into it*/
	buff = (char*)calloc(BUFF_SIZE + 1, sizeof(char));
	if (!buff)
		return (GNL_EXIT_FAILURE);
	ssize_t	ret = read(STDIN_FILENO, buff, BUFF_SIZE);
	if (ret < 0)
		return (GNL_EXIT_FAILURE);
	
	if (!(p = strchr(buff, '\n')))
	{
		/* if no line found in buffer, keep reading into buff untill found */
		for (int count = 2; !(p = ft_strchr(buff, '\n')) && ret != 0; count++)
		{
			tmp = (char*)calloc((BUFF_SIZE * count) + 1, sizeof(char));
			if (!tmp)
				return (GNL_EXIT_FAILURE);
			strcpy(tmp, buff);
			free(buff);
			buff = tmp;
			ret = read(STDIN_FILENO, buff + (BUFF_SIZE * (count - 1)), BUFF_SIZE);
			if (ret < 0)
				return (GNL_EXIT_FAILURE);
		}
	}

	/* if remainder in s_linebuff, add to buff as well */
	if (s_linebuff)
	{
		int	len = strlen(s_linebuff);
		tmp = (char*)calloc(len + strlen(buff) + 1, sizeof(char));
		if (!tmp)
			return (GNL_EXIT_FAILURE);
		strcpy(tmp, s_linebuff);
		strcpy(tmp + len, buff);
		free(buff);
		free(s_linebuff);
		s_linebuff = NULL;
		buff = tmp;
	}

	/* if remainder in buff, split this using s_linebuff */
	if ((p = strchr(buff, '\n')))
	{
		tmp = strndup(buff, (p - buff));
		if (!tmp)
			return (GNL_EXIT_FAILURE);
		//printf("%li remain: %s\n", (p - buff), tmp);
		s_linebuff = strdup(buff + (p - buff) + 1);
		if (!s_linebuff)
			return (GNL_EXIT_FAILURE);
		free(buff);
		buff = tmp;
	}
	*line = buff;
	return ((ret > 0) ? GNL_EXIT_SUCCESS : GNL_EXIT_EOF);
}
