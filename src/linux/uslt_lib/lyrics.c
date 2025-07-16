#include <id3tag.h>
#include <id3frame_content.h>
#include <uslt.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

t_uslt	*new_uslt()
{
	t_uslt	*lyrics;

	lyrics = malloc(sizeof(t_uslt));
	if (!lyrics)
		return (NULL);
	lyrics->text_encoding = 0x0;
	memcpy(lyrics->language, "\0\0\0", 3);
	lyrics->description = NULL;
	lyrics->text_size = 0;
	lyrics->text = NULL;
	return (lyrics);
}

void	free_uslt(t_uslt **ptr)
{
	t_uslt	*uslt;

	uslt = *ptr;
	if (!uslt)
		return ;
	free(uslt->description);
	free(uslt->text);
	free(uslt);
	*ptr = NULL;
}

static void	tag_to_uslt(t_id3tag *tag, t_uslt **uslt_ptr)
{
	t_id3framelist	*list;
	t_id3frame		*frame;
	char			*content;
	uint32_t		i;

	list = filter_by_id(tag->frames, "USLT");
	if (!list)
		return ;
	frame = list->frame;
	free_framelist(&list, 0);
	if (frame->header.size < 5)
		return free_uslt(uslt_ptr);
	content = frame->content;
	(*uslt_ptr)->text_encoding = content[0];
	memcpy((*uslt_ptr)->language, &content[1], 3);
	i = 4;
	(*uslt_ptr)->description = get_string(content, &i, (*uslt_ptr)->text_encoding);
	if (!(*uslt_ptr)->description)
		return free_uslt(uslt_ptr);
	(*uslt_ptr)->text_size = frame->header.size - i;
	if (!(*uslt_ptr)->text_size)
		return ;
	(*uslt_ptr)->text = malloc((*uslt_ptr)->text_size);
	if (!(*uslt_ptr)->text)
		return free_uslt(uslt_ptr);
	memcpy((*uslt_ptr)->text, &content[i], (*uslt_ptr)->text_size);
}

t_uslt	*get_uslt(t_id3tag *tag)
{
	t_uslt	*uslt;

	if (!tag)
		return (NULL);
	uslt = new_uslt();
	if (!uslt)
		return (NULL);
	tag_to_uslt(tag, &uslt);
	return (uslt);
}

void	print_lyrics(t_uslt *uslt, int fd)
{
	uint32_t	size;

	if (!uslt)
		return ;
	if (uslt->text_encoding == 0x0)
		write(fd, "ISO-8859-1\n", 11);
	else if (uslt->text_encoding == 0x1)
		write(fd, "UTF-16 with BOM\n", 16);
	else
		write(fd, "(Invalid codification)\n", 23);
	write(fd, "Language: ", 10);
	write(fd, uslt->language, 3);
	write(fd, "\n", 1);
	size = 0;
	
	while (uslt->description[size])
		size++;
	write(fd, "Description: ", 13);
	write(fd, uslt->description, size);
	write(fd, "\n", 1);
	write(fd, uslt->text, uslt->text_size);
	write(fd, "\n", 1);
}

int	export_lyrics(t_uslt *uslt, char *filename)
{
	int				file_fd;
	int				bytes_written;

	if (!uslt)
		return (1);
	if (!filename)
		file_fd = 1;
	else
		file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (file_fd == -1)
		return (1);
	bytes_written = write(file_fd, uslt->text, uslt->text_size);
	close(file_fd);
	if (bytes_written == -1)
		return (1);
	return (0);
}

/* int	import_lyrics(t_id3tag *tag, char *lyrics_file)
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
} */

#include <id3tagged_file.h>

int main()
{
	t_id3tagged_file	*file;
	char				*filename = "example/Ed Sheeran - Thinking out loud.mp3";
	t_uslt				*uslt;

	file = get_tagged_file(filename);
	if (!file)
		return (1);
	uslt = get_uslt(file->tag);
	export_lyrics(uslt, "test.txt");
	free_uslt(&uslt);
	free_tagged_file(&file);
	return (0);
}
