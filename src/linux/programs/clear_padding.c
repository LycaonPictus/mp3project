#include <id3tagged_file.h>
#define TEMP_FILE ".temp"
#include <sys/wait.h>

static int	temp_writing(char *input)
{
	int					temp_fd;
	t_id3tagged_file	*file;
	int					result;

	result = 0;
	file = get_tagged_file(input);
	if (!file)
		return (1);
	temp_fd = open (TEMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (temp_fd == -1)
	{
		write(2, "Error opening temp file.\n", 26);
		free_tagged_file(&file);
		return (1);
	}
	if (file->tag)
	{
		file->tag->size -= file->tag->padding_size;
		file->tag->padding_size = 0;
	}
	result = write_file(file, temp_fd);
	free_tagged_file(&file);
	close(temp_fd);
	return (result);
}

static int	out_writing(char *output)
{
	int		temp_fd;
	int		fd_out;
	int		bytes_read;
	char	buffer[BUFFER_SIZE];
	int		result;

	temp_fd = open (TEMP_FILE, O_RDONLY);
	if (temp_fd == -1)
	{
		write(2, "Error opening temp file.\n", 26);
		return (1);
	}
	fd_out = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd_out == -1)
	{
		write(2, "Error opening the output file.\n", 31);
		close(temp_fd);
		return (1);
	}
	bytes_read = 0;
	result = 0;
	do
	{
		write(fd_out, buffer, bytes_read);
		bytes_read = read(temp_fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			result = 1;
			write(2, "Error reading file.\n", 20);
			break ;
		}
	} while (bytes_read);
	close(temp_fd);
	close(fd_out);
	return (result);
}

char	*next_path(char **paths, char *cmd)
{
	char	*path;
	size_t	path_len;
	size_t	cmd_len;
	size_t	i;

	if (!**paths)
		return (NULL);
	path_len = 0;
	while (*paths && (*paths)[path_len] != ':' && (*paths)[path_len] != '\0')
		path_len++;
	cmd_len = 0;
	while (cmd && cmd[cmd_len])
		cmd_len++;
	path = malloc(path_len + cmd_len + 2);
	if (!path)
		return (NULL);
	i = 0;
	while (i < path_len)
	{
		path[i] = (*paths)[i];
		i++;
	}
	path[path_len++] = '/';
	i = 0;
	while (i < cmd_len)
	{
		path[path_len + i] = cmd[i];
		i++;
	}
	path[path_len + i] = '\0';
	*paths += path_len;
	if (**paths == ':')
		(*paths)++;
	return (path);
}

void	call_rm(char *file, char **env)
{
	pid_t	pid;
	char	*argv[4];
	char	*path;
	char	**envcpy;

	envcpy = env;
	while (*envcpy && memcmp(*envcpy, "PATH=", 5))
		envcpy++;
	if (!*envcpy)
	{
		write(2, "PATH not found.\n", 17);
		return ;
	}
	(*envcpy) += 5;
	path = NULL;
	while (**envcpy)
	{
		free(path);
		path = next_path(envcpy, "rm");
		if (!path)
			return ;
		if (!access(path, F_OK))
			break;
	}
	argv[0] = "rm";
	argv[1] = "-rf";
	argv[2] = file;
	argv[3] = NULL;
	pid = fork();
	if (pid)
	{
		waitpid(pid, NULL, 0);
		free(path);
	}
	else
		execve(path, argv, env);
	
}

int	main(int argc, char **argv, char **env)
{
	char	*out;

	if (argc < 2 || argc > 3)
	{
		write(2, "usage: " , 8);
		write(2, argv[0], strlen(argv[0]));
		write(2, " input_file [output_file].\n", 28);
		return (1);
	}
	if (argc == 3)
		out = argv[2];
	else
		out = argv[1];
	if (temp_writing(argv[1]) || out_writing(out))
		return (1);
	call_rm(TEMP_FILE, env);
	return (0);
}
