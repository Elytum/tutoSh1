#include <shell.h>
#include <interprete.h>
#include <string.h>

# define STRING "  $lol 'TEST_SIZE$lolt' "

t_env		*init_env(void)
{
	t_env	*env;

	if (!(env = (t_env *)malloc(sizeof(t_env))))
		return (ERROR);
	env->local_variables = ht_create( 65536 );
	memcpy(env->line, STRING, LINE_SIZE - 1);
	env->line[LINE_SIZE - 1] = '\0';
	env->len = strlen(env->line);
	return (env);
}

int			main(void)
{
	t_env	*env;

	if ((env = init_env()) == ERROR)
		return (-1);
	launch_interprete(env);
	return (0);
}