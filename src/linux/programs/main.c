#include <mp3project.h>
#define BUFFER_SIZE 1024
#define BIN_DIR_NAME "bin"

int	execute_cmd(int argc, char **argv, char **env)
{
	char	*folder;
	char	*path;
	size_t	bin_dir_size;
	size_t	com_size;
	int		pid;
	int		result;

	folder = "./" BIN_DIR_NAME;
	result = 0;
	if (!argc)
		return (1);
	bin_dir_size = strlen(folder);
	com_size = strlen(argv[0]);
	path = malloc(bin_dir_size + com_size + 2);
	memcpy(path, folder, bin_dir_size);
	path[bin_dir_size++] = '/';
	memcpy(&path[bin_dir_size], argv[0], com_size);
	path[bin_dir_size + com_size] = '\0';
	pid = fork();
	if (!pid)
		result = execve(path, argv, env);
	else
		free(path);
	if (result)
		exit(1);
	return (0);
}

void	free_array(char **array)
{
	unsigned int	array_index;

	array_index = 0;
	while (array[array_index])
		free(array[array_index++]);
	free(array);
}

/* char	*my_readline(char *prompt)
{
	char	buffer[BUFFER_SIZE];
	int		bytes_read;

	write(1, prompt, strlen(prompt));
	bytes_read = read(0, buffer, BUFFER_SIZE);
	buffer[bytes_read - 1] = '\0';
	return (strdup(buffer));
} */

int	enter_shell(char **env)
{
	char	*line;
	int		argc;
	char	**argv;

	write(1, "--Welcome to the Tag Editor --\n", 32);
	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (*line)
		{
			check_line(&line);
			add_history(line);
			get_argv(line, &argc, &argv);
			if (!strcmp(argv[0], "exit"))
			{
				free(line);
				free_array(argv);
				rl_clear_history();
				return (0);
			}
			execute_cmd(argc, argv, env);
			free_array(argv);
		}
		free(line);
	}
	return (1);
}

int main(int argc, char **argv, char **env)
{
    if (argc == 1)
        return (enter_shell(env));
	else
		return (execute_cmd(argc - 1, &argv[1], env));
}
