#include <mp3project.h>
#define BUFFER_SIZE 1024

int	execute_cmd(int argc, char **argv)
{
	if (argc > 0)
	{
		write(1, "Executing \"", 11);
		write(1, argv[0], strlen(argv[0]));
		write(1, "\"...\n", 5);
	}
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

char	*my_readline(char *prompt)
{
	//char	*line;
	char	buffer[BUFFER_SIZE];
	int		bytes_read;

	write(1, prompt, strlen(prompt));
	bytes_read = read(0, buffer, BUFFER_SIZE);
	buffer[bytes_read] = '\0';
	printf("%i\n", bytes_read);
	return (strdup(buffer));
}

int	enter_shell()
{
	char	*line;
	int		argc;
	char	**argv;

	line = NULL;
	while (1)
	{
		line = my_readline("> ");
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
			execute_cmd(argc, argv);
			free_array(argv);
		}
		free(line);
	}
	return (1);
}

int main(int argc, char **argv)
{
    if (argc == 1)
        return (enter_shell());
	else
		return (execute_cmd(argc - 1, &argv[1]));
}
