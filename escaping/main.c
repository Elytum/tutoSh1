#include <stdlib.h>
#include <string.h>
#define LINE_SIZE 4096
#define STRING "te\\  st"

#define TEST_SIZE 15
#include <stdio.h>

#define INTERPRETED 1
#define SPACING 2
#define SIMPLE_QUOTED 3
#define DOUBLE_QUOTED 4
#define BACK_QUOTED 5
#define BACKSLASHED 6
#define REMOVE 7

typedef struct	s_env
{
	size_t		len;
	size_t		pos;
	char		line[LINE_SIZE];
	char		buffer[LINE_SIZE];
	char		interprete[LINE_SIZE];
}				t_env;

void		debug_env(t_env *env)
{
	size_t	position;

	printf("Buffer:     {");
	position = 0;
	while (position <= env->len)
	{
		if (position == 0)
			printf("'%c'", env->buffer[position]);
		else if (position != env->len)
			printf(", '%c'", env->buffer[position]);
		else
			printf(", '%c'}.\n", env->buffer[position]);
		++position;
	}

	printf("interprete: {");
	position = 0;
	while (position <= env->len)
	{
		if (position == 0)
			printf(" %i", env->interprete[position]);
		else if (position != env->len)
			printf(",   %i", env->interprete[position]);
		else
			printf(",   %i}.\n", env->interprete[position]);
		++position;
	}
	if (env->interprete[env->len] == INTERPRETED || env->interprete[env->len] == SPACING)
		printf("Line is closed\n");
	else if (env->interprete[env->len] == SIMPLE_QUOTED)
		printf("Line is not closed due to a simple quote.\n");
	else if (env->interprete[env->len] == DOUBLE_QUOTED)
		printf("Line is not closed due to a double quote.\n");
	else if (env->interprete[env->len] == BACK_QUOTED)
		printf("Line is not closed due to a back quote.\n");
	else if (env->interprete[env->len] == BACKSLASHED)
		printf("Line is not closed due to a backslash.\n");
}

void		interprete_simple_quote(t_env *env)
{
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '\'')
		{
			env->interprete[env->pos++] = SIMPLE_QUOTED;
			return ;
		}
		else
			env->interprete[env->pos++] = SIMPLE_QUOTED;
	}
}

void		interprete_double_quote(t_env *env)
{
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '\"')
		{
			env->interprete[env->pos++] = DOUBLE_QUOTED;
			return ;
		}
		else
			env->interprete[env->pos++] = DOUBLE_QUOTED;
	}
}

void		interprete_back_quote(t_env *env)
{
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '`')
		{
			env->interprete[env->pos++] = BACK_QUOTED;
			return ;
		}
		else
			env->interprete[env->pos++] = BACK_QUOTED;
	}
}

void		interprete_backslash(t_env *env)
{
	env->interprete[env->pos++] = BACKSLASHED;
}

void		do_interprete(t_env *env)
{
	env->pos = 0;
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '\'')
		{
			env->interprete[env->pos++] = SIMPLE_QUOTED;
			interprete_simple_quote(env);
		}
		else if (env->buffer[env->pos] == '\"')
		{
			env->interprete[env->pos++] = DOUBLE_QUOTED;
			interprete_double_quote(env);
		}
		else if (env->buffer[env->pos] == '\\')
		{
			env->interprete[env->pos++] = BACKSLASHED;
			interprete_backslash(env);
		}
		else if (env->buffer[env->pos] == '`')
		{
			env->interprete[env->pos++] = BACK_QUOTED;
			interprete_back_quote(env);
		}
		else if (env->buffer[env->pos] == ' ' || env->buffer[env->pos] == '\t')
			env->interprete[env->pos++] = SPACING;
		else
			env->interprete[env->pos++] = INTERPRETED;
	}
}

void		process_simple_quote(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	env->interprete[env->pos++] = INTERPRETED;
}

void		process_double_quote(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	env->interprete[env->pos++] = INTERPRETED;
}

void		process_backslash(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	env->interprete[env->pos++] = INTERPRETED;
}

void		process_back_quote(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	env->interprete[env->pos++] = INTERPRETED;
}

void		do_process(t_env *env)
{
	env->pos = 0;
	while (env->pos <= env->len)
	{
		if (env->interprete[env->pos] == INTERPRETED || env->interprete[env->pos] == SPACING)
			env->pos++;
		else if (env->interprete[env->pos] == SIMPLE_QUOTED)
			process_simple_quote(env);
		else if (env->interprete[env->pos] == DOUBLE_QUOTED)
			process_double_quote(env);
		else if (env->interprete[env->pos] == BACKSLASHED)
			process_back_quote(env);
		else if (env->interprete[env->pos] == BACK_QUOTED)
			process_backslash(env);
		else
			++env->pos;
	}
}

void		do_simplify(t_env *env)
{
	size_t			buffer_pos;
	const size_t	len = env->len;

	buffer_pos = 0;
	env->pos = 0;
	while (env->pos <= len)
	{
		if (env->interprete[env->pos] == REMOVE)
		{
			++env->pos;
			--env->len;
		}
		else
		{
			env->buffer[buffer_pos] = env->buffer[env->pos];
			env->interprete[buffer_pos] = env->interprete[env->pos];
			++buffer_pos;
			++env->pos;
		}
	}
}

void		launch_interprete(t_env *env)
{
	memcpy(env->buffer, env->line, env->len + 1);
	memset(env->interprete, '\0', env->len + 1);

	do_interprete(env);
	if (env->interprete[env->len] != INTERPRETED && env->interprete[env->len] != SPACING)
		debug_env(env);
	else
	{
		debug_env(env);
		do_process(env);
		debug_env(env);
		do_simplify(env);
		debug_env(env);
	}
	
}

int			main(void)
{
	t_env	*env;

	if (!(env = (t_env *)malloc(sizeof(t_env))))
		return (-1);
	memcpy(env->line, STRING, LINE_SIZE - 1);
	env->line[LINE_SIZE - 1] = '\0';
	env->len = strlen(env->line);
	launch_interprete(env);
	return (0);
}