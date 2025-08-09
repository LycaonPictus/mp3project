#include <id3tagged_file.h>
#include <unistd.h>

int	main()
{
	t_id3tagged_file	*tf;

	tf = get_tagged_file("example/4 Non blondes - What's up.mp3");
	if (!tf)
		return 1;
	t_id3framelist	*node;
	t_id3frame		*f;
	node = filter_by_id(tf->tag->frames, "TIT2");
	if (!node)
	{
		free_tagged_file(&tf);
		return 0;
	}
	f = node->frame;
	write(1, &f->content[1], f->header.size - 1);
	free_tagged_file(&tf);
	return 0;
}