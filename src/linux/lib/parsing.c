#include <mp3project.h>

static void	addline(char **lineptr)
{
	unsigned int	len1;
	unsigned int	len2;
	char			*line1;
	char			*line2;
	char			*full_line;

	line1 = *lineptr;
	line2 = readline(">> ");
	len1 = strlen(line1);
	len2 = strlen(line2);
	full_line = malloc(len1 + len2 + 2);
	if (!full_line)
	{
		*lineptr = NULL;
		return;
	}
	memcpy(full_line, line1, len1);
	free(line1);
	full_line[len1] = '\n';
	memcpy(&full_line[len1 + 1], line2, len2);
	free(line2);
	full_line[len1 + len2 + 1] = '\0';
	*lineptr = full_line;
}

void	check_line(char **lineptr)
{
	char			quote;
	int				esc;
	char			*line;

	line = *lineptr;
	if (!line)
		return;
	quote = '\0';
	esc = 0;
	while (*line)
	{
		if (!esc && *line == '\\')
			esc = 1;
		else if (!esc && !quote && (*line == '\'' || *line == '\"'))
			quote = *line;
		else if (!esc && quote == *line)
			quote = '\0';
		else if (esc)
			esc = 0;
		line++;
	}
	if (esc || quote)
	{
		addline(lineptr);
		check_line(lineptr);
	}
}

static void	get_argc(char *line, int *argcptr)
{
	int		argc;
	char	quote;
	int		esc;

	argc = 0;
	quote = '\0';
	esc = 0;
	while (*line)
	{
		while (isspace(*line))
			line++;
		if (*line && !isspace(*line))
			argc++;
		while (*line && (esc || quote || !isspace(*line)))
		{
			if (!esc && *line == '\\')
				esc = 1;
			else if (!esc && !quote && (*line == '\'' || *line == '\"'))
				quote = *line;
			else if (!esc && quote == *line)
				quote = '\0';
			else if (esc)
				esc = 0;
			line++;
		}
	}
	*argcptr = argc;
}

static unsigned int	arg_len(char *str)
{
	unsigned int	len;
	int				esc;
	char			quote;

	len = 0;
	esc = 0;
	quote = '\0';
	while (isspace(*str))
		str++;
	while (*str && (esc || quote || !isspace(*str)))
	{
		if (!esc && *str == '\\')
			esc = 1;
		else if (!esc && !quote && (*str == '\'' || *str == '\"'))
			quote = *str;
		else if (!esc && quote == *str)
			quote = '\0';
		else
		{
			esc = 0;
			len++;
		}
		str++;
	}
	return (len);
}

static char	*get_arg(char **strptr)
{
	unsigned int	i;
	unsigned int	j;
	char			*str;
	int				esc;
	char			quote;
	char			*arg;

	i = 0;
	str = *strptr;
	if (!str)
		return NULL;
	while (str[i] && isspace(str[i]))
		i++;
	if (!str[i])
	{
		*strptr += i;
		return NULL;
	}
	arg = malloc(arg_len(str) + 1);
	if (!arg)
		return NULL;
	esc = 0;
	quote = '\0';
	j = 0;
	while (str[i] && (esc || quote || !isspace(str[i])))
	{
		if (!esc && str[i] == '\\')
			esc = 1;
		else if (!esc && !quote && (str[i] == '\'' || str[i] == '\"'))
			quote = str[i];
		else if (!esc && quote == str[i])
			quote = '\0';
		else
		{
			esc = 0;
			arg[j++] = str[i];
		}
		i++;
	}
	arg[j] = '\0';
	*strptr += i;
	return arg;
}

void	get_argv(char *line, int *argcptr, char ***argvptr)
{
	unsigned int	array_index;

	if (!line)
	{
		*argvptr = NULL;
		return;
	}
	get_argc(line, argcptr);
	*argvptr = malloc(sizeof(char **) * (*argcptr + 1));
	if (!*argvptr)
		return;
	array_index = 0;
	while (*line)
	{
		(*argvptr)[array_index++] = get_arg(&line);
		if (!*line)
			(*argvptr)[array_index] = NULL;
	}
}
