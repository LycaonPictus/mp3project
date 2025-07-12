#include <id3frame.h>

#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

static int	is_frame_id(char id[4])
{
	int	i;
	for (i = 0; i < 4; i++)
	{
		if (!isalnum(id[i]) || islower(id[i]))
			return (0);
	}
	return (1);
}

static int	is_padding(char *buffer, int bytes_read)
{
	int	i;

	if (bytes_read == -1)
		return (0);
	for (i = 0; i < bytes_read; i++)
		if (buffer[i] != '\0')
			return (0);
	return (1);
}

static int	no_more_frames(int fd, uint32_t *rem, uint32_t *padding, char * header)
{
	int			bytes_read;
	char		*garbage;

	if (*rem < 10)
	{
		bytes_read = read(fd, header, *rem);
		if (is_padding(header, bytes_read))
			*padding = *rem;
		*rem = 0;
		return (1);
	}
	bytes_read = read(fd, header, 10);
	if (bytes_read == -1)
	{
		write(2, "Read error.\n", 12);
		return (0);
	}
	*rem -= 10;
	if (!is_frame_id(header))
	{
		if (is_padding(header, bytes_read))
			*padding = *rem + 10;
		garbage = malloc(*rem);
		bytes_read = read(fd, garbage, *rem);
		*rem = 0;
		free(garbage);
		return (1);
	}
	return (0);
}
#include <stdio.h>
t_id3frame	*get_frame(int fd, uint32_t *rem, uint32_t *padding)
{
	t_id3frame	*frame;
	int			bytes_read;
	char		header[10];
	char		*garbage;

	if (no_more_frames(fd, rem, padding, header))
		return (NULL);
	frame = malloc(sizeof(t_id3frame));
	if (!frame)
		return (NULL);
	frame->content = NULL;
	frame->header = get_frame_header(header);
	if (*rem < frame->header.size)
	{
		write(2, "Error. Frame too big.\n", 22);
		garbage = malloc(*rem);
		read(fd, garbage, *rem);
		free(garbage);
		*rem = 0;
		free_frame(&frame);
		return (NULL);
	}
	frame->content = malloc(frame->header.size);
	bytes_read = read(fd, frame->content, frame->header.size);
	*rem -= frame->header.size;
	if (bytes_read == -1)
	{
		write(2, "Read error.\n", 12);
		free_frame(&frame);
		return (NULL);
	}

	printf("%c%c%c%c (%i):\n", frame->header.frameID[0], frame->header.frameID[1], frame->header.frameID[2], frame->header.frameID[3], frame->header.id_int);
	if (frame->header.frameID[0] == 'T')
	switch (frame->content[0])
	{
		case 0:
			printf("(ISO-8859-1)");
			break;
		case 1:
			printf("(UTF-16 con BOM)");
			break;
		case 2:
			printf("(UTF-16BE sin BOM)");
			break;
		default:
			printf("(??\?)");
	}
	if (frame->header.size < 300)
	for (uint32_t i = 0; i < frame->header.size; i++)
		printf("%c", frame->content[i]);
	printf("\n");

	return (frame);
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

int	write_frame(t_id3frame *frame, int fd)
{
	int			bytes_written;

	if (!frame)
		return (1);
	if (write_frame_header(frame->header, fd))
		return (1);
	bytes_written = write(fd, frame->content, frame->header.size);
	if (bytes_written == -1)
	{
		write(2, "Cannot write in given file descriptor.\n", 39);
		return (1);
	}
	return (0);
}

void	set_content(t_id3frame *frame, char *content, uint32_t size)
{
	if (!content)
		size = 0;
	free(frame->content);
	frame->content = content;
	frame->header.size = size;
}
