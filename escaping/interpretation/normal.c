#include <interprete.h>

void		interprete_spacing(t_env *env)
{
	env->interprete[env->pos++] = SPACING;
}

void		interprete_normal(t_env *env)
{
	env->interprete[env->pos++] = INTERPRETED;
}