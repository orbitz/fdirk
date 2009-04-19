#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <mp3errcodes.h>
#include <renamebyid3.h>
#include <id3tag.h>

static void chopFilePath (const char *input, char *filepath, char *filename)
{
	int     end = strlen (input);
	int     c;
	char   *temp = malloc (sizeof (char) * (strlen (input) + 1));

	strncpy (temp, input, strlen (input));

	memset (filepath, 0, sizeof (MAXNAMLEN + 1));
	memset (filename, 0, sizeof (MAXNAMLEN + 1));
	for (c = end; (temp[c] != '/') && (c >= 0); c--);

	if (c < 0)
	{
		snprintf (filename, MAXNAMLEN, "%s", temp);
		snprintf (filepath, MAXNAMLEN, "./");
	}
	else
	{
		/*
		   strncpy (filename, &temp[c + 1], end - c);
		 */
		snprintf (filename, end - c, "%s", &temp[c + 1]);
		temp[c + 1] = 0;
		/*
		   strncpy (filepath, temp, c + 1);
		 */
		snprintf (filename, c + 1, "%s", temp);
	}

	free (temp);

}

int getID3Tag (ID3Tag * myTag, const char *filename)
{
	FILE   *fp = fopen (filename, "r");
	char    id3[31];
	if (fp == NULL)
		return BAD_FILENAME;

	fseek (fp, -128, SEEK_END);
	memset (myTag, 0, sizeof (ID3Tag));
	memset (id3, 0, sizeof (id3));
	fread (id3, 1, TAGLEN, fp);
	if (strcmp (id3, "TAG") == 0)
	{
		memset (&id3, 0, sizeof (id3));
		fread (id3, 1, ID3SONGNAME, fp);
		id3_setSongname (myTag, id3);
		memset (&id3, 0, sizeof (id3));
		fread (id3, 1, ID3ARTIST, fp);
		id3_setArtist (myTag, id3);
		cleanID3Tag (myTag);
	}
	else
	{
		fclose (fp);
		return BAD_ID3TAG;
	}
	fclose (fp);
	return 1;
}

bool renameMP3 (const char *oldfilename, const ID3Tag * myTag)
{
	char    newfilename[MAXNAMLEN + 1];
	char    dir[MAXNAMLEN + 1];
	char    filename[MAXNAMLEN + 1];
	FILE   *fp;
	int     len;

	chopFilePath (oldfilename, dir, filename);


	len =
		snprintf (newfilename, MAXNAMLEN + 1, "%s%s_-_%s.mp3", dir,
							id3_getArtist (myTag), id3_getSongname (myTag));
	if (len <= MAXNAMLEN + 1)
	{
		if ((fp = fopen (newfilename, "r")) == NULL)
			rename (oldfilename, newfilename);
		else
		{
			fclose (fp);
			return false;
		}
	}
	else
		return false;

	return true;
}


void cleanID3Tag (ID3Tag * myTag)
{
				/* Evil code */
	int     c;
	for (c = ID3SONGNAME - 1; myTag->songname[c] == ' '; c--)
		myTag->songname[c] = 0;

	for (c = ID3ARTIST - 1; myTag->artist[c] == ' '; c--)
		myTag->artist[c] = 0;

	/*
	   This is only here because ID3SONGNAME and ID3ARTIST
	   the same length 
	 */
	for (c = 0; c < ID3SONGNAME; c++)
	{
		if (myTag->artist[c] == ' ')
			myTag->artist[c] = '_';
		if (myTag->songname[c] == ' ')
			myTag->songname[c] = '_';
		if (myTag->artist[c] == '/')
			myTag->artist[c] = '_';
		if (myTag->songname[c] == '/')
			myTag->songname[c] = '_';

	}

}
