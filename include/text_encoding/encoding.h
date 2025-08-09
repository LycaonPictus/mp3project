#ifndef ENCODING_H
#define ENCODING_H

#include <inttypes.h>

typedef struct	s_string
{
	char		*ptr;
	uint32_t	size;
}	t_string;

int	text_conversion(t_string *original, t_string *converted, char encoding);
int	file_conversion(char *filename, t_string *converted, char encoding);

#endif