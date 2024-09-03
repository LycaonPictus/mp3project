#ifndef _ID3TAG_H
#define _ID3TAG_H
# include <id3frame.h>

typedef struct s_id3tag
{
	char			version[2];
	u_char			flags;
	u_int32_t		size;
	t_id3framelist	*frames;
	u_int32_t		padding_size;

}	t_id3tag;

t_id3tag	*get_tag(int fd);
u_int32_t	get_tag_size(char tag_header[10]);
void		free_tag(t_id3tag **ptr);
void		write_tag(t_id3tag *tag, int fd);
#endif