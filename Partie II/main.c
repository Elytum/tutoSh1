#include <unistd.h>
#include <string.h>
#include <get_next_line.h>

#define PROMPT "?> "

int			main(void)
{
	char	*line;

	write(1, PROMPT, sizeof(PROMPT) - 1);
	while (get_next_line(1, &line) > 0)
	{
		write(1, line, strlen(line));
		write(1, "\n", 1);
		write(1, PROMPT, sizeof(PROMPT) - 1);
	}
	return (0);
}