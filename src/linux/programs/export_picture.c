#include <id3tagged_file.h>
#include <apic.h>

int main(int argc, char **argv)
{
	t_id3tagged_file	*file;
	char				*filename;

	if (argc < 2)
	{
		write(2, "Usage: ", 7);
		write(2, argv[0], strlen(argv[0]));
		write(2, " [input file] <[output file]>\n", 30);
		return (1);
	}
	filename = argv[1];
	file = get_tagged_file(filename);
	if (!file)
		return (1);
	if (export_album_picture(file->tag, "test.jpg")) // Add the output file
		return (1);
	free_tagged_file(&file);
	return (0);
}
