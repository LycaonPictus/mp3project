#include <id3tag.h>
#include <id3frame.h>
#include <uslt.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <id3frame_content.h>

t_uslt	*new_uslt()
{
	t_uslt	*uslt;

	uslt = malloc(sizeof(t_uslt));
	if (!uslt)
		return (NULL);
	uslt->text_encoding = 0x0;
	memcpy(uslt->language, "\0\0\0", 3);
	uslt->description = NULL;
	uslt->text_size = 0;
	uslt->text = NULL;
	return (uslt);
}

void	free_uslt(t_uslt **ptr)
{
	t_uslt	*uslt;

	uslt = *ptr;
	if (!uslt)
		return ;
	free(uslt->description);
	uslt->description = NULL;
	free(uslt->text);
	uslt->text = NULL;
	free(uslt);
	*ptr = NULL;
}

t_uslt	*frame_to_uslt(t_id3frame *frame)
{
	t_uslt		*uslt;
	char		*frame_content;
	uint32_t	i;

	if (frame->header.size < 5)
		return (NULL);
	uslt = new_uslt();
	if (!uslt)
		return (NULL);
	frame_content = frame->content;
	uslt->text_encoding = frame_content[0];
	memcpy(uslt->language, &frame_content[1], 3);
	i = 4;
	uslt->description = get_string(frame_content, &i, uslt->text_encoding);
	if (!uslt->description)
	{
		free_uslt(&uslt);
		return NULL;
	}
	uslt->text_size = frame->header.size - i;
	if (!uslt->text_size)
		return uslt;
	uslt->text = malloc(uslt->text_size);
	if (!uslt->text)
	{
		free_uslt(&uslt);
		return NULL;
	}
	memcpy(uslt->text, &frame_content[i], uslt->text_size);
	return (uslt);
}

t_id3frame	*uslt_to_frame(t_uslt *uslt)
{
	t_id3frame	*frame;
	char		*content;
	uint32_t	desc_size;
	uint32_t	i;
	uint32_t	total_size;

	if (!uslt)
		return (NULL);
	if (uslt->description)
		desc_size = strlen(uslt->description);
	else
		desc_size = 0;
	total_size = 5 + desc_size + uslt->text_size;
	if (uslt->text_encoding == 0x1)
		total_size++;
	content = malloc(total_size);
	if (!content)
		return (NULL);
	i = 0;
	content[i++] = uslt->text_encoding;
	memcpy(&content[i], uslt->language, 3);
	i += 3;
	if (uslt->description)
		memcpy(&content[i], uslt->description, desc_size);
	i += desc_size;
	content[i++] = '\0';
	if (uslt->text_encoding == 0x1)
		content[i] = '\0';
	write(1, uslt->text, uslt->text_size);
	memcpy(&content[i], uslt->text, uslt->text_size);
	frame = new_frame("USLT", content, total_size);
	if (!content)
	{
		free(content);
		return (NULL);
	}
	return frame;
}

t_uslt	*tag_to_uslt(t_id3tag *tag)
{
	t_id3framelist	*list;
	t_id3frame		*frame;

	if (!tag)
		return (NULL);
	list = filter_by_id(tag->frames, "USLT");
	if (!list)
		return (NULL);
	frame = list->frame;
	free_framelist(&list, 0);
	return frame_to_uslt(frame);
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
	size = strlen(uslt->description);
	write(fd, "Description: ", 13);
	write(fd, uslt->description, size);
	write(fd, "\n", 1);
	write(fd, uslt->text, uslt->text_size);
	write(fd, "\n", 1);
}

int	export_lyrics(t_uslt *uslt, char *filename)
{
	int	file_fd;
	int	bytes_written;

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

/* #include <id3tagged_file.h>

int main(int argc, char **argv)
{
	t_id3tagged_file	*file;
	char				*output_filename;
	t_uslt				*uslt;

	if (argc < 2)
	{
		write(2, "Usage: ", 7);
		write(2, argv[0], strlen(argv[0]));
		write(2, " [input file] <[output file]>\n", 30);
		return (1);
	}
	file = get_tagged_file(argv[1]);
	if (!file)
		return (1);
	if (argc > 2)
		output_filename = argv[2];
	else
		output_filename = NULL;
	
	uslt = tag_to_uslt(file->tag);
	if (!uslt)
		return (0);
	export_lyrics(uslt, output_filename);
	free_uslt(&uslt);
	free_tagged_file(&file);
	return (0);
}
 */