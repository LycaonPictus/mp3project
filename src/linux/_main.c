#include <id3tagged_file.h>
#include <sys/wait.h>

int	main(int argc, char **argv)
{
	t_id3tagged_file	*file;
	char				*out;
	int					fd_out;
	char				buffer[BUFFER_SIZE];
	int					bytes_read;
	int					pipe_fd[2];
	int					pid;

	if (argc < 2 || argc > 3)
		return (1);
	file = get_tagged_file(argv[1]);
	if (!file)
		return (1);
	file->tag->size -= file->tag->padding_size;
	file->tag->padding_size = 0;
	if (pipe(pipe_fd))
	{
		free_tagged_file(&file);
		return (1);
	}
	pid = fork();
	if (!pid)
	{
		close(pipe_fd[0]);
		write_tag(file->tag, pipe_fd[1]);
		write_rem(file, pipe_fd[1]);
		free_tagged_file(&file);
		close(pipe_fd[1]);
	}
	else
	{
		free_tagged_file(&file);
		close(pipe_fd[1]);
		//waitpid(pid, NULL, 0);
		if (argc == 3)
			out = argv[2];
		else
			out = argv[1];
		fd_out = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd_out == -1)
		{
			close(pipe_fd[0]);
			return (1);
		}
		bytes_read = 0;
		do
		{
			write(fd_out, &buffer, bytes_read);
			bytes_read = read(pipe_fd[0], &buffer, BUFFER_SIZE);
		} while (bytes_read > 0);
		close(fd_out);
	}
	write(1, "3\n", 2);
	return (0);
}
