#include <shell.h>
#include <builtins.h>

void			init_builtins(hashtable_t *table)
{
	ht_set(table, "cd", &builtin_cd);
	ht_set(table, "env", &builtin_env);
	ht_set(table, "setenv", &builtin_setenv);
	ht_set(table, "unsetenv", &builtin_unsetenv);
	ht_set(table, "exit", &builtin_exit);
}