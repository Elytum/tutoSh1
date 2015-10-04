#include <interprete.h>

void		interprete_back_quote(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '`')
		{
			env->interprete[env->pos++] = REMOVE;
			return ;
		}
		else
			env->interprete[env->pos++] = BACK_QUOTED;
	}
}

void		process_back_quotes(t_env *env) // Actually just ignores it
{
	char	kind;

	env->pos = 0;
	kind = INTERPRETED;
	while (env->pos <= env->len)
	{
		if (env->interprete[env->pos] == BACK_QUOTED)
			env->interprete[env->pos] = kind;
		kind = env->interprete[env->pos];
		++env->pos;
	}
}
