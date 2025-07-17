#include <textframe.h>
#include <stdlib.h>
#include <id3frame.h>
#include <string.h>

t_text_content	*new_text_content()
{
	t_text_content	*content;

	content = malloc(sizeof(t_text_content));
	if (!content)
		return (NULL);
	content->text_encoding = 0x0;
	content->size = 0;
	content->information = NULL;
	return (content);
}

void	free_text_content(t_text_content **ptr)
{
	t_text_content	*content;

	content = *ptr;
	if (!content)
		return ;
	free(content->information);
	free(content);
	*ptr = NULL;
}

t_text_content	*parse_text_content(t_id3frame const *frame)
{
	t_text_content	*content;

	if (has_tag_id(frame, "TXXX") || frame->header.frameID[0] != 'T')
		return (NULL);
	content = new_text_content();
	content->text_encoding = frame->content[0];
	content->size = frame->header.size - 1;
	content->information = malloc(content->size);
	if (!content->information)
	{
		free_text_content(&content);
		return (NULL);
	}
	memcpy(content->information, &frame->content[1], content->size);
	return (content);
}

char	*to_frame_content(t_text_content *content, uint32_t *size)
{
	char	*out;
	if (!content)
		return (NULL);
	out = malloc(content->size + 1);
	if (!out)
		return (NULL);
	out[0] = content->text_encoding;
	memcpy(&out[1], content->information, content->size);
	*size = content->size + 1;
	return (out);
}

#include <id3tagged_file.h>
int	main()
{
	t_id3tagged_file	*file;
	t_text_content		*content;

	file = get_tagged_file("example/Bump of chicken - Shin sekai.mp3");
	if (!file)
		return (1);
	for (t_id3framelist *node = file->tag->frames; node; node = node->next)
	{
		content = parse_text_content(node->frame);
		if (content)
			break;
	}
	if (!content)
		return (0);
	write(1, content->information, content->size);
	free_text_content(&content);
	free_tagged_file(&file);
	return (0);
}
