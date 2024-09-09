#include <id3frame.h>

typedef struct s_id3framelist
{
	t_id3frame				*frame;
	struct s_id3framelist	*next;
}	t_id3framelist;

t_id3framelist	*read_frames_v3(int fd, u_int32_t size, u_int32_t *padding);
void			free_framelist(t_id3framelist **ptr);
int				write_frames(t_id3framelist *list, int fd);