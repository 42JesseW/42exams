#include "get_next_line.h"

#include <string.h>
#include <stdio.h>
#include <fcntl.h>

char    *get_next_line(int fd)
{
    static char *s_buffer = NULL;
    int         len;
    ssize_t     ret;
    char        *internal_buffer;
    char        *tmp;
    char        *p;
    int         buffer_size;
    int         read_count, alloc_count;

    /* check if still a line in buff */
    if (s_buffer && (p = strchr(s_buffer, '\n')))
    {
        internal_buffer = strndup(s_buffer, (p - s_buffer) + 1);
        tmp = strdup(s_buffer + (p - s_buffer) + 1);
        if (!tmp || !internal_buffer)
        {
            free(internal_buffer);
            free(tmp);
            return (NULL);
        }
        free(s_buffer);
        s_buffer = tmp;
        if (!(*s_buffer))
        {
            free(s_buffer);
            s_buffer = NULL;
        }
        return (internal_buffer);
    }

    buffer_size = BUFFER_SIZE;
    if (buffer_size < 1024)
        buffer_size = 1024;

    internal_buffer = (char*)malloc(sizeof(char) * (buffer_size + 1));
    ret = read(fd, internal_buffer, BUFFER_SIZE);
    if (ret < 0 || (ret == 0 && !s_buffer))
    {
        free(internal_buffer);
        return (NULL);
    }
    internal_buffer[ret] = '\0';

    /* read more bytes into buffer so as to find new line */
    if (ret != 0 && !(p = strchr(internal_buffer, '\n')))
    {
        read_count = 1;
        alloc_count = 0;
        while (!(p = strchr(internal_buffer, '\n')) && ret != 0)
        {
            /* TODO check if buffer needs to be increased */
            ret = read(fd, internal_buffer + (read_count * BUFFER_SIZE), BUFFER_SIZE);
            read_count++;
        }
        internal_buffer[((read_count - 1) * BUFFER_SIZE) + ret] = '\0';
    }

    /* if remainder in static buffer copy to internal buffer */
    if (s_buffer)
    {
        len = strlen(s_buffer);
        if (len > 0)
        {
            tmp = (char*)malloc(sizeof(char) * (len + strlen(internal_buffer) + 1));
            strcpy(tmp, s_buffer);
            strcpy(tmp + len, internal_buffer);
            tmp[len + strlen(internal_buffer)] = '\0';
            free(s_buffer);
            free(internal_buffer);
            internal_buffer = tmp;
            s_buffer = NULL;
        }
    }

    /* if remainer in internal buffer copy to static buffer */
    if ((p = strchr(internal_buffer, '\n')))
    {
        if ((long)strlen(internal_buffer) != (p - internal_buffer + 1))
        {
            tmp = strdup(internal_buffer + (p - internal_buffer) + 1);
            s_buffer = tmp;
            internal_buffer[p - internal_buffer + 1] = '\0';
        }
    }
    return (internal_buffer);
}

void    leakcheck(void)
{
    system("leaks a.out");
}

/*
for (( i=1; i < 100; i++ ));
do
    if ! diff <(gcc -D BUFFER_SIZE=$i -Wall -Werror -Wextra get_next_line.c && ./a.out) <(cat get_next_line.c);
    then
        echo $i;
    fi;
done
*/

int     main(void)
{
    char    *line;
    int     fd;

    //atexit(leakcheck);
    fd = open("get_next_line.c", O_RDONLY);
    if (fd < 0)
        return (EXIT_FAILURE);
    line = get_next_line(fd);
    while (line)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    return (EXIT_SUCCESS);
}
