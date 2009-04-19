#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <debug.h>
#include <cexception.h>

#include <mp3errcodes.h>
#include <id3tag.h>
#include <miscfuncs.h>

static bool id3_readid3v1 (ID3Tag * tag, FILE * fp);
static bool id3_writeid3v1 (const ID3Tag * tag, FILE * fp);
/* CODE NEEDS ERROR CHECKING */

/*
Func: id3_create (ID3Tag * tag)
Description: Create an id3 tag and set values
*/
bool id3_create (ID3Tag * tag)
{
	memset (tag, 0, sizeof (ID3Tag));
	return true;
}

/*
Func: id3_destroy (ID3Tag * tag)
Description: Destroy an id3 tag
*/
void id3_destroy (ID3Tag * tag)
{
	free (tag->songname);
	free (tag->artist);
	free (tag->album);
	free (tag->comment);
	free (tag->year);
}

/*
Func: id3_read (ID3Tag * tag, const char *filename, unsigned int flags)
Description: Read an id3 tag into 'tag' from 'filename'.  flags specify
  what kind of id3 tag to read in
*/
bool id3_read (ID3Tag * tag, const char *filename, unsigned int flags)
{
	FILE   *fp = fopen (filename, "r");
	if (fp == NULL)
	{
		CExCreatef (BAD_FILENAME, "Bad filename: %s", filename);
		return false;
	}

	memset (tag, 0, sizeof (ID3Tag));

	if (flags & ID3V1)
	{
		if (!id3_readid3v1 (tag, fp))
		{
			/*
			   error stuff 
			 */
		}
	}

	if (flags & ID3V2)
	{
		fseek (fp, 0, SEEK_SET);
	}

	fclose (fp);
	if (CExGetException ())
		return false;
	return true;
}

/*
Func: id3_write (const ID3Tag * tag, const char *filename, unsigned int flags)
Description: Write an id3 tag into the file specified by filename from the 'tag'
  variable, flags designante what kind of flag to output
*/
bool id3_write (const ID3Tag * tag, const char *filename, unsigned int flags)
{
	FILE   *fp = fopen (filename, "r+");
	bool    retval = true;

	if (fp == NULL)
	{
		CExCreatef (BAD_FILENAME, "Bad filename: %s does not exist", filename);
		return false;
	}

	if (flags & ID3V1)
	{
		retval = id3_writeid3v1 (tag, fp);
	}

	fclose (fp);

	return retval;
}

bool id3_copyvals (ID3Tag * dst, const ID3Tag * src)
{
	bool    retval = false;
	const char *temp;
	int     tempint;
	temp = id3_getSongname (src);
	if (temp)
	{
		id3_setSongname (dst, temp);
		retval = true;
	}
	temp = id3_getArtist (src);
	if (temp)
	{
		id3_setArtist (dst, temp);
		retval = true;
	}
	temp = id3_getAlbum (src);
	if (temp)
	{
		id3_setAlbum (dst, temp);
		retval = true;
	}
	temp = id3_getComment (src);
	if (temp)
	{
		id3_setComment (dst, temp);
		retval = true;
	}
	temp = id3_getYear (src);
	if (temp)
	{
		id3_setYear (dst, temp);
		retval = true;
	}
	tempint = id3_getTrack (src);
	if (tempint)
	{
		id3_setTrack (dst, tempint);
		retval = true;
	}
	tempint = id3_getGenre (src);
	if ((unsigned) tempint <= 79)
	{
		id3_setGenre (dst, tempint);
		retval = true;
	}

	return retval;
}

bool id3_setSongname (ID3Tag * tag, const char *songname)
{
	return strset (&tag->songname, songname);
}

bool id3_setArtist (ID3Tag * tag, const char *artist)
{
	return strset (&tag->artist, artist);
}

bool id3_setAlbum (ID3Tag * tag, const char *album)
{
	return strset (&tag->album, album);
}

bool id3_setComment (ID3Tag * tag, const char *comment)
{
	return strset (&tag->comment, comment);
}


bool id3_setTrack (ID3Tag * tag, int track)
{
	tag->track = (char) track;
	return true;
}

bool id3_setYear (ID3Tag * tag, const char *year)
{
	return strset (&tag->year, year);
}

bool id3_setGenre (ID3Tag * tag, unsigned int genre)
{
	tag->genre = (char) genre;
	return true;
}


const char *id3_getSongname (const ID3Tag * tag)
{
	return tag->songname;
}
const char *id3_getArtist (const ID3Tag * tag)
{
	return tag->artist;
}
const char *id3_getAlbum (const ID3Tag * tag)
{
	return tag->album;
}
const char *id3_getComment (const ID3Tag * tag)
{
	return tag->comment;
}

int id3_getTrack (const ID3Tag * tag)
{
	return tag->track;
}
const char *id3_getYear (const ID3Tag * tag)
{
	return tag->year;
}

unsigned int id3_getGenre (const ID3Tag * tag)
{
	return tag->genre;
}

/*
Func: id3_readid3v1 (ID3Tag * tag, FILE * fp)
Description: Reads an id3v1 tag and returns true
  or false depending on successfullness
*/
static bool id3_readid3v1 (ID3Tag * tag, FILE * fp)
{
	char    temp[ID3ARTIST + 1];	/* they are all 30 chars or less */
	bool    retval = true;

	/*
	   everything is 30 bytes long 
	 */
	fseek (fp, -128, SEEK_END);
	memset (tag, 0, sizeof (ID3Tag));
	memset (temp, 0, ID3ARTIST + 1);
	fread (temp, 1, TAGLEN, fp);
	if (strcmp (temp, "TAG") == 0)
	{

		fread (temp, 1, ID3SONGNAME, fp);
		id3_setSongname (tag, temp);
		fread (temp, 1, ID3ARTIST, fp);
		id3_setArtist (tag, temp);
		fread (temp, 1, ID3ALBUM, fp);
		id3_setAlbum (tag, temp);
		fread (temp, 1, ID3YEAR, fp);
		id3_setYear (tag, temp);
		fread (temp, 1, ID3COMMENT, fp);
		/*
		   Is there a track number in here ? 
		 */
		if (!temp[ID3COMMENT - 2] && temp[ID3COMMENT - 1])
		{
			debug ("Setting track number to: %d\n", (int) temp[ID3COMMENT - 1]);
			id3_setTrack (tag, temp[ID3COMMENT - 1]);
			temp[ID3COMMENT - 1] = '\0';

		}
		id3_setComment (tag, temp);
		fread (temp, 1, 1, fp);
		id3_setGenre (tag, temp[0]);

	}
	else
	{
		CExCreate (BAD_ID3TAG, "No ID3Tag present");
		retval = false;
	}

	return retval;
}

/*
Func: id3_writeid3v1 (ID3Tag * tag, FILE * fp)
Description: Write's an id3v1 tag, returns true or false
  depending on successfullness
*/
static bool id3_writeid3v1 (const ID3Tag * tag, FILE * fp)
{
	char    temp[ID3ARTIST];
	char    ch;
	fseek (fp, -128, SEEK_END);
	memset (temp, 0, sizeof (temp));
	fread (temp, 1, TAGLEN, fp);
	debug ("FREAD: %s\n", temp);
	if (strcmp (temp, "TAG"))			/* if it isn't right */
	{
		debug ("NO TAG");
		fseek (fp, 0, SEEK_END);
		fprintf (fp, "TAG");
	}
	write_tag (id3_getSongname (tag), ID3SONGNAME, fp);
	write_tag (id3_getArtist (tag), ID3ARTIST, fp);
	write_tag (id3_getAlbum (tag), ID3ALBUM, fp);
	write_tag (id3_getYear (tag), ID3YEAR, fp);
	if (id3_getTrack (tag))
	{
		ch = (char) id3_getTrack (tag);
		debug ("Writing tracknum\n");
		write_tag (id3_getComment (tag), ID3COMMENT - 1, fp);
		fwrite (&ch, 1, 1, fp);
	}
	else
		write_tag (id3_getComment (tag), ID3COMMENT, fp);

	ch = (char) id3_getGenre (tag);
	fwrite (&ch, 1, 1, fp);

	return true;
}

static int write_tag (const char *output, size_t size, FILE * fp)
{
	int     len = strlen (output);
	int     c;
	int     ret;
	if (len < size)
	{
		ret = fwrite (output, 1, len, fp);
		if (ret != EOF)
		{
			for (c = len; (c < size) && (ret != EOF); c++)
				ret = fputc (0, fp);
		}
		debug ("Wrote %d chars\n", c);
	}
	else
		ret = fwrite (output, 1, size, fp);

	return ret;
}
