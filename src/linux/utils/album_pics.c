#include <id3tag.h>
#include <fcntl.h>
#include <unistd.h>

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
	for (node = tag->frames; node; node = node->next)
		if (has_tag_id(node->frame, "APIC"))
			break;
	if (!node)
		return (0);
	char		*content;
	uint32_t	pre_size;
	pre_size = 1;
	content = node->frame->content;
	while (content[pre_size])
		pre_size++;
	pre_size += 2;
	while (content[pre_size])
		pre_size++;
	pre_size++;
	write(file_fd, &content[pre_size], node->frame->header.size - pre_size);
	close(file_fd);
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
	if (export_lyrics(file->tag, "test.jpg"))
		return (1);
	free_tagged_file(&file);
	return (0);
}
