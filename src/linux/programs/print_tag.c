#include <id3tagged_file.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>

static int	correct_args(int argc, char **argv)
{
	if (argc < 2)
	{
		write(2, argv[0], strlen(argv[0]));
		write(2, ": not enough arguments.\n", 24);
		return (0);
	}
	return (1);
}

static int	exists(char *file_name, char *prog_name)
{
	if (access(file_name, F_OK))
	{
		write(2, prog_name, strlen(prog_name));
		write(2, ": ", 2);
		write(2, file_name, strlen(file_name));
		write(2, ": not such file or directory.\n", 30);
		return (0);
	}
	return (1);
}

static int	readable(char *file_name, char *prog_name)
{
	if (access(file_name, R_OK))
	{
		write(2, prog_name, strlen(prog_name));
		write(2, ": ", 2);
		write(2, file_name, strlen(file_name));
		write(2, ": cannot read file.\n", 20);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	DIR 				*dir;
	struct dirent		*ent;
	t_id3tagged_file	*tf;
	int					result;
	
	if (!correct_args(argc, argv) || !exists(argv[1], argv[0]) || !readable(argv[1], argv[0]))
		return (1);
	dir = opendir(argv[1]);
	if (dir)
	{
		do
		{
			ent = readdir(dir);
			if (ent)
				printf("%s\n", ent->d_name);
		} while (ent);
		closedir(dir);
	}
	else
	{
		tf = get_tagged_file(argv[1]);
		if (!tf)
		{
			write(2, "Error\n", 6);
			return (1);
		}
		result = write_tag(tf->tag, 1);
		free_tagged_file(&tf);
		if (result)
			write(2, "Error\n", 6);
		return (result);
	}
	return (0);
}