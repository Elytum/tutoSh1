#include <interprete.h>
#include <string.h>

void			set_argc(t_env *env)
{
	char	in_word;
	size_t	pos;

	env->argc = 0;
	pos = 0;
	in_word = FALSE;
	while (pos < env->len)
	{
		if (env->interprete[pos] == SPACING)
				in_word = FALSE;
		else
		{
			if (in_word == FALSE)
				++env->argc;
			in_word = TRUE;
		}
		++pos;
	}
}

int			set_argv(t_env *env)
{
	size_t	pos;
	size_t	old_pos;
	char	**ptr;

	if (!(env->argv = (char **)malloc(sizeof(char *) * (env->argc + 1))))
		return (ERROR);
	ptr = env->argv;
	pos = 0;
	while (pos < env->len)
	{
		while (pos < env->len && env->interprete[pos] == SPACING)
			++pos;
		old_pos = pos;
		while (pos < env->len && env->interprete[pos] != SPACING)
			++pos;
		if (old_pos - pos)
			*ptr++ = strndup(env->buffer + old_pos, pos - old_pos);
	}
	*ptr = NULL;
	put_env(env);
	return (NO_ERROR);
}