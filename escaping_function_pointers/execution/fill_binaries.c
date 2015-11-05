#include <htable.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

static void		file_fill(hashtable_t *binaries, char pwd[_POSIX_PATH_MAX],
					size_t len, char file[_POSIX_NAME_MAX])
{
	memcpy(pwd + len, file, _POSIX_NAME_MAX);
}

static void		file_link(hashtable_t *binaries, char pwd[_POSIX_PATH_MAX],
					size_t len, char file[_POSIX_NAME_MAX])
{
	memcpy(pwd + len, file, _POSIX_NAME_MAX);
}

static void		directory_fill(hashtable_t *binaries, char *directory, size_t len)
{
	const char		c = directory[len];
	char			pwd[_POSIX_PATH_MAX];
	DIR				*dir;
	struct dirent	*ent;

	if (len > _POSIX_PATH_MAX - _POSIX_NAME_MAX - 1)
		return ;
	directory[len] = '\0';
	if ((dir = opendir (directory)) != NULL)
	{
		memcpy(pwd, directory, len);
		pwd[len] = '/';
		while ((ent = readdir(dir)) != NULL) {
			if (ent.d_type & DT_REG)
				file_fill(binaries, pwd, len, file);
			else if (ent.d_type & DT_LNK)
				file_link(binaries, pwd, len, file);
			write(1, ent->d_name, strlen(ent->d_name));
			write(1, "\n", 1);
		}
		closedir (dir);
	}

	directory[len] = c;
}

void			fill_binaries(hashtable_t *binaries)
{
	char			path[] = "/nfs/zfs-student-3/users/2014/achazal/.brew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin:/usr/texbin";
	char			*old_ptr;
	char			*new_ptr;

	(void)binaries;
	old_ptr = (char *)path;
	while (*old_ptr)
	{
		new_ptr = old_ptr;
		while (*new_ptr && *new_ptr != ':')
			++new_ptr;
		directory_fill(binaries, old_ptr, new_ptr - old_ptr);
		if (!*new_ptr)
			return ;
		old_ptr = new_ptr + 1;
	}
}