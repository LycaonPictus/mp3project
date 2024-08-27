#ifndef MP3FILE_H
#define MP3FILE_H
#include <mp3tag.h>

typedef struct s_mp3file
{
	t_mp3tag	*tag;
	char		*audio;
}	t_mp3file;

#endif