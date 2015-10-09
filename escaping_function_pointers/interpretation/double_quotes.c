#include <interprete.h>
#include <string.h>

void		interprete_double_quote(t_env *env)
{
	const char		*ptr_buffer = env->buffer;
	char			*ptr_interprete;
	const size_t	len = env->len;
	size_t			tmp_pos;

	ptr_interprete = env->interprete;
	tmp_pos = env->pos + 1;
	ptr_interprete[tmp_pos] = REMOVE;
	while (tmp_pos <= len)
	{
		if (ptr_buffer[tmp_pos] == '\"')
		{
			ptr_interprete[tmp_pos++] = REMOVE;
			env->pos = tmp_pos;
			return ;
		}
		else if (ptr_buffer[tmp_pos] == '`')
			interprete_back_quote(env);
		else if (ptr_buffer[tmp_pos] == '$')
			interprete_value(env);
		else if (tmp_pos < len &&
			ptr_buffer[tmp_pos] == '\\' &&
			(ptr_buffer[tmp_pos + 1] == '$' ||
			ptr_buffer[tmp_pos + 1] == '`' ||
			ptr_buffer[tmp_pos + 1] == '"' ||
			ptr_buffer[tmp_pos + 1] == '\n'))
			interprete_backslash(env);
		else
			ptr_interprete[tmp_pos++] = INTERPRETED;
	}
	env->last_char = DOUBLE_QUOTED;
	env->pos = tmp_pos;
}

size_t		len_double_quote(t_env *env, size_t *pos)
{
	const size_t	len = env->len;
	const char		*ptr_interprete = env->interprete;
	size_t			size;
	size_t			tmp_pos;

	size = 1;
	tmp_pos = *pos + 1;
	while (tmp_pos < len && ptr_interprete[tmp_pos] == DOUBLE_QUOTED)
	{
		++size;
		++tmp_pos;
	}
	*pos = tmp_pos;
	return (size);
}

void		extract_double_quote(t_env *env, size_t *pos, char **ptr)
{
	const char	*ptr_buffer = env->buffer;
	const char	*ptr_interprete = env->interprete;
	char		*tmp_ptr = *ptr;
	size_t		tmp_pos;

	tmp_ptr = *ptr;
	tmp_pos = *pos;
	while (ptr_interprete[tmp_pos] == DOUBLE_QUOTED)
	{
		*tmp_ptr = ptr_buffer[tmp_pos];
		++tmp_ptr;
		++tmp_pos;
	}
	*ptr = tmp_ptr;
	*pos = tmp_pos;
}
