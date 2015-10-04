#include <interprete.h>
#include <string.h>

void		interprete_value(t_env *env)
{
	env->interprete[env->pos++] = START_LOCAL_VARIABLE;
	while (env->buffer[env->pos] != '\'' && env->buffer[env->pos] != '\"' &&
			env->buffer[env->pos] != '\\' && env->buffer[env->pos] != '`' &&
			env->buffer[env->pos] != '$' && env->buffer[env->pos] != ' ' &&
			env->buffer[env->pos] != '\t' && env->buffer[env->pos] != '\0')
		env->interprete[env->pos++] = LOCAL_VARIABLE;
}

size_t		len_value(t_env *env, size_t *pos)
{
	char	c;
	char	*tmp;
	size_t	end;

	end = *pos + 1;
	while (end < env->len && env->interprete[end] == LOCAL_VARIABLE)
		++end;
	if (end == *pos + 1)
		return (0);
	c = env->buffer[end];
	env->buffer[end] = '\0';
	tmp = ht_get(env->local_variables, env->buffer + *pos + 1);
	env->buffer[end] = c;
	*pos = end;
	if (!tmp)
		return (0);
	return (strlen(tmp));
}

void		extract_value(t_env *env, size_t *pos, char **ptr)
{
	char	c;
	char	*tmp;
	size_t	end;

	end = *pos + 1;
	while (end < env->len && env->interprete[end] == LOCAL_VARIABLE)
		++end;
	if (end == *pos + 1)
		return ;
	c = env->buffer[end];
	env->buffer[end] = '\0';
	tmp = ht_get(env->local_variables, env->buffer + *pos + 1);
	env->buffer[end] = c;
	*pos = end;
	if (!tmp)
		return ;
	end = strlen(tmp);
	memcpy(*ptr, tmp, end);
	*ptr += end;
}