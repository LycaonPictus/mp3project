#include <mp3tag.h>

u_int32_t	get_tag_size(char tag_header[4])
{
	u_int32_t	size;
	int			i;

	size = 0;
	i = 0;
	do
	{
		size *= 128;
		if (tag_header[i] < 0)
		{
			write(2, "Tag size error.\n", 16);
			return (0);
		}
		size += tag_header[i];
	} while (++i < 4);
	return (size);
}

t_mp3tag	*get_tag(int fd)
{
	int			bytes_read;
	char		buffer[10];
	t_mp3tag	*tag;

	bytes_read = read(fd, buffer, 10);
	if (bytes_read == -1)
	{
		write(2, "Read error.\n", 12);
		return (NULL);
	}
	if (strncmp(buffer, "ID3", (size_t) 3))
	{
		write(2, "No tag found.\n", 14);
		return (NULL);
	}
	tag = malloc(sizeof(t_mp3tag));
	tag->version[0] = buffer[3];
	tag->version[1] = buffer[4];
	tag->flags = buffer[5];
	tag->size = get_tag_size(&buffer[6]);
	//printf("Size is: %i\n", tag->size);
	if (tag->version[0] == 3 && tag->version[1] == 0)
		read_frames_v3(fd, tag->size);
	else
		write(1, "This is not an ID3v2.3.0 tag\n", 29);
	return (tag);
}
