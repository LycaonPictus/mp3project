#ifndef ID3_USLT_H
#define ID3_USLT_H

#include <inttypes.h>
#include <id3frame.h>

typedef struct	s_uslt
{
	char		text_encoding;
	char		language[3];
	char		*description;
	char		*text;
	uint32_t	text_size;
}	t_uslt;

t_uslt	*new_uslt();
void	free_uslt(t_uslt **ptr);

t_id3frame	*uslt_to_frame(t_uslt *uslt);

#endif