gcc *.c env_tools/*.c htable/*.c interpretation/*.c builtins/exit.c -I./ -I./env_tools/ -I./htable/ -I./interpretation/ -I./builtins -g -Wall -Wextra -Werror -O3
