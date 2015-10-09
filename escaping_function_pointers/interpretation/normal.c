#include <interprete.h>

#include <stdio.h>

void		interprete_spacing(t_env *env)
{
	env->interprete[env->pos++] = SPACING;
}

void		interprete_normal(t_env *env)
{
	env->interprete[env->pos++] = INTERPRETED;
}

size_t		len_normal(t_env *env, size_t *pos)
{
	(void)env;
	++*pos;
	return (1);
}

void		extract_normal(t_env *env, size_t *pos, char **ptr)
{
	char		*ptr_buffer = env->buffer;
	const char	*ptr_interprete = env->interprete;
	size_t		tmp_pos;
	char		*tmp_ptr;

	tmp_pos = *pos;
	tmp_ptr = *ptr;
	while (ptr_interprete[tmp_pos] == INTERPRETED)
	{
		*tmp_ptr = ptr_buffer[tmp_pos];
		++tmp_ptr;
		++tmp_pos;
	}
	*pos = tmp_pos;
	*ptr = tmp_ptr;
}