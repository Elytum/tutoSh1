#include <interprete.h>
#include <string.h>

#include <stdio.h>

void		do_interprete(t_env *env)
{
	env->pos = 0;
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '\'')
			interprete_simple_quote(env);
		else if (env->buffer[env->pos] == '\"')
			interprete_double_quote(env);
		else if (env->buffer[env->pos] == '\\')
			interprete_backslash(env);
		else if (env->buffer[env->pos] == '`')
			interprete_back_quote(env);
		else if (env->buffer[env->pos] == '$')
			interprete_value(env);
		else if (env->buffer[env->pos] == '~')
			interprete_tilde(env);
		else if (env->buffer[env->pos] == ' ' || env->buffer[env->pos] == '\t')
			interprete_spacing(env);
		else if (env->buffer[env->pos] == '\0')
			interprete_null(env);
		else
			interprete_normal(env);
	}
}


void		do_process(t_env *env)
{
	process_back_quotes(env);
}

void		do_simplify(t_env *env)
{
	size_t			buffer_pos;
	const size_t	len = env->len;

	buffer_pos = 0;
	env->pos = 0;
	while (env->pos <= len)
	{
		if (env->interprete[env->pos] == REMOVE)
		{
			++env->pos;
			--env->len;
		}
		else
		{
			env->buffer[buffer_pos] = env->buffer[env->pos];
			env->interprete[buffer_pos] = env->interprete[env->pos];
			++buffer_pos;
			++env->pos;
		}
	}
	while (env->interprete[env->len - 1] == SPACING)
		--env->len;
}

int			set_arguments(t_env *env)
{
	set_argc(env);
	return (set_argv(env));
}

void		launch_interprete(t_env *env)
{
	memcpy(env->buffer, env->line, env->len + 1);
	memset(env->interprete, '\0', env->len + 1);

	do_interprete(env);
	if (env->interprete[env->len] != INTERPRETED && env->interprete[env->len] != SPACING)
	{
		printf("Line is not closed\n");
		debug_env(env);
	}
	else
	{
		do_simplify(env);
		do_process(env);
		debug_env(env);
		set_arguments(env);
	}
}
