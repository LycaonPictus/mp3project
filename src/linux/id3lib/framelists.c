#include <id3framelist.h>
#include <stdlib.h>
#include <string.h>

t_id3framelist	*read_frames_v3(int fd, uint32_t size, uint32_t *padding)
{
	t_id3frame		*frame;
	t_id3framelist	*list;
	t_id3framelist	*last;
	t_id3framelist	*new;

	list = NULL;
	last = NULL;
	do // At least once to get the padding data
	{
		frame = get_frame(fd, &size, padding);
		if (!frame)
			break ;
		new = malloc(sizeof(t_id3framelist));
		new->next = NULL;
		if (!new)
		{
			free_frame(&frame);
			free_framelist(&list, 1);
			return (NULL);
		}
		new->frame = frame;
		if (!last)
		{
			list = new;
			last = new;
		}
		else
		{
			last->next = new;
			last = new;
		}
	}
	while (size);
	return (list);
}

void	del_frame_by_index(t_id3framelist **ptr, unsigned int index)
{
	t_id3framelist	*node;
	unsigned int	cur_index;
	t_id3framelist	*prev;

	cur_index = 0;
	prev = NULL;
	node = *ptr;
	while (node && cur_index < index)
	{
		prev = node;
		node = node->next;
		cur_index++;
	}
	if (!node)
		return ;
	if (prev)
		prev->next = node->next;
	else
		*ptr = node->next;
	free_frame(&node->frame);
	free(node);
}

void	del_frame_by_id(t_id3framelist **ptr, char id[4])
{
	t_id3framelist	*node;
	t_id3framelist	*prev;

	prev = NULL;
	node = *ptr;
	while (node)
	{
		if (!strncmp(id, node->frame->header.frameID, 4))
		{
			if (prev)
				prev->next = node->next;
			else
				*ptr = node->next;
			free_frame(&node->frame);
			free(node);
			if (prev)
				node = prev->next;
			else
				node = *ptr;
		}
		else
		{
			prev = node;
			node = node->next;
		}
	}
}

int	add_frame_last(t_id3framelist **ptr, t_id3frame *frame)
{
	t_id3framelist	*node;
	t_id3framelist	*new_node;

	if (!frame)
		return (1);
	new_node = malloc(sizeof(t_id3framelist));
	if (!new_node)
		return (1);
	new_node->frame = frame;
	new_node->next = NULL;
	if (!*ptr)
	{
		*ptr = new_node;
		return (0);
	}
	node = *ptr;
	while (node->next)
		node = node->next;
	node->next = new_node;
	return (0);
}

t_id3framelist	*filter_by_id(t_id3framelist *list, char id[4])
{
	t_id3framelist	*out;

	out = NULL;
	while (list)
	{
		if (has_tag_id(list->frame, id))
			if (add_frame_last(&out, list->frame))
				free_framelist(&out, 0);
		list = list->next;
	}
	return (out);
}

void	free_framelist(t_id3framelist **ptr, int delete_frames)
{
	t_id3framelist *node;
	t_id3framelist *next;

	node = *ptr;
	while (node)
	{
		next = node->next;
		if (delete_frames)
			free_frame(&node->frame);
		free(node);
		node = next;
		*ptr = node;
	}
}

int	write_frames(t_id3framelist *list, int fd)
{
	int	bytes_written;

	if (!list)
		return (1);
	while (list)
	{
		bytes_written = write_frame(list->frame, fd);
		if (bytes_written == -1)
			return (1);
		list = list->next;
	}
	return (0);
}
