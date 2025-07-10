#ifndef ID3_FRAME_LIST_H
#define ID3_FRAME_LIST_H

# include <id3frame.h>
# include <stdint.h>

typedef struct s_id3framelist
{
	t_id3frame				*frame;
	struct s_id3framelist	*next;
}	t_id3framelist;

t_id3framelist	*read_frames_v3(int fd, uint32_t size, uint32_t *padding);
void			free_framelist(t_id3framelist **ptr);
int				write_frames(t_id3framelist *list, int fd);

#endif