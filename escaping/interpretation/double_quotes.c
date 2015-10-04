#include <interprete.h>

void		interprete_double_quote(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '\"')
		{
			env->interprete[env->pos++] = REMOVE;
			return ;
		}
		else if (env->buffer[env->pos] == '`')
			interprete_back_quote(env);
		else
			env->interprete[env->pos++] = DOUBLE_QUOTED;
	}
}