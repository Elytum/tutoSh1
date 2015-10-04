#include <interprete.h>

void		interprete_backslash(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	if (env->buffer[env->pos] == '\r')
		env->interprete[env->pos] = REMOVE;
	else
		env->interprete[env->pos] = INTERPRETED;
	++env->pos;
}

size_t		len_backslash(t_env *env, size_t *pos)
{
	++*pos;
	return (1);
}

void		extract_backslash(t_env *env, size_t *pos, char **ptr)
{
	**ptr = env->buffer[*pos];
	++*ptr;
	++*pos;
}
