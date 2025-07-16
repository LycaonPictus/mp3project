#ifndef ID3_TAG_HEADER_H
# define ID3_TAG_HEADER_H

# include <stdint.h>

typedef struct s_id3tag_header
{
	char			version[2];
	uint8_t			flags;
	uint32_t		size;
}	t_id3tag_header;

t_id3tag_header	get_tag_header(char header[10]);
int				write_tag_header(t_id3tag_header header, int fd);

#endif