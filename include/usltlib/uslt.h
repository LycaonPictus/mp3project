#ifndef ID3_USLT_H
#define ID3_USLT_H

typedef struct	s_uslt
{
	char		text_encoding;
	char		language[3];
	char		*description;
	char		*text;
	uint32_t	text_size;
}	t_uslt;

t_uslt	*new_uslt();

#endif