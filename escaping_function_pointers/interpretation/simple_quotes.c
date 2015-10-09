#include <interprete.h>

void		interprete_simple_quote(t_env *env)
{
	const char		*ptr_buffer = env->buffer;
	char			*ptr_interprete;
	const size_t	len = env->len;

	ptr_interprete = env->interprete;
	env->interprete[env->pos++] = REMOVE;
	while (env->pos <= len)
	{
		if (ptr_buffer[env->pos] == '\'')
		{
			ptr_interprete[env->pos++] = REMOVE;
			return ;
		}
		else
			ptr_interprete[env->pos++] = INTERPRETED;
	}
	env->last_char = SIMPLE_QUOTED;
}

size_t		len_simple_quote(t_env *env, size_t *pos)
{
	const char		*ptr_interprete = env->interprete;
	const size_t	len = env->len;
	size_t			tmp_pos;
	size_t			size;

	size = 1;
	++*pos;
	tmp_pos = *pos;
	while (tmp_pos < len && ptr_interprete[tmp_pos] == SIMPLE_QUOTED)
	{
		++size;
		++tmp_pos;
	}
	*pos = tmp_pos;
	return (size);
}

void		extract_simple_quote(t_env *env, size_t *pos, char **ptr)
{
	const char		*ptr_buffer = env->buffer;
	const char		*ptr_interprete = env->interprete;
	size_t			tmp_pos;
	char			*tmp_ptr;

	tmp_pos = *pos;
	tmp_ptr = *ptr;
	while (ptr_interprete[*pos] == SIMPLE_QUOTED)
	{
		*tmp_ptr = ptr_buffer[*pos];
		++tmp_ptr;
		++tmp_pos;
	}
	*pos = tmp_pos;
	*ptr = tmp_ptr;

}