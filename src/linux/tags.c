#include <mp3tag.h>

int	read_frames_v3(int fd, int size);

int	get_tag_size(char array[4])
{
	int	size;
	int	i;

	size = 0;
	i = 0;
	do
	{
		size *= 128;
		if (array[i] < 0)
		{
			write(2, "Tag size error.\n", 16);
			return (0);
		}
		size += array[i];
	} while (++i < 4);
	return (size);
}

int	read_tag(int fd)
{
	int			bytes_read;
	char		buffer[10];
	int			i;
	t_mp3tag	*tag;

	bytes_read = read(fd, buffer, 10);
	if (strncmp(buffer, "ID3", (long unsigned int)3))
		write(2, "No tag found.\n", 14);
	tag = malloc(sizeof(t_mp3tag));
	tag->version[0] = buffer[3];
	tag->version[1] = buffer[4];
	tag->flags = buffer[5];
	tag->size = get_tag_size(&buffer[6]);
	printf("Size is: %i\n", tag->size);
	if (tag->version[0] == 3 && tag->version[1] == 0)
		read_frames_v3(fd, tag->size);
	else
		write(1, "This is not an ID3v2.3 tag\n", 27);
	free(tag);
}