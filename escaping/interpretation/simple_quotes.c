#include <interprete.h>

void		interprete_simple_quote(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '\'')
		{
			env->interprete[env->pos++] = REMOVE;
			return ;
		}
		else
			env->interprete[env->pos++] = SIMPLE_QUOTED;
	}
}