#ifndef ID3_APIC_H
#define ID3_APIC_H

typedef struct s_album_pic
{
	char		text_encoding;
	char		*mime_type;
	char		picture_type;
	char		*description;
	char		*binary_data;
	uint32_t	data_size;
}	t_album_pic;

t_album_pic	*get_album_pic(t_id3tag *tag);
int	export_album_picture(t_id3tag *tag, char *filename);

#endif