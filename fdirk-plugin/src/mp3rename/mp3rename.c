#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <fileinfo.h>
#include <debug.h>
#include <argvparser.h>
#include <fdirkerrcode.h>
#include <cexception.h>

#include <id3tag.h>
#include <mp3flags.h>
#include <miscfuncs.h>

#define VERSION "0.0.1 beta"

static MP3Flags flags;
static ID3Tag changetag;

void Usage (void)
{

	printf ("mp3rename fdirk plugin version %s\n", VERSION);
	printf ("Flags:\n");
	printf
		("  -a             Assume all input is mp3's rather than checking extension\n");
	printf ("  -N artist      Set the id3 artist value\n");
	printf ("  -S song        Set the id3 songname value\n");
	printf ("  -T track       Set the id3 track num value\n");
	printf ("  -A album       Set the id3 album value\n");
	printf ("  -C comment     Set the id3 comments valus\n");
	printf ("  -Y year        Set the id3 year value\n");
	printf ("  -G genre       Set the id3 genre value\n");
	printf ("  -r \"format\"    Set the rename format\n");

}

bool InitPlugin (ArgvParser * ap)
{
	char   *args[] = { NULL };		/* No long format args right now */
	char    ch;
	bool    ret;
	bool    done = false;

	mp3fCreate (&flags);
	id3_create (&changetag);
	id3_setGenre (&changetag, 80);	/* right now it this si for id3_copyvals */

	apSetOpt (ap, "aN:S:T:A:C:Y:G:r:", args);
	while (!done && ((ch = apGetNextOpt (ap)) != AP_DONE))
	{
		switch (ch)
		{
		case 'a':
			ret = mp3fSetAssume (&flags, true);
			break;
		case 'N':
			ret = id3_setArtist (&changetag, apGetOptArg (ap));
			break;
		case 'S':
			ret = id3_setSongname (&changetag, apGetOptArg (ap));
			break;
		case 'T':
			ret = id3_setTrack (&changetag, strtol (apGetOptArg (ap), NULL, 10));
			break;
		case 'A':
			ret = id3_setAlbum (&changetag, apGetOptArg (ap));
			break;
		case 'C':
			ret = id3_setComment (&changetag, apGetOptArg (ap));
			break;
		case 'Y':
			ret = id3_setYear (&changetag, apGetOptArg (ap));
			break;
		case 'G':
			ret = id3_setGenre (&changetag, strtol (apGetOptArg (ap), NULL, 10));
			break;
		case 'r':
			debug ("rename format not implemented yet\n");
			break;
		case AP_UNK:
			debug ("Unknown option: %c\n", ch);
			break;
		case AP_FREE:
			break;
		case AP_LONG:
			break;
		case AP_ERR:
		default:
			done = 1;
			debug ("Unknown option or error\n");
		}
		if (!ret)
		{
			CExCreatef (MEM_E, "Error processing %c", ch);
			done = true;
		}
	}

	return true;
}

bool CatalogueFile (const Fileinfo * finfo)
{
	ID3Tag  tag;
	bool    retval = true;

	if (mp3fGetAssume (&flags) || hasMP3Ext (fiGetFilename (finfo)))
	{
		const char *temp;
		bool    changedid3;

		id3_create (&tag);

		id3_read (&tag, fiGetFilename (finfo), ID3V1);
		changedid3 = id3_copyvals (&tag, &changetag);
		if (retval)
		{
			debug ("Artist: %s\n", id3_getArtist (&tag));
			debug ("Songname: %s\n", id3_getSongname (&tag));
			debug ("Album: %s\n", id3_getAlbum (&tag));
			debug ("Comment: %s\n", id3_getComment (&tag));
			debug ("Track: %d\n", (int) id3_getTrack (&tag));
		}

		if (changedid3)
		{
			debug ("Setting ID3 tag\n");
			if (!id3_write (&tag, fiGetFilename (finfo), ID3V1))
				debug ("Error writing id3 tag!! file: %s\n", fiGetFilename (finfo));
		}

		id3_destroy (&tag);

	}


	return true;
}

void ClosePlugin (void)
{
	mp3fDestroy (&flags);
}
