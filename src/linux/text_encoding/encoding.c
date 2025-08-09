#include <encoding.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#ifndef ENCODER_BUFFER
# define ENCODER_BUFFER 1024
#endif

#define ICONV_COMMAND "iconv -f UTF-8 -t UTF-16LE"

static void	append_text(t_string *string, char *buffer, int buf_size)
{
	char	*new_str;

	if (buf_size <= 0)
		return ;
	new_str = malloc(string->size + buf_size);
	if (new_str)
	{
		memcpy(new_str, string->ptr, string->size);
		memcpy(&new_str[string->size], buffer, buf_size);
		string->size += buf_size;
	}
	else
		string->size = 0;
	free(string->ptr);
	string->ptr = new_str;
}

static int	text_saving(int fd, t_string *converted)
{
	char	buffer[ENCODER_BUFFER];
	int		bytes_read;

	do
	{
		bytes_read = read(fd, buffer, ENCODER_BUFFER);
		if (bytes_read == -1)
		{
			free(converted->ptr);
			converted->ptr = NULL;
			return (1);
		}
		append_text(converted, buffer, bytes_read);
	} while (bytes_read == ENCODER_BUFFER);
	return 0;
}

int	text_conversion(t_string *original, t_string *converted, char encoding)
{
	int	pipe_1[2];
	int	pid_write;
	int	pipe_2[2];
	int	pid_conv;
	int	r;

	if (encoding != 0x1)
		return 1;
	if (pipe(pipe_1) == -1)
		return 1;
	pid_write = fork();
	if (!pid_write)
	{
		close(pipe_1[0]);
		write(pipe_1[1], original->ptr, original->size);
		close(pipe_1[1]);
		exit(0);
	}
	waitpid(pid_write, &r, 0);
	close(pipe_1[1]);
	if (pipe(pipe_2) == -1)
		return 1;
	pid_conv = fork();
	if (!pid_conv)
	{
		dup2(pipe_1[0], 0);
		dup2(pipe_2[1], 1);
		execlp("iconv", "iconv", "-f", "UTF-8", "-t", "UTF-16LE", NULL);
		close(pipe_1[0]);
		close(pipe_2[1]);
		exit(1);
	}
	close(pipe_1[0]);
	waitpid(pid_conv, &r, 0);
	close(pipe_2[1]);
	text_saving(pipe_2[0], converted);
	close(pipe_2[0]);
	return 0;
}

int	file_conversion(char *filename, t_string *converted, char encoding)
{
	int	pipefd[2];
	int	pid;
	int r;
	
	if (encoding != 0x1)
		return 1;
	if (pipe(pipefd) == -1)
		return 1;
	pid = fork();
	if (!pid)
	{
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		write(1, "\xff\xfe", 2);
		execlp("iconv", "iconv", "-f", "UTF-8", "-t", "UTF-16LE", filename, NULL);
		close(pipefd[1]);
		exit(1);
	}
	waitpid(pid, &r, 0);
	close(pipefd[1]);
	text_saving(pipefd[0], converted);
	close(pipefd[0]);
	return 0;
}
