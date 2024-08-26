#ifndef MP3FILE_H
#define MP3FILE_H
#include <stdlib.h>

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

typedef struct s_mp3frame
{
	t_frameID	frameID;
	u_int32_t	frameSize;
	u_char		flags[2];
	void		*content;
}	t_mp3frame;

typedef struct s_mp3tag
{

	t_mp3frame	*frames;

}	t_mp3tag;

typedef struct s_mp3file
{
	t_mp3tag	*tag;
	char		*audio;
}	t_mp3file;

#endif