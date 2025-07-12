#include <id3frame_header.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static const char * const IDs[] = {
	"AENC",
	"APIC",
	"COMM",
	"COMR",
	"ENCR",
	"EQUA",
	"ETCO",
	"GEOB",
	"GRID",
	"IPLS",
	"LINK",
	"MCDI",
	"MLLT",
	"OWNE",
	"PRIV",
	"PCNT",
	"POPM",
	"POSS",
	"RBUF",
	"RVAD",
	"RVRB",
	"SYLT",
	"SYTC",
	"TALB",
	"TBPM",
	"TCOM",
	"TCON",
	"TCOP",
	"TDAT",
	"TDLY",
	"TENC",
	"TEXT",
	"TFLT",
	"TIME",
	"TIT1",
	"TIT2",
	"TIT3",
	"TKEY",
	"TLAN",
	"TLEN",
	"TMED",
	"TOAL",
	"TOFN",
	"TOLY",
	"TOPE",
	"TORY",
	"TOWN",
	"TPE1",
	"TPE2",
	"TPE3",
	"TPE4",
	"TPOS",
	"TPUB",
	"TRCK",
	"TRDA",
	"TRSN",
	"TRSO",
	"TSIZ",
	"TSRC",
	"TSSE",
	"TYER",
	"TXXX",
	"UFID",
	"USER",
	"USLT",
	"WCOM",
	"WCOP",
	"WOAF",
	"WOAR",
	"WOAS",
	"WORS",
	"WPAY",
	"WPUB",
	"WXXX",
	NULL
};

static uint32_t	get_frame_size(char array[4])
{
	uint32_t	size;
	int	i;

	size = 0;
	for (i = 0; i < 4; i++)
	{
		size *= 256;
		size += (unsigned char)array[i];
	}
	return (size);
}

static void	encode_frame_size(uint32_t size, char array[4])
{
	int			i;

	for (i = 3; i >= 0; i--)
	{
		array[i] = size % 256;
		size /= 256;
	}
}

t_id3frame_header	get_frame_header(char buffer[10])
{
	int					i;
	t_id3frame_header	header;

	memcpy(header.frameID, buffer, 4);
	header.id_int = ERROR;
	i = 0;
	while (IDs[i] != NULL && strncmp(header.frameID, IDs[i], 4))
		i++;
	if (IDs[i] != NULL)
		header.id_int = i;
	header.size = get_frame_size(&buffer[4]);
	header.flags[0] = buffer[8];
	header.flags[1] = buffer[9];
	return header;
}

int	write_frame_header(t_id3frame_header header, int fd)
{
	int		bytes_written;
	char	buffer[10];

	memcpy(buffer, header.frameID, 4);
	encode_frame_size(header.size, &buffer[4]);
	buffer[8] = header.flags[0];
	buffer[9] = header.flags[1];
	bytes_written = write(fd, buffer, 10);
	if (bytes_written < 10)
	{
		write(2, "Cannot write in given file descriptor.\n", 39);
		return (1);
	}
	return 0;
}