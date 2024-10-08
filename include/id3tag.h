#ifndef _ID3TAG_H
#define _ID3TAG_H
# include <id3framelist.h>

typedef struct s_id3tag
{
	char			version[2];
	u_char			flags;
	u_int32_t		size;
	t_id3framelist	*frames;
	u_int32_t		padding_size;

}	t_id3tag;

t_id3tag	*get_tag(int fd, char header[10]);
void		free_tag(t_id3tag **ptr);
int			write_tag(t_id3tag *tag, int fd);
#endif