#ifndef SHELL_H
# define SHELL_H

#define ERROR 0
#define NO_ERROR 1
#define TRUE 0
#define FALSE 1

#define NORMAL_EXIT 0
#define ERROR_EXIT -1

# define LINE_SIZE 4096

# include <stdlib.h>
# include <htable.h>

# include <limits.h>

typedef struct	s_env
{
	size_t		len;
	size_t		pos;
	char		line[LINE_SIZE];
	char		buffer[LINE_SIZE];
	char		interprete[LINE_SIZE];
	int			argc;
	char		**argv;
	hashtable_t	*local_variables;
	
	char		pwd[PATH_MAX];
	char		home[PATH_MAX];
	size_t		pwd_len;
	size_t		home_len;
}				t_env;

#endif
