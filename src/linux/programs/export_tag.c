#include <id3tagged_file.h>
#include <dirent.h>

static void	print_usage(char *progname)
{
	write(2, "usage: ", 8);
	write(2, progname, strlen(progname));
	write(2, " input_file [output_file]\n", 27);
	write(2, "usage: ", 8);
	write(2, progname, strlen(progname));
	write(2, " input_folder output_folder\n", 29);
}

int	main(int argc, char **argv)
{
	t_id3tagged_file	*file;
	DIR					*dir1;
	DIR					*dir2;
	char				*out;
	int					fd_out;

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
		write(1, "OK!\n", 4);
	}
	else
	{
		file = get_tagged_file(argv[1]);
		if (!file)
			return (1);
		if (argc == 2)
			out = "example.tag";
		else
			out = argv[2];
		fd_out = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		write_tag(file->tag, fd_out);
		close(fd_out);
		free_tagged_file(&file);
	}
	return (0);
}
