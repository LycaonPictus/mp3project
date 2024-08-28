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

int	extract_tag(char *file_mp3, char *file_tag)
{
	char		header[10];
	u_int32_t	size;
	int			bytes_read;
	int			fd_mp3;
	int			fd_tag;

	fd_mp3 = open(file_mp3, O_RDONLY);
	if (fd_mp3 == -1)
	{
		write(2, "Error. File not found.\n", 23);
		return (1);
	}
	if (!access(file_tag, F_OK))
	{
		write(2, "Error. File already exists.\n", 28);
		close(fd_mp3);
		return (1);
	}
	fd_tag = open(file_tag, O_WRONLY | O_APPEND, 666);
	if (fd_tag == -1)
	{
		write(2, "Error. Cannot open file.\n", 25);
		close(fd_mp3);
		return (1);
	}
	bytes_read = read(fd_mp3, header, 10);
	write(fd_tag, header, 10);
	size = get_tag_size(&header[6]);
	extract_tag_body(fd_mp3, fd_tag, size);
	close(fd_mp3);
	close(fd_tag);
	return (0);
}

int main()
{
	char *f1 = "example/Ed Sheeran - Thinking out loud.mp3";
	char *f2 = "example/Ed Sheeran - Thinking out loud.tag";

	extract_tag(f1, f2);
	return 0;
}