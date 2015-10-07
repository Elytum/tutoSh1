#include <interprete.h>
#include <string.h>

void		interprete_value(t_env *env)
{
	size_t	newpos;

	newpos = env->pos + 1;
	env->interprete[newpos] = START_LOCAL_VARIABLE;
	while (env->buffer[newpos] != '\'' && env->buffer[newpos] != '\"' &&
			env->buffer[newpos] != '\\' && env->buffer[newpos] != '`' &&
			env->buffer[newpos] != '$' && env->buffer[newpos] != ' ' &&
			env->buffer[newpos] != '\t' && env->buffer[newpos] != '\0')
		newpos++;
	if ((env->pos == 0 || env->interprete[env->pos - 1] == SPACING) &&
		(env->buffer[newpos] == ' ' || env->buffer[newpos] == '\t'))
		memcpy(env->interprete + env->len, LOCAL_VARIABLE, newpos - env->len);
	else
		memcpy(env->interprete + env->len, ALONE_LOCAL_VARIABLE, newpos - env->len);
	env->pos = newpos;
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