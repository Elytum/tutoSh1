#include <interprete.h>

void		interprete_value(t_env *env)
{
	env->interprete[env->pos++] = START_LOCAL_VARIABLE;
	while (env->buffer[env->pos] != '\'' && env->buffer[env->pos] != '\"' &&
			env->buffer[env->pos] != '\\' && env->buffer[env->pos] != '`' &&
			env->buffer[env->pos] != '$' && env->buffer[env->pos] != ' ' &&
			env->buffer[env->pos] != '\t')
		env->interprete[env->pos++] = LOCAL_VARIABLE;
}

void		process_value(t_env *env) // Actually just ignores it
{
	(void)env;
	// char	kind;

	// env->pos = 0;
	// kind = INTERPRETED;
	// while (env->pos <= env->len)
	// {
	// 	if (env->interprete[env->pos] == LOCAL_VARIABLE)
	// 		env->interprete[env->pos] = kind;
	// 	kind = env->interprete[env->pos];
	// 	++env->pos;
	// }
}
