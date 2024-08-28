#ifndef _MP3TAG_H
#define _MP3TAG_H
# include <mp3frame.h>

typedef struct s_mp3tag
{
	char		version[2];
	char		flags;
	int			size;
	t_mp3frame	*frames;

}	t_mp3tag;

int	read_tag(int fd);

#endif