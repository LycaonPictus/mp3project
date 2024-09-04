#include <id3tag.h>

u_int32_t	get_tag_size(char tag_header[10])
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
#include <stdio.h>
t_id3tag	*get_tag(int fd)
{
	int			bytes_read;
	char		header[10];
	t_id3tag	*tag;

	bytes_read = read(fd, header, 10);
	if (bytes_read == -1)
	{
		write(2, "Read error.\n", 12);
		return (NULL);
	}
	if (strncmp(header, "ID3", 3))
	{
		write(2, "No tag found.\n", 14);
		return (NULL);
	}
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

void	write_tag_size(t_id3tag *tag, int fd)
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
	write (fd, buffer, 4);
}

int	write_tag(t_id3tag *tag, int fd)
{
	t_id3framelist	*fl;
	u_int32_t		padding;
	int				bytes_read;

	write(fd, "ID3", 3);
	write(fd, tag->version, 2);
	write(fd, &tag->flags, 1);
	write_tag_size(tag, fd);
	fl = tag->frames;
	while (fl)
	{
		bytes_read = write_frame(fl->frame, fd);
		if (bytes_read == -1)
			return (1);
		fl = fl->next;
	}
	padding = tag->padding_size;
	while (padding)
	{
		if (write(fd, "\0", 1) == -1)
		{
			write(2, "Error writing tag padding.\n", 27);
			return (1);
		}
		padding--;
	}
	return (0);
}
