#ifndef TEXT_FRAME_H
#define TEXT_FRAME_H

# include <stdint.h>

typedef struct s_text_content
{
	char		text_encoding;
	char		*information;
	uint32_t	size;
}	t_text_content;

#endif