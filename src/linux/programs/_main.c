#include <id3tagged_file.h>
#include <sys/wait.h>

int	main(int argc, char **argv)
{
	t_id3tagged_file	*file;
	char				*out;
	int					fd_out;

	if (argc < 3 || argc > 3)
		return (1);
	file = get_tagged_file(argv[1]);
	if (!file)
		return (1);
	file->tag->size -= file->tag->padding_size;
	file->tag->padding_size = 0;
	if (argc == 3)
		out = argv[2];
	else
		out = argv[1];
	fd_out = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd_out == -1)
	{
		free_tagged_file(&file);
		return (1);
	}
	write_tag(file->tag, 1);
	write_rem(file, 1);
	close(fd_out);
	return (0);
}
