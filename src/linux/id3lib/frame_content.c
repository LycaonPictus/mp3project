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
	if (encoding == 0x0)
		size++;
	else if (encoding == 0x1)
		size += 2;
	string = malloc(size);
	if (!string)
		return (NULL);
	memcpy(string, &data[start], size);
	*i += size;
	return (string);
}

static char	detect_codification(char *string)
{
	if (!string)
		return (0x0);
	while (*string)
		if (*(string++) & 0x80)
			return (0x1);
	return (0x0);
}

static char	*to_utf_16(char *string, uint32_t *size)
{
	char	*content;
	size_t	i;
	size_t	n_chars;

	if (!string)
		return (NULL);
	n_chars = 0;
	i = 0;
	while (string[i])
	{
		if (string[i++] & 0x8)
			continue;
		n_chars++;
	}
	*size = 2 * (n_chars + 1);
	content = malloc(*size);
	if (!content)
		return (NULL);
	i = 0;
	while (*string)
	{
		if (*string & 0x8)
			content[i++] = *(string++);
		else
			content[i++] = '\0';
		content[i++] = *(string++);
	}
	content[i++] = '\0';
	content[i] = '\0';
	return (content);
}

char	*parse_string(char *string, uint32_t *size, char *codif)
{
	char	*content;
	size_t	len;

	*size = 0;
	*codif = 0x0;
	if (!string)
		return (NULL);
	*codif = detect_codification(string);
	if (*codif == 0x1)
		return (to_utf_16(string, size));
	len = strlen(string);
	content = malloc(len + 1);
	if (!content)
		return (NULL);
	*size = len + 1;
	memcpy(content, string, *size);
	return (content);
}
/* #include <stdio.h>
int main(int argc, char **argv)
{
	if (argc < 2)
		return (1);
	uint32_t	size;
	char		codif;
	char *text = parse_string(argv[1], &size, &codif);
	printf("Codif: %u\n", codif);
	printf("Size: %u\n", size);
	size_t	i = 0;
	while (i < size)
		printf("%u ", text[i++]);
	printf("\n");
	free(text);
	return (0);
}
 */