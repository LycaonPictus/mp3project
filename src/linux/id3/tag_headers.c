#include <id3tag_header.h>
#include <unistd.h>

static uint32_t	get_tag_size(char tag_header[10])
{
	uint32_t	size;
	int			i;

	size = 0;
	i = 6;
	while (i < 10)
	{
		size *= 128;
		if (tag_header[i] < 0)
		{
			write(2, "Tag size error.\n", 16);
			return (0);
		}
		size += tag_header[i++];
	}
	return (size);
}

t_id3tag_header	get_tag_header(char buffer[10])
{
	t_id3tag_header	header;
	
	header.version[0] = buffer[3];
	header.version[1] = buffer[4];
	header.flags = buffer[5];
	header.size = get_tag_size(buffer);
	return header;
}

static int	write_tag_size(t_id3tag_header header, int fd)
{
	uint32_t	size;
	char		buffer[4];
	int			i;

	size = header.size;
	for (i = 3; i >= 0; i--)
	{
		buffer[i] = size % 128;
		size /= 128;
	}
	if (write (fd, buffer, 4) == -1)
		return (-1);
	return (4);
}

int	write_tag_header(t_id3tag_header header, int fd)
{
	int	total_bytes;
	int	bytes_written;

	total_bytes = 0;
	bytes_written = write(fd, "ID3", 3);
	if (bytes_written == -1)
		return (-1);
	total_bytes += bytes_written;
	bytes_written = write(fd, header.version, 2);
	if (bytes_written == -1)
		return (-1);
	total_bytes += bytes_written;
	bytes_written = write(fd, &header.flags, 1);
	if (bytes_written == -1)
		return (-1);
	total_bytes += bytes_written;
	bytes_written = write_tag_size(header, fd);
	if (bytes_written == -1)
		return (-1);
	return (total_bytes);
}
