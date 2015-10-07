#ifndef INTERPRETE_H
# define INTERPRETE_H

#include <shell.h>

#define INTERPRETED 1
#define SPACING 2
#define SIMPLE_QUOTED 3
#define DOUBLE_QUOTED 4
#define BACK_QUOTED 5
#define BACKSLASHED 6
#define START_LOCAL_VARIABLE 7
#define LOCAL_VARIABLE 8
#define ALONE_LOCAL_VARIABLE 9
#define TILDE 10
#define REMOVE 11

void		launch_interprete(t_env *env);

void		debug_env(t_env *env);
void		put_env(t_env *env);

void		set_argc(t_env *env);
int			set_argv(t_env *env);
int			set_arguments(t_env *env);

void		do_interprete(t_env *env);
void		interprete_simple_quote(t_env *env);
void		interprete_double_quote(t_env *env);
void		interprete_back_quote(t_env *env);
void		interprete_value(t_env *env);
void		interprete_backslash(t_env *env);
void		interprete_tilde(t_env *env);
void		interprete_spacing(t_env *env);
void		interprete_normal(t_env *env);
void		interprete_null(t_env *env);

void		do_process(t_env *env);
void		process_back_quotes(t_env *env);

void		do_simplify(t_env *env);

size_t		len_normal(t_env *env, size_t *pos);
size_t		len_simple_quote(t_env *env, size_t *pos);
size_t		len_double_quote(t_env *env, size_t *pos);
size_t		len_back_quote(t_env *env, size_t *pos);
size_t		len_backslash(t_env *env, size_t *pos);
size_t		len_value(t_env *env, size_t *pos);
// size_t		len_tilde(t_env *env, size_t *pos);

void		extract_normal(t_env *env, size_t *pos, char **ptr);
void		extract_simple_quote(t_env *env, size_t *pos, char **ptr);
void		extract_double_quote(t_env *env, size_t *pos, char **ptr);
void		extract_back_quote(t_env *env, size_t *pos, char **ptr);
void		extract_backslash(t_env *env, size_t *pos, char **ptr);
void		extract_value(t_env *env, size_t *pos, char **ptr);
// void		extract_tilde(t_env *env, size_t *pos, char **ptr);

void		add_local_variable(t_env *env, const char *key, const char *value);

void		*debug_malloc(size_t size);

#endif
