#include <interprete.h>

void		interprete_tilde(t_env *env)
{
	env->interprete[env->pos++] = TILDE;
}

void		process_tilde(t_env *env) // Actually just ignores it
{
	char	kind;

	env->pos = 0;
	kind = INTERPRETED;
	while (env->pos <= env->len)
	{
		if (env->interprete[env->pos] == TILDE)
			env->interprete[env->pos] = kind;
		kind = env->interprete[env->pos];
		++env->pos;
	}
}
