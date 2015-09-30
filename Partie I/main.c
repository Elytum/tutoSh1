#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct	s_env {
char			**env_table;
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

	if (!(env = (t_env *)malloc(sizeof(t_env))))
		return (-1);
	env->env_table = copy_string_array(env_table);
	builtin_env(env);
	return (0);
}
