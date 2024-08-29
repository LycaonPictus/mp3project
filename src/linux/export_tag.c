#include <unistd.h>
#include <mp3tag.h>
#include <fcntl.h>

static int	read_header(int fd_mp3, char header[10])
{
	int			bytes_read;

	bytes_read = read(fd_mp3, header, 10);
	if (bytes_read < 10)
	{
		write(2, "Error. Cannot read tag header.\n", 31);
		return (1);
	}
	return (0);
}

static int	read_body(int fd_mp3, char header[10], char **body)
{
	u_int32_t	size;
	int			bytes_read;

	size = get_tag_size(&header[6]);
	*body = malloc(size);
	bytes_read = read(fd_mp3, *body, size);
	if (bytes_read < size)
	{
		write (2, "Error. Cannot read tag.\n", 24);
		return (1);
	}
	return (0);
}

int	extract_tag(char *file_mp3, char *file_tag)
{
	char	header[10];
	int		fd_mp3;
	int		fd_tag;
	char	*body;

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
	if (read_header(fd_mp3, header))
		return (1);
	body = NULL;
	if (read_body(fd_mp3, header, &body))
		return (1);
	close(fd_mp3);
	write (fd_tag, header, 10);
	write (fd_tag, body, get_tag_size(&header[6]));
	free(body);
	close(fd_tag);
	return (0);
}

int	main(void)
{
	extract_tag("example/Ed Sheeran - Thinking out loud.mp3", "example/Ed Sheeran - Thinking out loud.tag");
	return (0);
}
