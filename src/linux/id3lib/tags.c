#include <id3tag.h>
#include <stdlib.h>
#include <unistd.h>

/* Constructor functions */

t_id3tag	*get_tag(int fd, char header[10])
{
	t_id3tag	*tag;

	tag = malloc(sizeof(t_id3tag));
	if (!tag)
		return (NULL);
	tag->padding_size = 0;
	tag->header = get_tag_header(header);
	if (tag->header.version[0] == 3 && tag->header.version[1] == 0)
		tag->frames = read_frames_v3(fd, tag->header.size, &tag->padding_size);
	else
		write(1, "This is not an ID3v2.3.0 tag\n", 29);
	return (tag);
}

/* Destructor functions */

void	free_tag(t_id3tag **ptr)
{
	t_id3tag	*tag;

	tag = *ptr;
	if (!tag)
		return ;
	free_framelist(&tag->frames, 1);
	free(tag);
	*ptr = NULL;
}

/* Writing functions */

static int	write_padding(uint32_t size, int fd)
{
	int	bytes_written;

	while (size)
	{
		bytes_written = write(fd, "\0", 1);
		if (bytes_written == -1)
		{
			write(2, "Error writing tag padding.\n", 27);
			return (1);
		}
		size--;
	}
	return (0);
}

static void	calculate_tag_size(t_id3tag *tag)
{
	t_id3framelist	*frame_node;
	uint32_t		size;

	size = 0;
	frame_node = tag->frames;
	while (frame_node)
	{
		if (frame_node->frame)
			size += 10 + frame_node->frame->header.size;
		frame_node = frame_node->next;
	}
	size += tag->padding_size;
	tag->header.size = size;
}

int	write_tag(t_id3tag *tag, int fd)
{
	if (!tag)
		return (0);
	calculate_tag_size(tag);
	return (write_tag_header(tag->header, fd)
			|| write_frames(tag->frames, fd)
			|| write_padding(tag->padding_size, fd));
}
