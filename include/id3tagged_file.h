#ifndef ID3TAGGED_FILE_H
# define ID3TAGGED_FILE_H
# include <id3tag.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# define BUFFER_SIZE 1024

typedef struct s_id3tagged_file
{
	char			*name;
	t_id3tag		*tag;
	char			*content;
	unsigned int	content_size;
	int				fd;
}	t_id3tagged_file;

t_id3tagged_file	*get_tagged_file(char *file_name);
void				free_tagged_file(t_id3tagged_file **ptr);
int					write_content(t_id3tagged_file *tf, int fd_out);
int					write_file(t_id3tagged_file *tf, int fd);
#endif