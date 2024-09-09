#include <id3framelist.h>

t_id3framelist	*read_frames_v3(int fd, u_int32_t size, u_int32_t *padding)
{
	t_id3frame		*frame;
	t_id3framelist	*list;
	t_id3framelist	*last;
	t_id3framelist	*new;

	list = NULL;
	last = NULL;
	do
	{
		frame = get_frame(fd, &size, padding);
		if (!frame)
			break ;
		new = malloc(sizeof(t_id3framelist));
		new->next = NULL;
		if (!new)
		{
			free_frame(&frame);
			free_framelist(&list);
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

void	del_frame(t_id3framelist **ptr, unsigned int index)
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

void	free_framelist(t_id3framelist **ptr)
{
	t_id3framelist *node;
	t_id3framelist *next;

	node = *ptr;
	while (node)
	{
		next = node->next;
		free_frame(&node->frame);
		free(node);
		node = next;
	}
}

int	write_frames(t_id3framelist *list, int fd)
{
	int	bytes_written;
	int	total_bytes;

	total_bytes = 0;
	while (list)
	{
		bytes_written = write_frame(list->frame, fd);
		if (bytes_written == -1)
			return (-1);
		total_bytes += bytes_written;
		list = list->next;
	}
	return (total_bytes);
}
