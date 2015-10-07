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

// size_t		len_tilde(t_env *env, size_t *pos) {
// 	++*pos;
// 	return (env->home_len);
// }

// void		extract_tilde(t_env *env, size_t *pos, char **ptr)
// {
// 	memcpy(*ptr, env->home, env->home_len);
// 	*ptr += env->home_len;
// 	++*pos;
// }