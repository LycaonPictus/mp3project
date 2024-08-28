#include <unistd.h>
#include <mp3tag.h>
#include <fcntl.h>

static void	extract_tag_body(int fd_mp3, int fd_tag, u_int32_t	size)
{
	char		*body;
	int			bytes_read;

	body = malloc(size);
	bytes_read = read(fd_mp3, body, size);
	write(fd_tag, body, bytes_read);
	free(body);
}

void	extract_tag(int fd_mp3, int fd_tag)
{
	char		header[10];
	u_int32_t	size;
	int			bytes_read;
	int			fd_mp3;
	int			fd_tag;

	bytes_read = read(fd_mp3, header, 10);
	write(fd_tag, header, 10);
	size = get_tag_size(&header[6]);
	extract_tag_body(fd_mp3, fd_tag, size);
	close(fd_mp3);
	close(fd_tag);
}

int main()
{
	char *f1 = "example/Ed Sheeran - Thinking out loud.mp3";
	char *f2 = "example/Ed Sheeran - Thinking out loud.tag";

	extract_tag(open(f1, O_RDONLY), open(f2, O_CREAT | O_WRONLY));
	return 0;
}