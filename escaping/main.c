#include <shell.h>
#include <interprete.h>
#include <string.h>

# define STRING "  $lol ~ 'TEST_SIZE$lolt' "

t_env		*init_env(void)
{
	t_env	*env;

	if (!(env = (t_env *)malloc(sizeof(t_env))))
		return (ERROR);
	env->local_variables = ht_create( 65536 );
	memcpy(env->line, STRING, LINE_SIZE - 1);
	env->line[LINE_SIZE - 1] = '\0';
	env->len = strlen(env->line);

	memcpy(env->pwd, "/tmp/tutoSh1/escaping", sizeof(env->pwd) - 1);
	memcpy(env->home, "/Users/isabelle", sizeof(env->home) - 1);
	
	return (env);
}

void		add_local_variable(t_env *env, const char *key, const char *value)
{
	ht_set( env->local_variables, strdup(key), strdup(value));
}

int			main(void)
{
	t_env	*env;

	if ((env = init_env()) == ERROR)
		return (ERROR_EXIT);
	add_local_variable(env, "lol", "lilol");
	launch_interprete(env);
	return (NORMAL_EXIT);
}