#include <id3tag.h>
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
 /* */
static int	read_header(int fd_mp3, char header[10])
{
	int			bytes_read;

	bytes_read = read(fd_mp3, header, 10);
	if (bytes_read < 10)
	{
		write(2, "Error. Cannot read tag header.\n", 31);
		return (1);
	}
	return (0);
}

static int	read_body(int fd_mp3, char **body, u_int32_t size)
{
	int			bytes_read;

	*body = malloc(size);
	bytes_read = read(fd_mp3, *body, size);
	if (bytes_read < (int)size)
	{
		write (2, "Error. Cannot read tag.\n", 24);
		return (1);
	}
	return (0);
}

void	write_tag(int fd_in, int fd_out)
{
	char		header[10];
	char		*body;
	u_int32_t	size;

	read_header(fd_in, header);
	size = get_tag_size(header);
	read_body(fd_in, &body, size);
	write(fd_out, header, 10);
	write(fd_out, body, size);
	free(body);
}

int	main(int argc, char **argv)
{
	DIR 			*dir;
	struct dirent	*ent;
	int				fd_in;
	
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
		fd_in = open(argv[1], O_RDONLY);
		if (fd_in == -1)
			return (1);
		write_tag(fd_in, 1);
		close(fd_in);
	}
	return (0);
}