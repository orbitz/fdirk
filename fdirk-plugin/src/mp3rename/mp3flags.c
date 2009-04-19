#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>

#include <debug.h>
#include <mp3flags.h>
#include <miscfuncs.h>


bool mp3fCreate (MP3Flags * mf)
{
	memset (mf, 0, sizeof (MP3Flags));
	return true;
}

void mp3fDestroy (MP3Flags * mf)
{
	memset (mf, 0, sizeof (MP3Flags));
}

bool mp3fSetAssume (MP3Flags * mf, bool assume)
{
	mf->assume = assume;
	return true;
}

bool mp3fGetAssume (const MP3Flags * mf)
{
	return mf->assume;
}
