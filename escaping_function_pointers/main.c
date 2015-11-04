#include <shell.h>
#include <interprete.h>
#include <string.h>

#include <stdio.h>

// #define STRING "testing~ ~ \"begin ~ end\" 'simple' \"double\""
// #define STRING "exit \"-2000\" lol"
// #define STRING "testing~ ~ \"begin ~ end\" 'simple' \"double\""
// #define STRING "ls \"~\" -la ./ / ~ $HOME $PWD"
#define STRING "ls \"-la\""
// #define STRING "ls \"-la\" ; echo 'lol'"
// #define STRING "$PWD ; \"lol\";"
// #define STRING "; $l"
// #define STRING "HOME$HOME"
// #define STRING "ls -la ; echo \"$PATH\" & echo \"$PATH\" || ls && pwd"

// #define STRING "Line not closed with backslash \\"
// #define STRING "Line not closed with simple quotes \""
// #define STRING "Line not closed with double quotes '"
// #define STRING "Line not closed with back quotes `"

void		init_do_interprete_tab(t_env *env)
{
	unsigned int i;

	i = 0;
	while (i < sizeof(env->do_interprete_tab))
		env->do_interprete_tab[i++] = &interprete_normal;
	env->do_interprete_tab['\''] = &interprete_simple_quote;
	env->do_interprete_tab['\"'] = &interprete_double_quote;
	env->do_interprete_tab['\\'] = &interprete_backslash;
	env->do_interprete_tab['`'] = &interprete_back_quote;
	env->do_interprete_tab['$'] = &interprete_value;
	env->do_interprete_tab['~'] = &interprete_tilde;
	env->do_interprete_tab[' ' ] = &interprete_spacing;
	env->do_interprete_tab['\t'] = &interprete_spacing;
	env->do_interprete_tab['\0'] = &interprete_null;
	env->do_interprete_tab[';'] = &interprete_comma;
	env->do_interprete_tab['&'] = &interprete_background;
	env->do_interprete_tab['|'] =  &interprete_pipe;

}

void		init_interprete_value_stop(t_env *env)
{
	memset(env->interprete_value_stop, 1, sizeof(env->interprete_value_stop));
	env->interprete_value_stop['\''] = 0;
	env->interprete_value_stop['\"'] = 0;
	env->interprete_value_stop['\\'] = 0;
	env->interprete_value_stop['`'] = 0;
	env->interprete_value_stop['$'] = 0;
	env->interprete_value_stop[' '] = 0;
	env->interprete_value_stop['\t'] = 0;
	env->interprete_value_stop['\0'] = 0;
	env->interprete_value_stop[';'] = 0;
	env->interprete_value_stop['|'] = 0;
	env->interprete_value_stop['&'] = 0;
}

void		init_standard_delimiters(t_env *env)
{
	memset(env->standard_delimiters, 0, sizeof(env->standard_delimiters));
	env->standard_delimiters[' '] = 1;
	env->standard_delimiters['\t'] = 1;
	env->standard_delimiters['\0'] = 1;
	env->standard_delimiters[';'] = 1;
	env->standard_delimiters['|'] = 1;
	env->standard_delimiters['&'] = 1;
}

void		init_spaces(t_env *env)
{
	memset(env->spaces, 0, sizeof(env->spaces));
	env->spaces[' '] = 1;
	env->spaces['\t'] = 1;
	env->spaces['\0'] = 1;
}

void		init_should_len_tab(t_env *env)
{
	env->should_len_tab[INTERPRETED] = &len_normal;
	env->should_len_tab[SIMPLE_QUOTED] = &len_simple_quote;
	env->should_len_tab[DOUBLE_QUOTED] = &len_double_quote;
	env->should_len_tab[BACK_QUOTED] = &len_back_quote;
	env->should_len_tab[BACKSLASHED] = &len_backslash;
	env->should_len_tab[START_LOCAL_VARIABLE] = &len_value;
}

void		init_extract_content_tab(t_env *env)
{
	env->extract_content_tab[INTERPRETED] = &extract_normal;
	env->extract_content_tab[SIMPLE_QUOTED] = &extract_simple_quote;
	env->extract_content_tab[DOUBLE_QUOTED] = &extract_double_quote;
	env->extract_content_tab[BACK_QUOTED] = &extract_back_quote;
	env->extract_content_tab[BACKSLASHED] = &extract_backslash;
	env->extract_content_tab[START_LOCAL_VARIABLE] = &extract_value;
}

void		init_interprete_double_quote_tab(t_env *env)
{
	memset(env->interprete_double_quote_tab, 0, sizeof(env->interprete_double_quote_tab));
	env->interprete_double_quote_tab['$'] = 1;
	env->interprete_double_quote_tab['`'] = 1;
	env->interprete_double_quote_tab['"'] = 1;
	env->interprete_double_quote_tab['\n'] = 1;
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

int			get_line(char *buffer, size_t size)
{
	return (read(1, buffer, size));
}

void		subshell(t_env *env, ssize_t *size)
{
	ssize_t	tmp;

	env->multiline = 1;
	if (env->last_char == SIMPLE_QUOTED)
		write(1, "quote> ", 7);
	else if (env->last_char == DOUBLE_QUOTED)
		write(1, "dquote> ", 8);
	else if (env->last_char == BACK_QUOTED)
		write(1, "bquote> ", 8);
	else if (env->last_char == BACKSLASHED)
		write(1, "> ", 2);
	else
	{
		write(1, "UNKNOWN ERROR WTF\n", 18);
		exit (ERROR_EXIT);
	}
	if ((tmp = get_line(env->line + *size, sizeof(env->line) - *size)) < 0)
	{
		write(1, "Read error\n", 11);
		exit (ERROR_EXIT);
	}
	*size += tmp;
	env->len = *size;
}

int			main(void)
{
	t_env	*env;
	ssize_t	size;

	if ((env = init_env()) == ERROR)
		return (ERROR_EXIT);

	env->start = 0;

	add_local_variable(env, "HOME", "/nfs/zfs-student-3/users/2014/achazal");
	add_local_variable(env, "PWD", "/nfs/zfs-student-3/users/2014/achazal/tutoSh1/escaping");
	add_local_variable(env, "PATH", "/nfs/zfs-student-3/users/2014/achazal/.brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin:/usr/texbin");

	while ((size = get_line(env->line, sizeof(env->line))) > 0)
	{
		env->line[sizeof(env->line) - 1] = '\0';
		// /*TO BE MAYBE REMOVED*/ env->len = size;
		/*TO BE MAYBE REMOVED*/env->len = size - 1;
		env->multiline = 0;
		while (start_interprete(env) == NOT_CLOSED)
			subshell(env, &size);
		if (env->multiline)
			--env->len;
		while (launch_interprete(env) == CONTINUE)
		{
			put_env(env);
			free_argv(env);
		}
	}
	return (NORMAL_EXIT);
}