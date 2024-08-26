#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#include <stdio.h>
int	read_tag_v3(int fd, int size)
{
	int		bytes_read;
	char	*buffer;

	buffer = malloc(size + 1);
	buffer[size] = '\0';
	bytes_read = read(fd, buffer, size);
	printf("%i bytes read.\n", bytes_read);
	free(buffer);
}

int	get_tag_size(char array[4])
{
	int	size;
	int	i;

	size = 0;
	i = 0;
	do
	{
		size *= 128;
		if (array[i] < 0)
		{
			write(2, "Tag size error.\n", 16);
			return (0);
		}
		size += array[i];
	} while (++i < 4);
	return (size);
}

int	read_tag(int fd)
{
	int		bytes_read;
	char	buffer[10];
	int		size = 0;
	char	flags[2] = {0, 0};
	int		i;

	bytes_read = read(fd, buffer, 10);
	if (strncmp(buffer, "ID3", 3))
		write(2, "No tag found.\n", 14);
	size = get_tag_size(&buffer[6]);
	printf("Size is: %i\n", size);
	if (buffer[3] == 3 && buffer[4] == 0)
		read_tag_v3(fd, size);
	else
		write(1, "This is not an ID3v2.3 tag\n", 27);
}

int	read_fd(int fd)
{
	int		bytes_read;
	int		total = 0;
	char	buffer[BUFFER_SIZE];

	bytes_read = 0;
	read_tag(fd);
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
