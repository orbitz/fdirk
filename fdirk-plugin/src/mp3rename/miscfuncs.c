#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <debug.h>

#include <miscfuncs.h>


bool hasMP3Ext (const char *filename)
{
	int     len = strlen (filename);

	if (strcmp (&filename[len - 4], ".mp3") == 0)
		return true;

	return false;
}

bool strset (char **dst, const char *src)
{
	if (*dst)
	{
		debug ("freeing\n");
		free (*dst);
	}

	*dst = strdup (src);
	if (!(*dst))
		return false;
	debug ("dst = %s\n", *dst);

	return true;
}

bool strncopy (char *dst, const char *src, unsigned int size)
{
	unsigned int c;
	if (strlen (src) > size)
		return false;
	else
	{
		c = snprintf (dst, size, "%s", src);
		if (c < strlen (src))
			return false;
		if (c < size)
			memset (dst + c, 0, size - c);
	}

	return true;
}
