#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char	*get_string(char *data, uint32_t *i, char encoding)
{
	char		*string;
	uint32_t	start;
	uint32_t	size;

	start = *i;
	if (!data)
		return (NULL);
	size = 0;
	if (encoding == 0x0)
		while (data[start + size])
			size++;
	else if (encoding == 0x1)
		while (data[start + size] || data[start + size + 1])
			size += 2;
	string = malloc(size + 1);
	if (!string)
		return (NULL);
	memcpy(string, &data[start], size + 1);
	if (encoding == 0x0)
		*i += size + 1;
	else if (encoding == 0x1)
		*i += size + 2;
	return (string);
}
