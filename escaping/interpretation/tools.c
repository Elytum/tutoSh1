#include <interprete.h>
#include <string.h>

#include <stdio.h>

void		do_interprete(t_env *env)
{
	env->pos = env->start;
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
		else if (env->buffer[env->pos] == ';')
			interprete_comma(env);
		else if (env->buffer[env->pos] == '&')
			interprete_background(env);
		else if (env->buffer[env->pos] == '|')
			interprete_pipe(env);
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
	env->pos = env->start;
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

char		start_interprete(t_env *env)
{
	if (env->len == 0)
		return (STOP);
	env->start = 0;
	memcpy(env->buffer, env->line, env->len + 1);
	memset(env->interprete, '\0', env->len + 1);

	do_interprete(env);
	if (env->interprete[env->len] != INTERPRETED && env->interprete[env->len] != SPACING)
		return (NOT_CLOSED);
	do_simplify(env);
	do_process(env);
	return (CONTINUE);
}

char		launch_interprete(t_env *env)
{
	set_arguments(env);
	if (env->len < env->start -1)
		return (STOP);
	return (CONTINUE);
}
