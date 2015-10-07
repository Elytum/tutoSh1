#include <shell.h>
#include <interprete.h>
#include <string.h>

#include <stdio.h>

// #define STRING "testing~ ~ \"begin ~lol end\" 'simple' \"double\""
// #define STRING "testing~ ~ \"begin ~$lol end\" 'simple' \"double\""
// #define STRING "ls \"~\" -la ./ / ~ $HOME $PWD"
// #define STRING "ls \"-la\""
// #define STRING "ls \"-la\" ; echo 'lol'"
#define STRING "\"$HOME\" \"$PATH\" \"$PWD\""
// #define STRING "lol$lol"
// #define STRING "ls -la ; echo \"$PATH\" & echo \"$PATH\" || ls && pwd"

t_env		*init_env(void)
{
	t_env	*env;

	if (!(env = (t_env *)debug_malloc(sizeof(t_env))))
		return (ERROR);
	env->local_variables = ht_create( 65536 );
	memcpy(env->line, STRING, _POSIX2_LINE_MAX);
	env->line[_POSIX2_LINE_MAX - 1] = '\0';
	env->len = sizeof(STRING) - 1;

	memcpy(env->pwd, "<PWD VALUE>", sizeof(env->pwd));
	memcpy(env->home, "<HOME VALUE>", sizeof(env->home));

	env->pwd_len = strlen(env->pwd);
	env->home_len = strlen(env->home);
	
	env->argv_pool_size = 0;
	return (env);
}

void		free_argv(t_env *env)
{
	size_t	id;

	id = 0;
	while (id < env->argv_pool_size)
	{
		printf("Free of [%s]\n", env->argv_pool[id]);
		free(env->argv_pool[id++]);
	}
	env->argv_pool_size = 0;
}

void		add_local_variable(t_env *env, const char *key, const char *value)
{
	ht_set(env->local_variables, strdup(key), strdup(value));
}

void		*debug_malloc(size_t size)
{
	printf("\t\t<MALLOC>\n\t\tRequested a malloc of %lu\n\n", size);
	return (malloc(size));
}

int			main(void)
{
	t_env	*env;

	if ((env = init_env()) == ERROR)
		return (ERROR_EXIT);
	add_local_variable(env, "HOME", "/nfs/zfs-student-3/users/2014/achazal");
	add_local_variable(env, "PWD", "/nfs/zfs-student-3/users/2014/achazal/tutoSh1/escaping");
	add_local_variable(env, "PATH", "/nfs/zfs-student-3/users/2014/achazal/.brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin:/usr/texbin");
	
	start_interprete(env);
	while (launch_interprete(env) == CONTINUE)
	{
		put_env(env);
		free_argv(env);
	}
	return (NORMAL_EXIT);
}