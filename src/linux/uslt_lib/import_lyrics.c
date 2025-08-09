#include <uslt.h>
#include <id3tagged_file.h>
#include <stdlib.h>
#include <encoding.h>

int	import_lyrics(t_id3tag *tag, char *lyrics_file)
{
	t_uslt			*uslt;
	t_string		text;
	t_id3frame		*frame;

	if (!tag)
		return (1);
	uslt = new_uslt();
	if (!uslt)
		return 1;
	text.ptr = NULL;
	text.size = 0;
	if (file_conversion(lyrics_file, &text, 0x1))
	{
		free_uslt(&uslt);
		return 1;
	}
	uslt->text_encoding = 0x1;
	memcpy(uslt->language, "eng", 3);
	uslt->text = text.ptr;
	uslt->text_size = text.size;
	uslt->description = NULL;
	frame = uslt_to_frame(uslt);
	free_uslt(&uslt);
	return add_frame_last(&tag->frames, frame);
}

int	main()
{
	t_id3tagged_file	*tf;
	int	fd_out;

	tf = get_tagged_file("example/Bump of chicken - Shin sekai.mp3");
	if (!tf)
		return 1;
	if (import_lyrics(tf->tag, "example/Shin sekai lyrics.txt"))
	{
		free_tagged_file(&tf);
		return 1;
	}
	fd_out = open("example/test.mp3", O_WRONLY | O_CREAT, 0666);
	if (fd_out != -1)
		write_file(tf, fd_out);
	close(fd_out);
	free_tagged_file(&tf);
	return (fd_out == -1);
}