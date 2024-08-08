#include <mp3project.h>

int	execute_cmd(int argc, char **argv)
{
	if (argc > 0)
	{
		write(1, "Executing ", 10);
		write(1, argv[0], strlen(argv[0]));
		write(1, "...\n", 4);
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

int	enter_program()
{
	char	*line;
	int		argc;
	char	**argv;

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
    {
        return (enter_program());
    }
	else
	{
		return (execute_cmd(argc - 1, &argv[1]));
	}
}
