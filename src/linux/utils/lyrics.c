#include <id3tag.h>
#include <fcntl.h>
#include <unistd.h>

void	print_lyrics(t_id3tag *tag, int fd)
{
	t_id3framelist	*node;
	uint32_t		size;
	uint32_t		desc_size;
	char			*content;

	if (!tag || !tag->frames)
		return ;
	node = tag->frames;
	while (node)
	{
		if (has_tag_id(node->frame, "USLT"))
		{
			size = node->frame->header.size;
			content = node->frame->content;
			if (size < 5 || !content)
				return ;
			if (content[0] == 0x0)
				write(fd, "ISO-8859-1\n", 11);
			else if (content[0] == 0x1)
				write(fd, "UTF-16 with BOM\n", 16);
			else
				write(fd, "(Invalid codification)\n", 23);
			write(fd, "Language: ", 10);
			write(fd, &content[1], 3);
			write(fd, "\n", 1);
			desc_size = 0;
			while (content[desc_size + 4])
				desc_size++;
			write(fd, "Description: ", 13);
			write(fd, &content[4], desc_size);
			write(fd, "\n", 1);
			write(fd, &content[4 + desc_size + 1], size - 4 - desc_size - 1);
			write(fd, "\n", 1);
		}
		node = node->next;
	}
}

int	export_lyrics(t_id3tag *tag, char *filename)
{
	t_id3framelist	*node;
	int				file_fd;

	if (!tag || !tag->frames)
		return (0);
	if (!filename)
		file_fd = 1;
	else
		file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (file_fd == -1)
		return (1);
	close(file_fd);
	return (0);
}
#include <stdlib.h>
#include <string.h>
int	import_lyrics(t_id3tag *tag, char *lyrics_file)
{
	int			fd;
	t_id3frame	*frame;
	char		*content;

	content = malloc(5 + 4);
	if (!content)
		return (1);
	content[0] = 0x0;
	memcpy(&content[1], "spa", 3);
	content[4] = 0x0;
	memcpy(&content[5], "Hola", 4);
	frame = new_frame("USLT", content, 5 + 4);
	if (!frame)
	{
		free(content);
		return (1);
	}
	add_frame_last(&tag->frames, frame);
	return (0);
}

#include <id3tagged_file.h>
#include <stdio.h>
int main()
{
	t_id3tagged_file	*file;
	char				*filename = "example/Ed Sheeran - Thinking out loud.mp3";

	file = get_tagged_file(filename);
	if (!file)
		return (1);
	for (t_id3framelist *node = file->tag->frames; node; node = node->next)
		write(1, node->frame->header.frameID, 4);
	if (import_lyrics(file->tag, filename))
		return (1);
	int	out_fd;
	out_fd = open("test.mp3", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (out_fd != -1)
		write_file(file, out_fd);
	close(out_fd);
	free_tagged_file(&file);
	return (0);
}
