#include <mp3tag.h>

static int	check_errors(int argc, char **argv)
{
	char	*message;
	char	*file;

	message = NULL;
	file = NULL;
	if (argc < 3)
		message = "not enough params.";
	else if (access(argv[1], F_OK))
	{
		message = "file not found.";
		file = argv[1];
	}
	else if (access(argv[1], R_OK))
	{
		message = "bad permissions.";
		file = argv[1];
	}
	else if (access(argv[2], F_OK))
	{
		message = "file not found.";
		file = argv[2];
	}
	else if (access(argv[2], W_OK))
	{
		message = "bad permissions.";
		file = argv[2];
	}
	if (!message)
		return (0);
	write(2, argv[0], strlen(argv[0]));
	write(2, ": ", 2);
	if (file)
	{
		write(2, file, strlen(file));
		write(2, ": ", 2);
	}
	write(2, message, strlen(message));
	write(2, "\n", 1);
	return (1);
}

int	main(int argc, char **argv)
{
	if (check_errors(argc, argv))
		return (1);
	return (0);
}