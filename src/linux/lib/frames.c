#include <id3frame.h>

int	is_frame_id(char id[4])
{
	int	i;
	for (i = 0; i < 4; i++)
	{
		if (!isalnum(id[i]) || islower(id[i]))
			return (0);
	}
	return (1);
}

int	is_padding(char *buffer, int bytes_read)
{
	int	i;

	if (bytes_read == -1)
		return (0);
	for (i = 0; i < bytes_read; i++)
		if (buffer[i] != '\0')
			return (0);
	return (1);
}

u_int32_t	get_frame_size(char array[4])
{
	u_int32_t	size;
	int	i;

	size = 0;
	for (i = 0; i < 4; i++)
	{
		size *= 256;
		size += (unsigned char)array[i];
	}
	return (size);
}

void	encode_frame_size(u_int32_t size, char array[4])
{
	int			i;

	for (i = 3; i >= 0; i--)
	{
		array[i] = size % 256;
		size /= 256;
	}
}

t_id3frame	*new_frame(void)
{
	t_id3frame	*frame;

	frame = malloc(sizeof(t_id3frame));
	if (!frame)
		return (NULL);
	frame->content = NULL;
	frame->size = 0;
	frame->flags[0] = 0;
	frame->flags[1] = 0;
	frame->frameID[0] = 0;
	frame->frameID[1] = 0;
	frame->frameID[2] = 0;
	frame->frameID[3] = 0;
	return (frame);
}

t_id3frame	*get_frame(int fd, u_int32_t *rem, u_int32_t *padding)
{
	int			bytes_read;
	t_id3frame	*frame;
	char		header[10];
	char		*content;

	if (*rem < 10)
	{
		bytes_read = read(fd, header, *rem);
		if (is_padding(header, bytes_read))
			*padding = *rem;
		*rem = 0;
		return (NULL);
	}
	bytes_read = read(fd, header, 10);
	*rem -= 10;
	if (!is_frame_id(header))
	{
		if (is_padding(header, bytes_read))
			*padding = *rem + 10;
		content = malloc(*rem);
		bytes_read = read(fd, content, *rem);
		*rem = 0;
		free(content);
		return (NULL);
	}
	frame = new_frame();
	if (!frame)
		return (NULL);
	memcpy(frame->frameID, header, 4);
	frame->size = get_frame_size(&header[4]);
	frame->flags[0] = header[8];
	frame->flags[1] = header[9];
	//
	if (*rem < frame->size)
	{
		write(2, "Error. Frame too big.\n", 22);
		content = malloc(*rem);
		read(fd, content, *rem);
		free(content);
		*rem = 0;
		free_frame(&frame);
		return (NULL);
	}
	frame->content = malloc(frame->size);
	bytes_read = read(fd, frame->content, frame->size);
	*rem -= frame->size;
	if (bytes_read == -1)
	{
		write(2, "Read error.\n", 12);
		free_frame(&frame);
		return (NULL);
	}
	return (frame);
}

t_id3framelist	*read_frames_v3(int fd, u_int32_t size, u_int32_t *padding)
{
	t_id3frame		*frame;
	t_id3framelist	*list;
	t_id3framelist	*last;
	t_id3framelist	*new;

	list = NULL;
	last = NULL;
	do
	{
		frame = get_frame(fd, &size, padding);
		if (!frame)
			break ;
		new = malloc(sizeof(t_id3framelist));
		new->next = NULL;
		if (!new)
		{
			free_frame(&frame);
			free_framelist(&list);
			return (NULL);
		}
		new->frame = frame;
		if (!last)
		{
			list = new;
			last = new;
		}
		else
		{
			last->next = new;
			last = new;
		}
	}
	while (size);
	return (list);
}

void	free_frame(t_id3frame **ptr)
{
	t_id3frame *frame;

	frame = *ptr;
	if (!frame)
		return ;
	free(frame->content);
	free(frame);
	*ptr = NULL;
}

void	free_framelist(t_id3framelist **ptr)
{
	t_id3framelist *node;
	t_id3framelist *next;

	node = *ptr;
	while (node)
	{
		next = node->next;
		free_frame(&node->frame);
		free(node);
		node = next;
	}
}

void	del_frame(t_id3framelist **ptr, unsigned int index)
{
	t_id3framelist	*node;
	unsigned int	cur_index;
	t_id3framelist	*prev;

	cur_index = 0;
	prev = NULL;
	node = *ptr;
	while (node && cur_index < index)
	{
		prev = node;
		node = node->next;
		cur_index++;
	}
	if (!node)
		return ;
	if (prev)
		prev->next = node->next;
	else
		*ptr = node->next;
	free_frame(&node->frame);
	free(node);
}

int	write_frame(t_id3frame *frame, int fd)
{
	int			bytes_written;
	int			total_bytes;
	char		header[10];

	if (!frame)
		return (-1);
	memcpy(header, frame->frameID, 4);
	encode_frame_size(frame->size, &header[4]);
	header[8] = frame->flags[0];
	header[9] = frame->flags[1];
	bytes_written = write(fd, header, 10);
	total_bytes = bytes_written;
	if (bytes_written < 10)
	{
		write(2, "Cannot write in given file descriptor.\n", 39);
		return (total_bytes);
	}
	bytes_written = write(fd, frame->content, frame->size);
	if (bytes_written == -1)
	{
		write(2, "Cannot write in given file descriptor.\n", 39);
		return (total_bytes);
	}
	total_bytes += bytes_written;
	return (total_bytes);
}
