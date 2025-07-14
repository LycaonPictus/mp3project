#include <id3tagged_file.h>
#include <dirent.h>

static void	print_usage(char *progname)
{
	write(2, "usage: ", 7);
	write(2, progname, strlen(progname));
	write(2, " input_file [output_file]\n", 26);
	write(2, "usage: ", 7);
	write(2, progname, strlen(progname));
	write(2, " input_folder output_folder\n", 28);
}

static int	export_from_file(char *filename, char *out)
{
	t_id3tagged_file	*file;
	int					fd_out;
	int					result;

	file = get_tagged_file(filename);
	if (!file)
		return (1);
	fd_out = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd_out == -1)
	{
		free_tagged_file(&file);
		return (1);
	}
	result = write_tag(file->tag, fd_out);
	close(fd_out);
	free_tagged_file(&file);
	return (result);
}

int	main(int argc, char **argv)
{
	DIR					*dir1;
	DIR					*dir2;
	char				*out;
	
	if (argc < 2 || argc > 3)
	{
		print_usage(argv[0]);
		return (1);
	}
	dir1 = opendir(argv[1]);
	if (dir1)
	{
		if (argc == 2)
		{
			print_usage(argv[0]);
			closedir(dir1);
			return (1);
		}
		dir2 = opendir(argv[2]);
		if (!dir2)
		{
			print_usage(argv[0]);
			closedir(dir1);
			return (1);
		}
	}
	else
	{
		if (argc == 2)
			out = "example.id3dump";
		else
			out = argv[2];
		return (export_from_file(argv[1], out));
	}
	return (0);
}
