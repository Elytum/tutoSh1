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