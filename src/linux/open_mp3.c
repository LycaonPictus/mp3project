#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <mp3file.h>
#include <ctype.h>
#define BUFFER_SIZE 1024
#include <stdio.h>

int	read_fd(int fd)
{
	int		bytes_read;
	int		total = 0;
	char	buffer[BUFFER_SIZE];

	bytes_read = 0;
	read_tag(fd);
	write(1, "Reading audio section...\n", 25);
	do
	{
		if (0)
			write(1, buffer, bytes_read);
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		total += bytes_read;
	}
	while (bytes_read > 0);
	write(1, "EOF\n", 4);
	close(fd);
	if (bytes_read)
	{
		write(2, "undefined error\n", 16);
		return (1);
	}
	return (0);
}

int	read_file(char *file, char *prog_name)
{
	int		fd;

	if (access(file, F_OK) || access(file, R_OK))
	{
		write(2, prog_name, strlen(prog_name));
		write(2, ": ", 2);
		write(2, file, strlen(file));
		write(2, ": ", 2);
		if (access(file, F_OK))
			write(2, "No such file or directory\n", 26);
		else
			write(2, "Permission denied\n", 18);
		return (1);
	}
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		write(2, prog_name, strlen(prog_name));
		write(2, ": ", 2);
		write(2, file, strlen(file));
		write(2, ": ", 2);
		write(2, "Cannot open file\n", 17);
		return (1);
	}
	return (read_fd(fd));
}

int	main(int argc, char **argv)
{
	int	arg_index;

	if (argc == 1)
	{
		read_fd(0);
		return (0);
	}
	arg_index = 1;
	while (arg_index < argc)
		read_file(argv[arg_index++], argv[0]);
	return (0);
}
