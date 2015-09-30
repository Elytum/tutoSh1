#include <get_next_line.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

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

static void		do_setenv(t_env *env, const char *name, const char *value)
{
	char		*env_element;
	size_t		len_name;
	size_t		len_value;
	const char	*ptr;

	ptr = name;
	while (*ptr && !(*ptr == ' ' || *ptr == '\t'))
		++ptr;
	len_name = ptr - name;
	ptr = value;
	while (*ptr && !(*ptr == ' ' || *ptr == '\t'))
		++ptr;
	len_value = ptr - value;
	if (!(env_element = (char *)malloc(sizeof(char) * (len_name + len_value + 2))))
		return ;
	memcpy(env_element, name, len_name);
	env_element[len_name] = '=';
	memcpy(env_element + len_name + 1, value, len_value);
	env_element[len_name + 1 + len_value] = '\0';
	
	free(env_element);
}

void			builtin_setenv(t_env *env)
{
	char		*ptr;
	char		*name;

	ptr = env->line + sizeof("setenv");
	name = NULL;
	while (*ptr == ' ' || *ptr == '\t')
		++ptr;
	if (!*ptr)
		builtin_env(env);
	else
	{
		dprintf(1, "Else [%s]\n", ptr);
		name = ptr;
		while (*ptr && !(*ptr == ' ' | *ptr == '\t'))
			++ptr;
		while (*ptr == ' ' || *ptr == '\t')
			++ptr;
		dprintf(1, "Done [%s]\n", ptr);
		if (!*ptr)
			do_setenv(env, name, "");
		else
			do_setenv(env, name, ptr);
	}
	// {
	// 	name = ptr;
	// 	while (*ptr && !(*ptr == ' ' | *ptr == '\t'))
	// 		++ptr;
	// 	while (*ptr == ' ' || *ptr == '\t')
	// 	++ptr;
	// 	if (!*ptr)
	// 		do_setenv(env, name, "");
	// 	else
	// 		do_setenv(env, name, ptr);
	// }
}

void			builtin_exit(t_env *env)
{
	int			ret;

	ret = atoi(env->line + sizeof("exit"));
	free(env);
	exit (ret);
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

int				main(int argc, char **argv, char **env_table)
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
			else if (!strcmp(env->command, "setenv"))
				builtin_setenv(env);
			else if (!strcmp(env->command, "exit"))
				builtin_exit(env);
			else
				unknown_command(env->command);
		}
		write(1, PROMPT, sizeof(PROMPT) - 1);
	}
	return (0);
}
