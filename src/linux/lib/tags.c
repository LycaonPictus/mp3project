#include <id3tag.h>

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
	free_framelist(&tag->frames);
	free(tag);
	*ptr = NULL;
}

/* Writing functions */

static int	write_padding(uint32_t size, int fd)
{
	int	total_bytes;
	int	bytes_written;

	total_bytes = 0;
	while (size)
	{
		bytes_written = write(fd, "\0", 1);
		if (bytes_written == -1)
		{
			write(2, "Error writing tag padding.\n", 27);
			return (-1);
		}
		size--;
		total_bytes++;
	}
	return (bytes_written);
}

int	write_tag(t_id3tag *tag, int fd)
{
	int	total_bytes;
	int	bytes_written;

	if (!tag)
		return (0);
	total_bytes = 0;
	bytes_written = write_tag_header(tag->header, fd);
	if (bytes_written == -1)
		return (-1);
	total_bytes += bytes_written;
	bytes_written = write_frames(tag->frames, fd);
	if (bytes_written == -1)
		return (-1);
	total_bytes += bytes_written;
	bytes_written = write_padding(tag->padding_size, fd);
	if (bytes_written == -1)
		return (-1);
	total_bytes += bytes_written;
	return (total_bytes);
}
