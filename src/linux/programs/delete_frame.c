#include <id3tagged_file.h>
#define TEMP_FILE ".temp"
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

static int	temp_writing(char *input, char *tag_id)
{
	int					temp_fd;
	t_id3tagged_file	*file;
	int					result;

	result = 0;
	file = get_tagged_file(input);
	if (!file)
		return (1);
	temp_fd = open(TEMP_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (temp_fd == -1)
	{
		write(2, "Error opening temp file.\n", 26);
		free_tagged_file(&file);
		return (1);
	}
	if (file->tag)
		del_frame_by_id(&file->tag->frames, tag_id);
	result = write_file(file, temp_fd);
	free_tagged_file(&file);
	close(temp_fd);
	return (result);
}

static int	copy_to_outfile(char *output)
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

static int	validate_id(char *id)
{
	size_t	size;

	size = 0;
	while (id[size])
	{
		if (!isalnum(id[size]))
			return (1);
		if (size == 4)
			return (1);
		size++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	char	*out;

	if (argc < 3 || argc > 4 || validate_id(argv[2]))
	{
		write(2, "usage: " , 8);
		write(2, argv[0], strlen(argv[0]));
		write(2, " input_file tag_id [output_file].\n", 34);
		return (1);
	}
	if (argc == 4)
		out = argv[3];
	else
		out = argv[1];	
	if (temp_writing(argv[1], argv[2]) || copy_to_outfile(out))
	{
		system("rm -rf " TEMP_FILE);
		return (1);
	}
	system("rm -rf " TEMP_FILE);
	return (0);
}
