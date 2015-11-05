#include <shell.h>
#include <interprete.h>
#include <string.h>
#include <unistd.h>

void		launch_command(t_env *env)
{
	const char		fork_error[] = SHELL_NAME": fork error\n";
	const char		execve_error[] = SHELL_NAME": execve error\n";
	pid_t			child;
	int				stat_loc;

	if (launch_interprete(env) != CONTINUE)
		return ;
	while (42)
	{
		memcpy(env->argv_tmp, env->argv, sizeof(env->argv_tmp));
		child = fork();
		if (child == -1)
		{
			write(1, fork_error, sizeof(fork_error));
			return ;
		}
		else if (child != 0)
		{
			waitpid(child, &stat_loc, 0);
			if (launch_interprete(env) != CONTINUE)
				return ;
		}
		else
		{
			if (env->argv_tmp[0] && execve(env->argv_tmp[0], env->argv_tmp, NULL) == -1) //GOTTA ADD ENV
			{
				write(1, execve_error, sizeof(execve_error));
				return ;
			}
		}
	}
}