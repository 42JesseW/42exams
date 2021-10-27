#include "get_next_line.h"

char    *get_next_line(int fd)
{
    static char buff[STATIC_BUFFSIZE];
    char        *p;

    /* check if still a line in buff */
    if ((p = strchr(buff, '\n')))
    {
        
    }
    return (NULL);
}

int     main(void)
{
    get_next_line(0);
    return (0);
}