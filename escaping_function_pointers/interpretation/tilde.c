#include <interprete.h>
#include <string.h>

#include <stdio.h>

void		interprete_tilde(t_env *env)
{
	if ((env->pos == 0 || env->interprete[env->pos - 1] == SPACING) &&
		(env->buffer[env->pos + 1] == ' ' ||
		env->buffer[env->pos + 1] == '\t' ||
		env->buffer[env->pos + 1] == '\0'))
		env->interprete[env->pos] = TILDE;
	else
		env->interprete[env->pos] = INTERPRETED;
	++env->pos;
}
