#include <id3tagged_file.h>

void	get_content(t_id3tagged_file *tf, int fd)
{
	char		buffer[BUFFER_SIZE];
	int			bytes_read;
	char		*content;
	char		*aux;
	u_int32_t	size;

	size = 0;
	bytes_read = 0;
	content = NULL;
	aux = NULL;
	do
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
			break ;
		if (bytes_read == 0)
		{
			tf->content_size = size;
			tf->content = content;
		}
		aux = malloc(size + bytes_read);
		if (!aux)
			break ;
		memcpy(aux, content, size);
		memcpy(&aux[size], buffer, bytes_read);
		size += bytes_read;
		free(content);
		content = aux;
	} while (bytes_read > 0);
	free(content);
	tf->content = NULL;
}

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
	tf->tag = get_tag(fd);
	get_content(tf, fd);
	close(fd);
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
	free(tf->content);
	tf->content = NULL;
	free(tf);
	*ptr = NULL;
}

void	write_rem(t_id3tagged_file *tf, int fd_out)
{
	int			bytes_written;
	u_int32_t	size;
	u_int32_t	bytes_to_write;

	if (!tf)
		return ;
	size = tf->content_size;
	while (size)
	{
		if (size > BUFFER_SIZE)
			bytes_to_write = BUFFER_SIZE;
		else
			bytes_to_write = size;
		bytes_written = write(fd_out, tf->content, bytes_to_write);
		size -= bytes_to_write;
		if (bytes_written == -1)
		{
			write(2, "Error. Content not fully copied.\n", 33);
			return ;
		}
	}
}
