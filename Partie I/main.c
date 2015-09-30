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

int				main(int argc, char **argv, char **env_table)
{
	t_env		*env;

	if (!(env = (t_env *)malloc(sizeof(t_env))))
		return (-1);
	env->env_table = env_table;
	builtin_env(env);
}

