#ifndef RENAMEBYID3_H__
#define RENAMEBYID3_H__

#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <id3tag.h>

#define TAGLEN 3

#ifndef MAXNAMLEN
#define MAXNAMLEN 255
#endif

int     getID3Tag (ID3Tag * myTag, const char *filename);
bool    renameMP3 (const char *oldfilename, const ID3Tag * myTag);
void    cleanID3Tag (ID3Tag * myTag);

#endif /* RENAMEBYID3_H__ */
