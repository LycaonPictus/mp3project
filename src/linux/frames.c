#include <mp3frame.h>

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

t_mp3frame	*get_frame(int fd, int *rem)
{
	int			bytes_read;
	t_mp3frame	*frame;
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
		content = malloc(*rem + 1);
		bytes_read = read(fd, content, *rem);
		*rem = 0;
		free(content);
		return (NULL);
	}
	frame = malloc(sizeof(t_mp3frame));
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
	content = malloc(frame->size + 1);
	content[frame->size] = '\0';
	read(fd, content, frame->size);
	*rem -= frame->size;
	write(1, header, 4);
	write(1, ": ", 2);
	if (strncmp(header, "APIC", 4))
		write(1, content, frame->size);
	else
		write(1, "(binary)", 8);
	write(1, "\n", 1);
	free(content);
	return (frame);
}

int	read_frames_v3(int fd, int size)
{
	t_mp3frame	*res;
	res = (t_mp3frame *)0x1;
	while (size && res)
	{
		res = get_frame(fd, &size);
		free(res);
	}
}
