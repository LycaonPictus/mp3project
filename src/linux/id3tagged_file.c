#include <id3tagged_file.h>

t_id3tagged_file	*get_tagged_file(char *file_name)
{
	int					fd;
	t_id3tagged_file	*tf;

	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		write(2, "Cannot open file \"", 18);
		write(2, file_name, strlen(file_name));
		write(2, "\n", 1);
		return (NULL);
	}
	tf = malloc(sizeof(t_id3tagged_file));
	if (!tf)
	{
		write(2, "get_tagged_file: No memory available\n", 37);
		return (NULL);
	}
	tf->tag = get_tag(fd);
	tf->name = strdup(file_name);
	tf->fd = fd;
	return (tf);
}

void	free_tagged_file(t_id3tagged_file **ptr)
{
	t_id3tagged_file *tf;

	tf = *ptr;
	if (!tf)
		return ;
	close(tf->fd);
	free_tag(&tf->tag);
	free(tf->name);
	tf->name = NULL;
	free(tf);
	*ptr = NULL;
}

void	write_rem(t_id3tagged_file *tf, int fd_out)
{
	int		bytes_read;
	int		bytes_written;
	char	buffer[BUFFER_SIZE];

	if (!tf)
		return ;
	do
	{
		bytes_read = read(tf->fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			write(2, "Cannot read file \"", 18);
			write(2, tf->name, strlen(tf->name));
			write(2, "\".\n", 3);
			break ;
		}
		bytes_written = write(fd_out, buffer, bytes_read);
		if (bytes_written == -1)
		{
			write(2, "Cannot write in file descriptor.\n", 33);
			break ;
		}
	} while (bytes_read);
}
