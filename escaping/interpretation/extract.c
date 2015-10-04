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

size_t		extract_len(t_env *env, size_t *pos)
{
	size_t	len;

	while (pos < env->len && env->interprete[pos] != SPACING)
	{
		if (env->interprete[pos] != NORMAL)
			;
		else if (env->interprete[pos] != SIMPLE_QUOTED)
			;
		else if (env->interprete[pos] != DOUBLE_QUOTED)
			;
		else if (env->interprete[pos] != BACK_QUOTED)
			;
		else if (env->interprete[pos] != BACKSLASH_QUOTED)
			;
		else if (env->interprete[pos] != SIMPLE_QUOTED)
			;
		else if (env->interprete[pos] != SIMPLE_QUOTED)
			;
	}
}

int			set_argv(t_env *env)
{
	size_t	pos;
	char	**ptr;
	size_t	len;

	if (!(env->argv = (char **)malloc(sizeof(char *) * (env->argc + 1))))
		return (ERROR);
	ptr = env->argv;
	pos = 0;
	while (pos < env->len)
	{
		while (pos < env->len && env->interprete[pos] == SPACING)
			++pos;
		len = extract_len(env, &pos);
		while (pos < env->len && env->interprete[pos] != SPACING)
			++pos;
				// if (old_pos - pos)
				// 	*ptr++ = strndup(env->buffer + old_pos, pos - old_pos);
	}
	*ptr = NULL;
	put_env(env);
	return (NO_ERROR);
}