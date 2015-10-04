#include <interprete.h>
#include <string.h>

#include <stdio.h>

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

	len = 0;
	while (*pos < env->len && env->interprete[*pos] != SPACING)
	{
		if (env->interprete[*pos] == INTERPRETED)
			len += len_normal(env, pos);
		else if (env->interprete[*pos] == SIMPLE_QUOTED)
			len += len_simple_quote(env, pos);
		else if (env->interprete[*pos] == DOUBLE_QUOTED)
			len += len_double_quote(env, pos);
		else if (env->interprete[*pos] == BACK_QUOTED)
			len += len_back_quote(env, pos);
		else if (env->interprete[*pos] == BACKSLASHED)
			len += len_backslash(env, pos);
		else if (env->interprete[*pos] == START_LOCAL_VARIABLE)
			len += len_value(env, pos);
		else if (env->interprete[*pos] == TILDE)
			len += len_tilde(env, pos);
	}
	return len;
}

void		extract_content(t_env *env, size_t pos, char *ptr)
{
	while (pos < env->len && env->interprete[pos] != SPACING)
	{
		if (env->interprete[pos] == INTERPRETED)
			extract_normal(env, &pos, &ptr);
		else if (env->interprete[pos] == SIMPLE_QUOTED)
			extract_simple_quote(env, &pos, &ptr);
		else if (env->interprete[pos] == DOUBLE_QUOTED)
			extract_double_quote(env, &pos, &ptr);
		else if (env->interprete[pos] == BACK_QUOTED)
			extract_back_quote(env, &pos, &ptr);
		else if (env->interprete[pos] == BACKSLASHED)
			extract_backslash(env, &pos, &ptr);
		else if (env->interprete[pos] == START_LOCAL_VARIABLE)
			extract_value(env, &pos, &ptr);
		else if (env->interprete[pos] == TILDE)
			extract_tilde(env, &pos, &ptr);
	}
	*ptr = '\0';
}

int			set_argv(t_env *env)
{
	size_t	pos;
	size_t	tmp_pos;
	size_t	len;
	char	**ptr;

	if (!(env->argv = (char **)malloc(sizeof(char *) * (env->argc + 1))))
		return (ERROR);
	ptr = env->argv;
	pos = 0;
	while (pos < env->len)
	{
		while (pos < env->len && env->interprete[pos] == SPACING)
			++pos;
		tmp_pos = pos;
		if ((len = extract_len(env, &pos)))
		{
			if (!(*ptr = (char *)malloc(sizeof(char) * (len + 1))))
				return (ERROR);
			extract_content(env, tmp_pos, *ptr);
			++ptr;
		}
		else
			--env->argc;
	}
	*ptr = NULL;
	return (NO_ERROR);
}