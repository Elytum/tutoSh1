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

static char		*get_env_name(const char *line)
{
	static char	buffer[128];
	char		*ptr;

	memcpy(buffer, line, sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';
	ptr = buffer;
	while (*ptr && *ptr != '=')
		++ptr;
	if (*ptr == '=')
		*ptr = '\0';
	return (buffer);
}

static int		env_name_match(char *cmp, char *tab)
{
	size_t		len;

	// dprintf(1, "\t\tTesting match with [%s] and [%s]\n", cmp, tab);
	len = strlen(cmp);
	return (!strncmp(cmp, tab, len) && tab[len] == '=');
}

static void		new_increased_env_table(t_env *env, char *line)
{
	char		**new_table;
	char		**new_ptr;
	char		**ptr;
	size_t		len;

	len = 0;
	ptr = env->env_table;
	while (*ptr)
	{
		++ptr;
		++len;
	}
	if (!(new_table = (char **)malloc(sizeof(char *) * (len + 2))))
		return ;
	new_ptr = new_table;
	ptr = env->env_table;
	while (*ptr)
		*new_ptr++ = *ptr++;
	*new_ptr++ = line;
	*new_ptr = NULL;
	free(env->env_table);
	env->env_table = new_table;
}

static void		do_setenv_insert(t_env *env, char *line)
{
	char		**ptr;

	ptr = env->env_table;
	while (*ptr)
	{
		if (env_name_match(get_env_name(*ptr), line))
		{
			free(*ptr);
			*ptr = line;
			return ;
		}
		++ptr;
	}
	new_increased_env_table(env, line);
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
	do_setenv_insert(env, env_element);
}

void			builtin_setenv(t_env *env)
{
	char		*ptr;
	char		*name;

	ptr = env->line + sizeof("setenv") - 1;
	name = NULL;
	while (*ptr == ' ' || *ptr == '\t')
		++ptr;
	if (!*ptr)
		builtin_env(env);
	else
	{
		name = ptr;
		while (*ptr && !(*ptr == ' ' | *ptr == '\t'))
			++ptr;
		while (*ptr == ' ' || *ptr == '\t')
			++ptr;
		if (!*ptr)
			do_setenv(env, name, "");
		else
			do_setenv(env, name, ptr);
	}
}

void			builtin_exit(t_env *env)
{
	int			ret;

	ret = atoi(env->line + sizeof("exit") - 1);
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

static char		**copy_string_array(char **array)
{
	char		**new_array;
	char		**new_ptr;
	char		**ptr;
	size_t		len;

	ptr = array;
	len = 0;
	while (*ptr)
	{
		++ptr;
		++len;
	}
	if (!(new_array = (char **)malloc(sizeof(char *) * (len + 1))))
		return (NULL);
	new_ptr = new_array;
	ptr = array;
	while (*ptr)
		*new_ptr++ = strdup(*ptr++);
	*new_ptr = NULL;
	return (new_array);
}

int				main(int argc, char **argv, char **env_table)
{
	t_env		*env;
	char		*line;

	if (!(env = (t_env *)malloc(sizeof(t_env))))
		return (-1);
	env->env_table = copy_string_array(env_table);
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
