#ifndef _ID3FRAME_H
#define _ID3FRAME_H

# include <stdint.h>

#include <id3frame_header.h>

typedef struct s_id3frame
{
	t_id3frame_header	header;
	char				*content;
}	t_id3frame;

t_id3frame	*new_frame(char id[4], char *content, uint32_t size);
t_id3frame	*get_frame(int fd, uint32_t *rem, uint32_t *padding);
void		free_frame(t_id3frame **ptr);
int			write_frame(t_id3frame *frame, int fd);
void		set_content(t_id3frame *frame, char *content, uint32_t size);
int			has_tag_id(t_id3frame *frame, char *id);

#endif