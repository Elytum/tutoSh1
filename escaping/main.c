#include <stdlib.h>
#include <string.h>
#define LINE_SIZE 4096
#define STRING " ~te\"r` s`l\"t"

#define TEST_SIZE 15
#include <stdio.h>

#define INTERPRETED 1
#define SPACING 2
#define SIMPLE_QUOTED 3
#define DOUBLE_QUOTED 4
#define BACK_QUOTED 5
#define BACKSLASHED 6
#define LOCAL_VARIABLE 7
#define TILDE 8
#define REMOVE 9

#define ERROR 0
#define NO_ERROR 1
#define TRUE 0
#define FALSE 1

typedef struct	s_env
{
	size_t		len;
	size_t		pos;
	char		line[LINE_SIZE];
	char		buffer[LINE_SIZE];
	char		interprete[LINE_SIZE];
	int			argc;
	char		**argv;
}				t_env;

void		debug_env(t_env *env);
void		interprete_simple_quote(t_env *env);
void		interprete_double_quote(t_env *env);
void		interprete_back_quote(t_env *env);
void		interprete_value(t_env *env);
void		interprete_backslash(t_env *env);
void		interprete_tilde(t_env *env);
void		interprete_spacing(t_env *env);
void		interprete_normal(t_env *env);
void		do_interprete(t_env *env);
void		do_process(t_env *env);
void		do_simplify(t_env *env);
int			set_arguments(t_env *env);
void		launch_interprete(t_env *env);

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
	env->interprete[env->pos++] = REMOVE;
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '\'')
		{
			env->interprete[env->pos++] = REMOVE;
			return ;
		}
		else
			env->interprete[env->pos++] = SIMPLE_QUOTED;
	}
}

void		interprete_double_quote(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '\"')
		{
			env->interprete[env->pos++] = REMOVE;
			return ;
		}
		else if (env->buffer[env->pos] == '`')
			interprete_back_quote(env);
		else
			env->interprete[env->pos++] = DOUBLE_QUOTED;
	}
}

void		interprete_back_quote(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '`')
		{
			env->interprete[env->pos++] = REMOVE;
			return ;
		}
		else
			env->interprete[env->pos++] = BACK_QUOTED;
	}
}

void		interprete_backslash(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	if (env->buffer[env->pos] == '\r')
		env->interprete[env->pos] = REMOVE;
	else
		env->interprete[env->pos] = INTERPRETED;
	++env->pos;
}

void		interprete_value(t_env *env)
{
	env->interprete[env->pos++] = REMOVE;
	while (env->buffer[env->pos] != '\'' && env->buffer[env->pos] != '\"' &&
			env->buffer[env->pos] != '\\' && env->buffer[env->pos] != '`' &&
			env->buffer[env->pos] != '$' && env->buffer[env->pos] != ' ' &&
			env->buffer[env->pos] != '\t')
		env->interprete[env->pos++] = LOCAL_VARIABLE;
}

void		interprete_tilde(t_env *env)
{
	env->interprete[env->pos++] = TILDE;
}

void		interprete_spacing(t_env *env)
{
	env->interprete[env->pos++] = SPACING;
}

void		interprete_normal(t_env *env)
{
	env->interprete[env->pos++] = INTERPRETED;
}

void		do_interprete(t_env *env)
{
	env->pos = 0;
	while (env->pos <= env->len)
	{
		if (env->buffer[env->pos] == '\'')
			interprete_simple_quote(env);
		else if (env->buffer[env->pos] == '\"')
			interprete_double_quote(env);
		else if (env->buffer[env->pos] == '\\')
			interprete_backslash(env);
		else if (env->buffer[env->pos] == '`')
			interprete_back_quote(env);
		else if (env->buffer[env->pos] == '$')
			interprete_value(env);
		else if (env->buffer[env->pos] == '~')
			interprete_tilde(env);
		else if (env->buffer[env->pos] == ' ' || env->buffer[env->pos] == '\t')
			interprete_spacing(env);
		else
			interprete_normal(env);
	}
}

void		process_back_quotes(t_env *env) // Actually just ignores it
{
	char	kind;

	env->pos = 0;
	kind = INTERPRETED;
	while (env->pos <= env->len)
	{
		if (env->interprete[env->pos] == BACK_QUOTED)
			env->interprete[env->pos] = kind;
		kind = env->interprete[env->pos];
		++env->pos;
	}
}

void		do_process(t_env *env)
{
	process_back_quotes(env);
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

int			set_arguments(t_env *env)
{
	set_argc(env);
	printf("There is %i arguments\n", env->argc);
	return (NO_ERROR);
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
		do_simplify(env);
		do_process(env);
		debug_env(env);
		set_arguments(env);
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