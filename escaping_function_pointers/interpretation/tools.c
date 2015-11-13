#include <interprete.h>
#include <string.h>

#include <unistd.h>

#include <stdio.h>

void		do_interprete(t_env *env)
{
	env->pos = env->start;
	while (env->pos <= env->len)
		env->do_interprete_tab[(int)env->buffer[env->pos]](env);
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

void		handle_alias(t_env *env)
{
	const char	error[] = "Line too long to handle your alias\n";
	char	*ptr;
	char	*alias;
	char	tmp_char;
	size_t	len;

	ptr = env->line;
	while (*ptr && (*ptr != ' ' && *ptr != '\t'))
		++ptr;
	tmp_char = *ptr;
	*ptr = '\0';
	if ((alias = ht_get(env->alias, env->line)))
	{
		len = strlen(alias);
		if (len + strlen(alias) > sizeof(env->buffer) - 1)
			write(1, error, sizeof(error) - 1);
		else
		{
			memcpy(env->buffer, alias, len);
			memcpy(env->buffer + len, env->line, env->len + 1);
			memset(env->interprete, '\0', env->len + len + 1);
			*ptr = tmp_char;
			return ;
		}
	}
	*ptr = tmp_char;
	memcpy(env->buffer, env->line, env->len + 1);
	memset(env->interprete, '\0', env->len + 1);
}

char		start_interprete(t_env *env)
{
	env->start = 0;
	env->last_char = '\0';
	if (env->len == 0)
		return (STOP);
	handle_alias(env);
	do_interprete(env);
	if (env->last_char)
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
