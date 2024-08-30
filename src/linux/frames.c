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

u_int32_t	get_frame_size(char array[4])
{
	u_int32_t	size;
	int	i;

	size = 0;
	i = 0;
	do
	{
		size *= 256;
		size += (unsigned char)array[i];
	} while (++i < 4);
	return (size);
}

t_id3frame	*get_frame(int fd, u_int32_t *rem)
{
	int			bytes_read;
	t_id3frame	*frame;
	char		header[10];
	char		*content;

	if (*rem < 10)
	{
		bytes_read = read(fd, header, *rem);
		*rem = 0;
		return (NULL);
	}
	bytes_read = read(fd, header, 10);
	*rem -= 10;
	if (!is_frame_id(header))
	{
		content = malloc(*rem);
		bytes_read = read(fd, content, *rem);
		*rem = 0;
		free(content);
		return (NULL);
	}
	frame = malloc(sizeof(t_id3frame));
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
		return (NULL);
	}
	frame->content = malloc(frame->size);
	bytes_read = read(fd, frame->content, frame->size);
	*rem -= frame->size;
	if (bytes_read == -1)
	{
		write(1, "Read error.\n", 12);
		return (NULL);
	}
	write(1, header, 4);
	write(1, ": ", 2);
	if (strncmp(header, "APIC", 4))
		write(1, frame->content, frame->size);
	else
		write(1, "(binary)", 8);
	write(1, "\n", 1);
	return (frame);
}

t_id3frame	*read_frames_v3(int fd, u_int32_t size)
{
	t_id3frame	*res;
	t_id3frame	*list;
	t_id3frame	*last;

	last = NULL;
	do
	{
		res = get_frame(fd, &size);
		if (!last)
		{
			list = res;
			last = list;
		}
		else
		{
			last->next = res;
			last = res;
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

void	free_frames(t_id3frame **ptr)
{
	t_id3frame *frame;
	t_id3frame *next;

	frame = *ptr;
	while (frame)
	{
		next = frame->next;
		free_frame(&frame);
		frame = next;
	}
}

void	del_frame(t_id3frame **ptr, unsigned int index)
{
	t_id3frame		*frame;
	unsigned int	cur_index;
	t_id3frame		*prev;

	cur_index = 0;
	prev = NULL;
	frame = *ptr;
	while (frame && cur_index < index)
	{
		prev = frame;
		frame = frame->next;
		cur_index++;
	}
	if (!frame)
		return ;
	if (prev)
		prev->next = frame->next;
	else
		*ptr = frame->next;
	free_frame(&frame);
}
