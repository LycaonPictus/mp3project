#include <unistd.h>
#include <mp3tag.h>
#include <fcntl.h>

static int	extract_tag_header(int fd_mp3, int fd_tag, char header[10])
{
	int			bytes_read;

	bytes_read = read(fd_mp3, header, 10);
	if (bytes_read < 10)
	{
		write(2, "Error. Cannot read tag header.\n", 31);
		return (1);
	}
	write(fd_tag, header, bytes_read);
	return (0);
}

static int	extract_tag_body(int fd_mp3, int fd_tag, char header[10])
{
	char		*body;
	u_int32_t	size;
	int			bytes_read;

	size = get_tag_size(&header[6]);
	body = malloc(size);
	bytes_read = read(fd_mp3, body, size);
	if (bytes_read == -1)
	{
		write (2, "Error. Cannot read file.\n", 25);
		return (1);
	}
	write(fd_tag, body, bytes_read);
	free(body);
	return (0);
}

int	extract_tag(char *file_mp3, char *file_tag)
{
	char		header[10];
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
	fd_tag = open(file_tag, O_WRONLY | O_CREAT, 0666);
	if (fd_tag == -1)
	{
		write(2, "Error. Cannot open file.\n", 25);
		close(fd_mp3);
		return (1);
	}
	if (extract_tag_header(fd_mp3, fd_tag, header))
		return (1);
	if (extract_tag_body(fd_mp3, fd_tag, header))
		return (1);
	close(fd_mp3);
	close(fd_tag);
	return (0);
}

int main()
{
	extract_tag("example/Ed Sheeran - Thinking out loud.mp3", "example/Ed Sheeran - Thinking out loud.tag");
	return (0);
}