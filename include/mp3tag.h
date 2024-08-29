#ifndef _MP3TAG_H
#define _MP3TAG_H
# include <mp3frame.h>

typedef struct s_mp3tag
{
	char		version[2];
	char		flags;
	u_int32_t	size;
	t_mp3frame	*frames;

}	t_mp3tag;

t_mp3tag	*get_tag(int fd);
u_int32_t	get_tag_size(char tag_header[10]);

#endif