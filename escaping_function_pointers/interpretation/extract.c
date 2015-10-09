#include <interprete.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>

void			set_argc(t_env *env)
{
	const char		*ptr_interprete = env->interprete;
	const size_t	len = env->len;
	char			in_word;
	size_t			pos;

	env->argc = 0;
	pos = env->start;
	in_word = FALSE;
	while (pos < len && ptr_interprete[pos] != DELIMITER)
	{
		if (ptr_interprete[pos] == SPACING)
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

int			avoid_allocation(t_env *env, size_t *pos, char ***ptr)
{
	const char		*ptr_interprete = env->interprete;
	const size_t	len = env->len;
	size_t			saved_pos;
	char			tmp_char;

	saved_pos = *pos;
	if (ptr_interprete[*pos] == TILDE)
	{
		**ptr = env->home;
		++*pos;
	}
	else if (ptr_interprete[saved_pos + 1] == ALONE_LOCAL_VARIABLE)
	{
		++saved_pos;
		while (ptr_interprete[saved_pos] == ALONE_LOCAL_VARIABLE)
			++saved_pos;
		tmp_char = env->buffer[saved_pos];
		env->buffer[saved_pos] = '\0';
		**ptr = ht_get(env->local_variables, env->buffer + *pos + 1);
		env->buffer[saved_pos] = tmp_char;
		*pos = saved_pos;
	}
	else
	{
		while (saved_pos < len && ptr_interprete[saved_pos] != SPACING)
		{
			if (ptr_interprete[saved_pos] != INTERPRETED)
				return (0);
			++saved_pos;
		}
		if (ptr_interprete[saved_pos] != SPACING && ptr_interprete[saved_pos] != INTERPRETED)
			return (0);
		**ptr = env->buffer + *pos;
		*pos = saved_pos;
		env->buffer[saved_pos] = '\0';
	}
	++*ptr;
	++env->argc;
	return (1);
}

size_t		should_len(t_env *env, size_t *pos, char ***ptr)
{
	const char	error[] = "should_len id too big\n";
	const char	*ptr_interprete = env->interprete;
	size_t		len;
	char		c;

	if (avoid_allocation(env, pos, ptr))
		return (0);
	len = 0;
	while (*pos < len && ptr_interprete[*pos] != SPACING &&
		ptr_interprete[*pos] != DELIMITER)
	{
		c = ptr_interprete[*pos];
		if ((unsigned long)c >= sizeof(env->should_len_tab))
		{
			write(2, error, sizeof(error) - 1);
			exit(0);
		}
		len += env->should_len_tab[(int)c](env, pos);
	}
	return len;
}

void		extract_content(t_env *env, size_t pos, char *ptr)
{
	const char		error[] = "extract_content id too big\n";
	const size_t	len = env->len;
	const char		*ptr_interprete = env->interprete;
	char			c;

	while (pos < len && ptr_interprete[pos] != SPACING &&
		ptr_interprete[pos] != DELIMITER)
	{
		c = ptr_interprete[pos];
		if ((unsigned long)c >= sizeof(env->extract_content_tab))
		{
			write(2, error, sizeof(error) - 1);
			exit(0);
		}
		env->extract_content_tab[(int)c](env, &pos, &ptr);
	}
	*ptr = '\0';
}

int			set_argv(t_env *env)
{
	const char		*ptr_interprete = env->interprete;
	const size_t	len = env->len;
	size_t			pos;
	size_t			tmp_pos;
	size_t			alloc_len;
	char			**ptr;

	ptr = env->argv;
	env->error = NO_ERROR;
	pos = env->start;
	while (pos < len && ptr_interprete[pos] != DELIMITER)
	{
		while (pos < len && ptr_interprete[pos] == SPACING)
			++pos;
		tmp_pos = pos;
		if ((alloc_len = should_len(env, &pos, &ptr)))
		{
			if (!(*ptr = (char *)debug_malloc(sizeof(char) * (alloc_len + 1))))
				return (ERROR);
			if (env->argv_pool_size < _POSIX_ARG_MAX)
				env->argv_pool[env->argv_pool_size++] = *ptr;
			else
				env->error = ERROR;
			extract_content(env, tmp_pos, *ptr);
			++ptr;
		}
		else
			--env->argc;
		if (env->error == ERROR)
			return (ERROR);
	}
	*ptr = NULL;
	env->start = pos + 1;
	return (NO_ERROR);
}