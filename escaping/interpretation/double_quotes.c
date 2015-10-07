#include <interprete.h>
#include <string.h>

void		interprete_double_quote(t_env *env)
{
	const size_t	old_pos = env->pos + 1;

	env->interprete[env->pos++] = REMOVE;
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '\"')
		{
			env->interprete[env->pos++] = REMOVE;
			break ;
		}
		else if (env->buffer[env->pos] == '`')
			interprete_back_quote(env);
		else if (env->buffer[env->pos] == '$')
			interprete_value(env);
		else if (env->pos < env->len &&
			env->buffer[env->pos] == '\\' &&
			(env->buffer[env->pos + 1] == '$' ||
			env->buffer[env->pos + 1] == '`' ||
			env->buffer[env->pos + 1] == '"' ||
			env->buffer[env->pos + 1] == '\n'))
			interprete_backslash(env);
		else
			env->interprete[env->pos++] = INTERPRETED;
	}
}

size_t		len_double_quote(t_env *env, size_t *pos)
{
	size_t	size;

	size = 1;
	++*pos;
	while (*pos < env->len && env->interprete[*pos] == DOUBLE_QUOTED)
	{
		++size;
		++*pos;
	}
	return (size);
}

void		extract_double_quote(t_env *env, size_t *pos, char **ptr)
{
	while (env->interprete[*pos] == DOUBLE_QUOTED)
	{
		**ptr = env->buffer[*pos];
		++*ptr;
		++*pos;
	}
}
