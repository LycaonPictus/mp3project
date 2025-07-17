#include <id3tagged_file.h>
#include <apic.h>

int main(int argc, char **argv)
{
	t_id3tagged_file	*file;
	char				*filename;

	if (argc < 2)
		return (1);
	filename = argv[1];
	file = get_tagged_file(filename);
	if (!file)
		return (1);
	if (export_album_picture(file->tag, "test.jpg"))
		return (1);
	free_tagged_file(&file);
	return (0);
}
