#ifndef _ID3FRAME_H
#define _ID3FRAME_H
# include <stdlib.h>
# include <ctype.h>
# include <unistd.h>
# include <string.h>

typedef enum e_frameID
{
	ERROR = -1,
	AENC,
	APIC,
	COMM,
	COMR,
	ENCR,
	EQUA,
	ETCO,
	GEOB,
	GRID,
	IPLS,
	LINK,
	MCDI,
	MLLT,
	OWNE,
	PRIV,
	PCNT,
	POPM,
	POSS,
	RBUF,
	RVAD,
	RVRB,
	SYLT,
	SYTC,
	TALB,
	TBPM,
	TCOM,
	TCON,
	TCOP,
	TDAT,
	TDLY,
	TENC,
	TEXT,
	TFLT,
	TIME,
	TIT1,
	TIT2,
	TIT3,
	TKEY,
	TLAN,
	TLEN,
	TMED,
	TOAL,
	TOFN,
	TOLY,
	TOPE,
	TORY,
	TOWN,
	TPE1,
	TPE2,
	TPE3,
	TPE4,
	TPOS,
	TPUB,
	TRCK,
	TRDA,
	TRSN,
	TRSO,
	TSIZ,
	TSRC,
	TSSE,
	TYER,
	TXXX,
	UFID,
	USER,
	USLT,
	WCOM,
	WCOP,
	WOAF,
	WOAR,
	WOAS,
	WORS,
	WPAY,
	WPUB,
	WXXX
}	t_frameID;

typedef struct s_id3frame
{
	char				frameID[4];
	u_int32_t			size;
	u_char				flags[2];
	char				*content;
}	t_id3frame;

t_id3frame	*get_frame(int fd, u_int32_t *rem, u_int32_t *padding);
void		free_frame(t_id3frame **ptr);
int			write_frame(t_id3frame *frame, int fd);

#endif