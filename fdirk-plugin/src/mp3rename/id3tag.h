#ifndef ID3TAG_H__
#define ID3TAG_H__

#include <stdbool.h>
#include <dirent.h>

/*
	 Flags for reading and writing
*/
#define ID3V1 1
#define ID3V2 2

#define ID3SONGNAME 30
#define ID3ARTIST 30
#define ID3ALBUM 30
#define ID3COMMENT 30
#define ID3YEAR 4
#define TAGLEN 3

struct id3tag_t
{
	char    *songname;
	char    *artist;
	char    *album;
	char    *comment;
	char    track;
	char    *year;
	char    genre;
};

typedef struct id3tag_t ID3Tag;

bool    id3_create (ID3Tag * tag);
bool    id3_read (ID3Tag * tag, const char *filename, unsigned int flags);
bool    id3_write (const ID3Tag * tag, const char *filename,
									 unsigned int flags);
bool id3_copyvals(ID3Tag *dst, const ID3Tag *src);

bool    id3_setSongname (ID3Tag * tag, const char *songname);
bool    id3_setArtist (ID3Tag * tag, const char *artist);
bool    id3_setAlbum (ID3Tag * tag, const char *album);
bool    id3_setComment (ID3Tag * tag, const char *comment);
bool    id3_setTrack (ID3Tag * tag, int track);
bool    id3_setYear (ID3Tag * tag, const char *year);
bool    id3_setGenre (ID3Tag * tag, unsigned int genre);

const char *id3_getSongname (const ID3Tag * tag);
const char *id3_getArtist (const ID3Tag * tag);
const char *id3_getAlbum (const ID3Tag * tag);
const char *id3_getComment (const ID3Tag * tag);
int     id3_getTrack (const ID3Tag * tag);
const char *id3_getYear (const ID3Tag * tag);
unsigned int id3_getGenre (const ID3Tag * tag);


#endif /* ID3TAG_H__ */
