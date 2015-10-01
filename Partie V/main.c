#include <get_next_line.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

#define PROMPT "?> "
#define SHELL_NAME "ft_sh1"
#define COMMAND_NOT_FOUND SHELL_NAME": command not found: "
#define ENV_TABLE_SIZE 256
#define ENV_TABLE_CONTENT_SIZE 64
#define MAX_COMMAND_LEN 64

typedef struct	s_env {
char			env_table[ENV_TABLE_SIZE][ENV_TABLE_CONTENT_SIZE];
char			*line;
char			*command;
char			flag;
}				t_env;

void			builtin_env(t_env *env)
{
	// size_t		element;

	// element = 0;
	// while (element < ENV_TABLE_SIZE && env->env_table[element][0])
	// {
	// 	write(1, env->env_table[element], strlen(env->env_table[element]));
	// 	write(1, "\n", 1);
	// 	++element;
	// }
	write(1, env->env_table, ENV_TABLE_SIZE * ENV_TABLE_CONTENT_SIZE);
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

	len = strlen(cmp);
	return (!strncmp(cmp, tab, len) && tab[len] == '=');
}

static void		do_setenv_insert(t_env *env, char buffer[ENV_TABLE_CONTENT_SIZE])
{
	const char	error[] = {"setenv: Too many environment variables\n"};
	size_t		element;

	element = 0;
	while (element < ENV_TABLE_SIZE && env->env_table[element][0])
	{
		if (env_name_match(get_env_name(env->env_table[element]), buffer))
		{
			memcpy(env->env_table[element], buffer, ENV_TABLE_CONTENT_SIZE);
			return ;
		}
		++element;
	}
	if (element < ENV_TABLE_SIZE)
	{
		memcpy(env->env_table[element], buffer, ENV_TABLE_CONTENT_SIZE);
		env->env_table[element + 1][0] = '\0';
	}
	else
		write(2, error, sizeof(error) - 1);
}

void			do_setenv_too_long(void)
{
	const char	error[] = {"setenv: Name/value length combination too long\n"};

	write(2, error, sizeof(error) - 1);
}

size_t			word_len(const char *sentence)
{
	const char	*ptr;

	ptr = sentence;
	while (*ptr && !(*ptr == ' ' || *ptr == '\t' || *ptr == '\n'))
		++ptr;
	return (ptr - sentence);
}

static void		do_setenv(t_env *env, const char *name, const char *value)
{
	char		buffer[ENV_TABLE_CONTENT_SIZE];
	size_t		size_left;
	size_t		len_name;
	size_t		len_value;
	const char	*ptr;

	env->flag = 0;
	len_name = word_len(name);
	size_left = sizeof(buffer);
	if (size_left >= len_name)
	{
		memcpy(buffer, name, len_name);
		size_left -= len_name;
		if (size_left > 1)
		{
			buffer[len_name] = '=';
			len_value =  word_len(value);
			if (--size_left >= len_value)
			{
				memcpy(buffer + len_name + 1, value, len_value);
				buffer[len_name + 1 + len_value] = '\0';
				env->flag = 1;
			}
		}
	}
	(env->flag) ? do_setenv_insert(env, buffer) : do_setenv_too_long();
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

// void			do_unsetenv(t_env *env, char *line)
// {
// 	char		**ptr;

// 	ptr = env->env_table;
// 	while (*ptr)
// 	{
// 		if (!strcmp(get_env_name(*ptr), line))
// 		{
// 			free(ptr[0]);
// 			ptr[0] = ptr[1];
// 			while (ptr[1])
// 			{
// 				ptr[0] = ptr[1];
// 				++ptr;
// 			}
// 			ptr[0] = NULL;
// 			return ;
// 		}
// 		++ptr;
// 	}
// }

// void			builtin_unsetenv(t_env *env)
// {
// 	char		*ptr;
// 	char		*name;
// 	const char	error[] = "Wrong format for unsetenv, should be \"unsetenv VARIABLE\"\n";

// 	ptr = env->line + sizeof("unsetenv") - 1;
// 	name = NULL;
// 	while (*ptr == ' ' || *ptr == '\t')
// 		++ptr;
// 	if (!*ptr)
// 		write(1, error, sizeof(error) - 1);
// 	else
// 		do_unsetenv(env, ptr);
// }

void			builtin_exit(t_env *env)
{
	int			ret;

	ret = atoi(env->line + sizeof("exit") - 1);
	free(env);
	exit (ret);
}

char			*get_called_command(char *line)
{
	static char		buffer[MAX_COMMAND_LEN];
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

void			init_env(t_env *env, char **env_table)
{
	size_t		element;
	char		**ptr;

	element = 0;
	ptr = env_table;
	while (element < ENV_TABLE_SIZE && *ptr)
	{
		strncpy(env->env_table[element], *ptr, sizeof(env->env_table[element]) - 1);
		env->env_table[element][sizeof(env->env_table[element]) - 1] = '\n';
		++element;
		++ptr;
	}
	if (element < ENV_TABLE_SIZE)
		env->env_table[element][0] = '\0';
}

int				main(int argc, char **argv, char **env_table)
{
	t_env		*env;
	char		*line;

	if (!(env = (t_env *)malloc(sizeof(t_env))))
		return (-1);
	init_env(env, env_table);
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
			// else if (!strcmp(env->command, "unsetenv"))
			// 	builtin_unsetenv(env);
			else if (!strcmp(env->command, "exit"))
				builtin_exit(env);
			else
				unknown_command(env->command);
		}
		write(1, PROMPT, sizeof(PROMPT) - 1);
	}
	return (0);
}
