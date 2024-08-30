#ifndef ID3TAGGED_FILE_H
# define ID3TAGGED_FILE_H
# include <id3tag.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# define BUFFER_SIZE 1024

typedef struct s_id3tagged_file
{
	t_id3tag	*tag;
	char		*name;
	int			fd;
}	t_id3tagged_file;

#endif