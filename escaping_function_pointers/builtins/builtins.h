#ifndef BUILTINS_H
# define BUILTINS_H

#include <shell.h>

char			*get_env_name(const char *line);
int				env_name_match(char *cmp, char *tab);

void			builtin_cd(t_env *env);
void			builtin_env(t_env *env);
void			builtin_exit(t_env *env);
void			builtin_setenv(t_env *env);
void			builtin_unsetenv(t_env *env);

#endif