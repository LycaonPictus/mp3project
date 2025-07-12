#ifndef _ID3FRAME_HEADER_H
#define _ID3FRAME_HEADER_H

# include <stdint.h>

typedef enum e_frameID
{
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
	WXXX,
	ERROR = -1
}	t_frameID;

typedef struct s_id3frame_header
{
	char				frameID[4];
	t_frameID			id_int;
	uint32_t			size;
	uint8_t				flags[2];
}	t_id3frame_header;

t_id3frame_header	get_frame_header(char buffer[10]);
int					write_frame_header(t_id3frame_header header, int fd);

#endif