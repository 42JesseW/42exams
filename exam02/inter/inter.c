#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

/*
Write a program that takes two strings and displays, without doubles, the
characters that appear in both strings, in the order they appear in the first
one.

The display will be followed by a \n.

If the number of arguments is not 2, the program displays \n.
*/

int	main(int argc, char *argv[])
{
	bool	in_second;
	bool	already_printed;
	ssize_t	ret;

	if (argc == 3)
	{
		for (int i = 0; argv[1][i]; i++)
		{
			in_second = false;
			already_printed = false;
			for (int j = 0; argv[2][j]; j++)
			{
				if (argv[1][i] == argv[2][j])
				{
					in_second = true;
					/* check in first string if already printed */
					for (int z = i - 1; z >= 0; z--)
					{
						if (argv[1][i] == argv[1][z])
						{
							already_printed = true;
							break ;
						}
					}
					break ;
				}
			}
			if (in_second && !already_printed)
				ret = write(STDOUT_FILENO, argv[1] + i, 1);
		}
	}
	ret = write(STDOUT_FILENO, "\n", 1);
	return ((ret < 0) ? EXIT_FAILURE : EXIT_SUCCESS);
}
