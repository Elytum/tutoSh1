#include <interprete.h>

void		interprete_back_quote(t_env *env)
{
	const char		*ptr_buffer = env->buffer;
	const size_t	len = env->len;
	size_t			pos;
	char			*ptr_interprete;

	ptr_interprete = env->interprete;
	pos = env->pos;
	ptr_interprete[pos++] = REMOVE;
	while (pos <= len)
	{
		if (ptr_buffer[pos] == '`')
		{
			ptr_interprete[pos++] = REMOVE;
			env->pos = pos;
			return ;
		}
		else
			ptr_interprete[pos++] = BACK_QUOTED;
	}
	env->pos = pos;
	env->last_char = BACK_QUOTED;
}

void		process_back_quotes(t_env *env) // Actually just ignores it
{
	const size_t	len = env->len;
	size_t			pos;
	char			kind;
	char			*ptr_interprete;

	pos = env->pos;
	ptr_interprete = env->interprete;
	pos = env->start;
	kind = INTERPRETED;
	while (pos <= len)
	{
		if (ptr_interprete[pos] == BACK_QUOTED)
			ptr_interprete[pos] = kind;
		kind = ptr_interprete[pos];
		++pos;
	}
	env->pos = pos;
}

size_t		len_back_quote(t_env *env, size_t *pos) // Implement once it can be
{
	(void)env;
	++*pos;
	return (0);
}

void		extract_back_quote(t_env *env, size_t *pos, char **ptr) // Implement once it can be
{
	(void)env;
	**ptr++ = env->buffer[*pos++];
}