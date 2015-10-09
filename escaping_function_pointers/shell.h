#ifndef SHELL_H
# define SHELL_H

#define ERROR 0
#define NO_ERROR 1
#define TRUE 0
#define FALSE 1

#define NORMAL_EXIT 0
#define ERROR_EXIT -1

# include <htable.h>
# include <config.h>

# include <limits.h>
# include <stdlib.h>

typedef struct	s_env
{
	size_t		len;
	size_t		pos;
	size_t		start;
	char		line[_POSIX2_LINE_MAX];
	char		buffer[_POSIX2_LINE_MAX];
	char		interprete[_POSIX2_LINE_MAX];
	int			argc;
	char		*argv[_POSIX_ARG_MAX];
	hashtable_t	*local_variables;
	
	char		pwd[_POSIX_PATH_MAX];
	char		home[_POSIX_PATH_MAX];
	size_t		pwd_len;
	size_t		home_len;

	char		*argv_pool[_POSIX_ARG_MAX];
	size_t		argv_pool_size;

	char		error;
	char		last_token;
	char		pursue;
	char		last_char;

	void		(*do_interprete_tab[256])(struct s_env *env);
	char		interprete_value_stop[256];
	char		standard_delimiters[256];
	char		spaces[256];
	size_t		(*should_len_tab[MAX_BASIC_INTERPRETATION])(struct s_env *env, size_t *pos);
	void		(*extract_content_tab[MAX_BASIC_INTERPRETATION])(struct s_env *env, size_t *pos, char **ptr);

}				t_env;

#endif
