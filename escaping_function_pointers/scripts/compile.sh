gcc *.c env_tools/*.c htable/*.c interpretation/*.c builtins/*.c execution/*.c -I./ -I./env_tools/ -I./htable/ -I./interpretation/ -I./builtins -g -Wall -Wextra -Werror -O3
