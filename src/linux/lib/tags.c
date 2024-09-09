#include <id3tag.h>

/* Constructor functions */

static u_int32_t	get_tag_size(char tag_header[10])
{
	u_int32_t	size;
	int			i;

	size = 0;
	i = 6;
	do
	{
		size *= 128;
		if (tag_header[i] < 0)
		{
			write(2, "Tag size error.\n", 16);
			return (0);
		}
		size += tag_header[i];
	} while (++i < 10);
	return (size);
}

t_id3tag	*get_tag(int fd, char header[10])
{
	t_id3tag	*tag;

	tag = malloc(sizeof(t_id3tag));
	tag->padding_size = 0;
	tag->version[0] = header[3];
	tag->version[1] = header[4];
	tag->flags = header[5];
	tag->size = get_tag_size(header);
	if (tag->version[0] == 3 && tag->version[1] == 0)
		tag->frames = read_frames_v3(fd, tag->size, &tag->padding_size);
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

static int	write_tag_size(t_id3tag *tag, int fd)
{
	u_int32_t	size;
	char		buffer[4];
	int			i;

	size = tag->size;
	for (i = 3; i >= 0; i--)
	{
		buffer[i] = size % 128;
		size /= 128;
	}
	if (write (fd, buffer, 4) == -1)
		return (-1);
	return (4);
}

static int	write_padding(u_int32_t size, int fd)
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
	bytes_written = write(fd, "ID3", 3);
	if (bytes_written == -1)
		return (-1);
	total_bytes += bytes_written;
	bytes_written = write(fd, tag->version, 2);
	if (bytes_written == -1)
		return (-1);
	total_bytes += bytes_written;
	bytes_written = write(fd, &tag->flags, 1);
	if (bytes_written == -1)
		return (-1);
	total_bytes += bytes_written;
	bytes_written = write_tag_size(tag, fd);
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
