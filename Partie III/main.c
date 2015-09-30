#include <string.h>
#include <stdio.h>

char		*get_called_command(char *line)
{
	static char		buffer[256];
	char			*ptr;

	memcpy(buffer, line, sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';
	ptr = buffer;
	while (*ptr)
	{
		if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
		{
			*ptr = '\0';
			return (buffer);
		}
		++ptr;
	}
	return (buffer);
}

void		testing(char *line)
{
	printf("Line [%s] ask for command [%s].\n", line, get_called_command(line));
}

int			main(void)
{
	testing("env");
	testing("env lolilol");
	testing("crepes_whisky");
	return (0);
}
