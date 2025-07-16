#include <id3tag.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <id3frame_content.h>

typedef struct s_album_pic
{
	char		text_encoding;
	char		*mime_type;
	char		picture_type;
	char		*description;
	char		*binary_data;
	uint32_t	data_size;
}	t_album_pic;

static void	frame_to_pic(t_id3frame *frame, t_album_pic **pic_ptr)
{
	char		*content;
	uint32_t	i;

	content = frame->content;
	(*pic_ptr)->text_encoding = content[0];
	i = 1;
	(*pic_ptr)->mime_type = get_string(content, &i, 0x0);
	if (!(*pic_ptr)->mime_type)
	{
		free(*pic_ptr);
		*pic_ptr = NULL;
		return ;
	}
	(*pic_ptr)->picture_type = content[i++];
	(*pic_ptr)->description = get_string(content, &i, (*pic_ptr)->text_encoding);
	if (!(*pic_ptr)->description)
	{
		free((*pic_ptr)->mime_type);
		free(*pic_ptr);
		*pic_ptr = NULL;
		return ;
	}
	(*pic_ptr)->data_size = frame->header.size - i;
	(*pic_ptr)->binary_data = NULL;
	if (!(*pic_ptr)->data_size)
		return ;
	(*pic_ptr)->binary_data = malloc((*pic_ptr)->data_size);
	if (!(*pic_ptr)->binary_data)
	{
		free((*pic_ptr)->mime_type);
		free((*pic_ptr)->description);
		free(*pic_ptr);
		*pic_ptr = NULL;
		return ;
	}
	memcpy((*pic_ptr)->binary_data, &content[i], (*pic_ptr)->data_size);
}

t_album_pic	*get_album_pic(t_id3tag *tag)
{
	t_album_pic		*pic;
	t_id3framelist	*pics;

	if (!tag || !tag->frames)
		return (NULL);
	pics = filter_by_id(tag->frames, "APIC");
	if (!pics)
		return (NULL);
	pic = malloc(sizeof(t_album_pic));
	if (!pic)
		return (NULL);
	frame_to_pic(pics->frame, &pic);
	return (pic);
}

int	export_album_picture(t_id3tag *tag, char *filename)
{
	t_album_pic	*picture;
	int			file_fd;

	if (!tag || !tag->frames)
		return (0);
	if (!filename)
		file_fd = 1;
	else
		file_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (file_fd == -1)
		return (1);
	picture = get_album_pic(tag);
	if (!picture)
		return (0);
	write(file_fd, picture->binary_data, picture->data_size);
	close(file_fd);
	return (0);
}
/* 
#include <id3tagged_file.h>
#include <stdio.h>
int main()
{
	t_id3tagged_file	*file;
	char				*filename = "example/Ed Sheeran - Thinking out loud.mp3";

	file = get_tagged_file(filename);
	if (!file)
		return (1);
	if (export_album_picture(file->tag, "test.jpg"))
		return (1);
	free_tagged_file(&file);
	return (0);
}
 */