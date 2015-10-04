#ifndef INTERPRETE_H
# define INTERPRETE_H

#include <shell.h>

#define INTERPRETED 1
#define SPACING 2
#define SIMPLE_QUOTED 3
#define DOUBLE_QUOTED 4
#define BACK_QUOTED 5
#define BACKSLASHED 6
#define LOCAL_VARIABLE 7
#define TILDE 8
#define REMOVE 9

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

void		do_process(t_env *env);
void		process_value(t_env *env);
void		process_tilde(t_env *env);
void		process_back_quotes(t_env *env);

void		do_simplify(t_env *env);

#endif
