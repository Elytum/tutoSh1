#include <shell.h>
#include <interprete.h>
#include <string.h>

#include <stdio.h>

// #define STRING "testing~ ~ \"begin ~lol end\" 'simple' \"double\""
// #define STRING "exit \"-2000\" lol"
// #define STRING "testing~ ~ \"begin ~$lol end\" 'simple' \"double\""
// #define STRING "ls \"~\" -la ./ / ~ $HOME $PWD"
// #define STRING "ls \"-la\""
// #define STRING "ls \"-la\" ; echo 'lol'"
// #define STRING "$PWD ; \"lol\";"
// #define STRING "; $l"
// #define STRING "HOME$HOME"
#define STRING "ls -la ; echo \"$PATH\" & echo \"$PATH\" || ls && pwd"

// #define STRING "Line not closed with backslash \\"
// #define STRING "Line not closed with simple quotes \""
// #define STRING "Line not closed with double quotes '"
// #define STRING "Line not closed with back quotes `"

void		init_do_interprete_tab(t_env *env)
{
	void		(**do_interprete_tab)(struct s_env *env);
	unsigned int i;

	do_interprete_tab = env->do_interprete_tab;
	i = 0;
	while (i < sizeof(env->do_interprete_tab))
		do_interprete_tab[i++] = &interprete_normal;
	do_interprete_tab['\''] = &interprete_simple_quote;
	do_interprete_tab['\"'] = &interprete_double_quote;
	do_interprete_tab['\\'] = &interprete_backslash;
	do_interprete_tab['`'] = &interprete_back_quote;
	do_interprete_tab['$'] = &interprete_value;
	do_interprete_tab['~'] = &interprete_tilde;
	do_interprete_tab[' ' ] = &interprete_spacing;
	do_interprete_tab['\t'] = &interprete_spacing;
	do_interprete_tab['\0'] = &interprete_null;
	do_interprete_tab[';'] = &interprete_comma;
	do_interprete_tab['&'] = &interprete_background;
	do_interprete_tab['|'] =  &interprete_pipe;

}

void		init_interprete_value_stop(t_env *env)
{
	char		*interprete_value_stop;

	interprete_value_stop = env->interprete_value_stop;
	memset(interprete_value_stop, 1, sizeof(env->interprete_value_stop));
	interprete_value_stop['\''] = 0;
	interprete_value_stop['\"'] = 0;
	interprete_value_stop['\\'] = 0;
	interprete_value_stop['`'] = 0;
	interprete_value_stop['$'] = 0;
	interprete_value_stop[' '] = 0;
	interprete_value_stop['\t'] = 0;
	interprete_value_stop['\0'] = 0;
	interprete_value_stop[';'] = 0;
	interprete_value_stop['|'] = 0;
	interprete_value_stop['&'] = 0;
}

void		init_standard_delimiters(t_env *env)
{
	char	*standard_delimiters;

	standard_delimiters = env->standard_delimiters;
	memset(standard_delimiters, 0, sizeof(env->standard_delimiters));
	standard_delimiters[' '] = 1;
	standard_delimiters['\t'] = 1;
	standard_delimiters['\0'] = 1;
	standard_delimiters[';'] = 1;
	standard_delimiters['|'] = 1;
	standard_delimiters['&'] = 1;
}

void		init_spaces(t_env *env)
{
	char	*spaces;

	spaces = env->spaces;
	memset(spaces, 0, sizeof(env->spaces));
	spaces[' '] = 1;
	spaces['\t'] = 1;
	spaces['\0'] = 1;
}

void		init_should_len_tab(t_env *env)
{
	size_t		(**should_len_tab)(struct s_env *env, size_t *pos);

	should_len_tab = env->should_len_tab;
	should_len_tab[INTERPRETED] = &len_normal;
	should_len_tab[SIMPLE_QUOTED] = &len_simple_quote;
	should_len_tab[DOUBLE_QUOTED] = &len_double_quote;
	should_len_tab[BACK_QUOTED] = &len_back_quote;
	should_len_tab[BACKSLASHED] = &len_backslash;
	should_len_tab[START_LOCAL_VARIABLE] = &len_value;
}

void		init_extract_content_tab(t_env *env)
{
	void		(**extract_content_tab)(struct s_env *env, size_t *pos, char **ptr);

	extract_content_tab = env->extract_content_tab;
	extract_content_tab[INTERPRETED] = &extract_normal;
	extract_content_tab[SIMPLE_QUOTED] = &extract_simple_quote;
	extract_content_tab[DOUBLE_QUOTED] = &extract_double_quote;
	extract_content_tab[BACK_QUOTED] = &extract_back_quote;
	extract_content_tab[BACKSLASHED] = &extract_backslash;
	extract_content_tab[START_LOCAL_VARIABLE] = &extract_value;
}

t_env		*init_env(void)
{
	t_env	*env;

	if (!(env = (t_env *)debug_malloc(sizeof(t_env))))
		return (ERROR);
	env->local_variables = ht_create( 65536 );
	// memcpy(env->line, STRING, _POSIX2_LINE_MAX);
	// env->line[_POSIX2_LINE_MAX - 1] = '\0';
	// env->len = sizeof(STRING) - 1;

	memcpy(env->pwd, "<PWD VALUE>", sizeof(env->pwd));
	memcpy(env->home, "<HOME VALUE>", sizeof(env->home));

	init_do_interprete_tab(env);
	init_interprete_value_stop(env);
	init_standard_delimiters(env);
	init_spaces(env);
	init_should_len_tab(env);
	init_extract_content_tab(env);

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
		// printf("Free of [%s]\n", env->argv_pool[id]);
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
	// printf("\t\t<MALLOC>\n\t\tRequested a malloc of %lu\n\n", size);
	return (malloc(size));
}

#include <unistd.h>

int			main(void)
{
	t_env	*env;
	// char	**tab;

	if ((env = init_env()) == ERROR)
		return (ERROR_EXIT);
	add_local_variable(env, "HOME", "/nfs/zfs-student-3/users/2014/achazal");
	add_local_variable(env, "PWD", "/nfs/zfs-student-3/users/2014/achazal/tutoSh1/escaping");
	add_local_variable(env, "PATH", "/nfs/zfs-student-3/users/2014/achazal/.brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin:/usr/texbin");
	
	int i = 0;
	while (i++ < 1000000)
	{
		memcpy(env->line, STRING, _POSIX2_LINE_MAX);
		env->line[_POSIX2_LINE_MAX - 1] = '\0';
		env->len = sizeof(STRING) - 1;
		if (start_interprete(env) == NOT_CLOSED)
			write(1, "Line not closed\n", sizeof("Line not closed\n") - 1);
		else
		{
			while (launch_interprete(env) == CONTINUE)
			{
				free_argv(env);
				// tab = (char **)malloc(sizeof(char *) * 5);
				// tab[0] = strdup("testing~");
				// tab[1] = strdup("~");
				// tab[2] = strdup("\"begin ~$lol end\"");
				// tab[3] = strdup("'simple'");
				// tab[4] = strdup("\"double\"");
				// free(tab[0]);
				// free(tab[1]);
				// free(tab[2]);
				// free(tab[3]);
				// free(tab[4]);
				// free(tab);
			}
		}
	}
	// while (42)
	// 	;
	return (NORMAL_EXIT);
}