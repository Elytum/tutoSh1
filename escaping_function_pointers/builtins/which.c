#include <shell.h>
#include <string.h>
#include <unistd.h>

/*
** GOTTA ADD ALIAS HANDLING
*/

void			builtin_which(t_env *env)
{
	char		*str;
	const char	builtin[] = ": shell built-in command\n";
	const char	not_found[] = " not found\n";

	if (env->argc <= 1)
		return ;
	if (ht_get(env->builtins, env->argv[1]))
	{
		write(1, env->argv[1], strlen(env->argv[1]));
		write(1, builtin, sizeof(builtin) - 1);

	}
	else if ((str = ht_get(env->binaries, env->argv[1])))
	{
		write(1, str, strlen(str));
		write(1, "\n", 1);
	}
	else
	{
		write(1, env->argv[1], strlen(env->argv[1]));
		write(1, not_found, sizeof(not_found) - 1);
	}
}