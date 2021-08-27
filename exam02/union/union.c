#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

/*
Write a program that takes two strings and displays, without doubles, the
characters that appear in either one of the strings.

The display will be in the order characters appear in the command line, and
will be followed by a \n.

If the number of arguments is not 2, the program displays \n.
*/

int	main(int argc, char *argv[])
{
	bool	already_printed;
	ssize_t	ret;

	if (argc == 3)
	{
		/* loop first string */
		for (int i = 0; argv[1][i]; i++)
		{
			already_printed = false;
			/* check if not already printed */
			if (i > 0)
			{
				for (int j = i - 1; argv[1][j]; j--)
				{
					if (argv[1][i] == argv[1][j])
					{
						already_printed = true;
						break ;
					}
				}
			}
			if (!already_printed)
				ret = write(STDOUT_FILENO, argv[1] + i, 1);
		}

		/* loop second string */
		for (int i = 0; argv[2][i]; i++)
		{
			already_printed = false;
			/* check if not already printed in first string*/
			for (int j = 0; argv[1][j]; j++)
			{
				if (argv[2][i] == argv[1][j])
				{
					already_printed = true;
					break ;
				}
			}

			if (!already_printed && i > 0)
			{
				/* check in current string if not already printed */
				for (int j = i - 1; j >= 0; j--)
				{
					if (argv[2][i] == argv[2][j])
					{
						already_printed = true;
						break ;
					}
				}
			}

			if (!already_printed)
				ret = write(STDOUT_FILENO, argv[2] + i, 1);
		}
	}
	ret = write(STDOUT_FILENO, "\n", 1);
	return ((ret < 0) ? EXIT_FAILURE : EXIT_SUCCESS);
}
