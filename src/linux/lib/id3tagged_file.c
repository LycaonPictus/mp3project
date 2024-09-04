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
		write(2, "\".\n", 3);
		return (NULL);
	}
	tf = malloc(sizeof(t_id3tagged_file));
	if (!tf)
	{
		write(2, "get_tagged_file: No memory available\n", 37);
		return (NULL);
	}
	tf->name = strdup(file_name);
	tf->fd = fd;
	tf->tag = get_tag(fd);
	return (tf);
}

void	free_tagged_file(t_id3tagged_file **ptr)
{
	t_id3tagged_file *tf;

	tf = *ptr;
	if (!tf)
		return ;
	free_tag(&tf->tag);
	free(tf->name);
	tf->name = NULL;
	close(tf->fd);
	free(tf);
	*ptr = NULL;
}

int	write_rem(t_id3tagged_file *tf, int fd_out)
{
	int		bytes_written;
	int		bytes_read;
	char	buffer[BUFFER_SIZE];

	if (!tf)
		return (1);
	bytes_read = 0;
	do
	{
		bytes_written = write(fd_out, buffer, bytes_read);
		if (bytes_written == -1)
		{
			write(2, "Error writing the content.\n", 27);
			return (1);			
		}
		bytes_read = read(tf->fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			write(2, "Error reading the content.\n", 27);
			return (1);
		}
	} while (bytes_read);
	return (0);
}

int	write_file(t_id3tagged_file *tf, int fd)
{
	return (write_tag(tf->tag, fd) || write_rem(tf, fd));
}
