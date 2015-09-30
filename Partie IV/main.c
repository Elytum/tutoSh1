#include <get_next_line.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PROMPT "?> "
#define SHELL_NAME "ft_sh1"
#define COMMAND_NOT_FOUND SHELL_NAME": command not found: "

typedef struct	s_env {
char			**env_table;
char			*line;
char			*command;
}				t_env;

void			builtin_env(t_env *env)
{
	char		**ptr;

	ptr = env->env_table;
	while (*ptr)
	{
		write(1, *ptr, strlen(*ptr));
		write(1, "\n", 1);
		++ptr;
	}
}

char			*get_called_command(char *line)
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

void			unknown_command(char *command)
{
	write(1, COMMAND_NOT_FOUND, sizeof(COMMAND_NOT_FOUND) - 1);
	write(1, command, strlen(command));
	write(1, "\n", 1);
}

int			main(int argc, char **argv, char **env_table)
{
	t_env		*env;
	char		*line;

	if (!(env = (t_env *)malloc(sizeof(t_env))))
		return (-1);
	env->env_table = env_table;
	write(1, PROMPT, sizeof(PROMPT) - 1);
	while (get_next_line(1, &env->line) > 0)
	{
		if (*env->line)
		{
			env->command = get_called_command(env->line);
			if (!strcmp(env->command, "env"))
				builtin_env(env);
			else
				unknown_command(env->command);
		}
		write(1, PROMPT, sizeof(PROMPT) - 1);
	}
	return (0);
}
