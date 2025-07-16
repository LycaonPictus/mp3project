#ifndef ID3TAG_H
#define ID3TAG_H

# include <id3tag_header.h>
# include <id3framelist.h>

typedef struct s_id3tag
{
	t_id3tag_header	header;
	t_id3framelist	*frames;
	uint32_t		padding_size;
}	t_id3tag;

t_id3tag	*get_tag(int fd, char header[10]);
void		free_tag(t_id3tag **ptr);
int			write_tag(t_id3tag *tag, int fd);
#endif