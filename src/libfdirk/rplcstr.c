#include <stdio.h>
#include <string.h>
#include <buffcontainer.h>

char   *rplcstr (const char *search, const char *find, const char *replace)
{
	const char *sptr = search;
	int     newstrlen = strlen (search) + 1;
	int     rplclen = strlen (replace) + 1;
	int     findlen = strlen (find);
	int     strdiffs = (rplclen - 1) - findlen;
	int     newstrpos = 0;
	char   *newstr = bc_malloc (newstrlen);
	char   *temp;

	if (findlen == 0)
		goto badexit;

	while (*sptr)
	{
		for (; *sptr != find[0] && *sptr; sptr++, newstrpos++)
			newstr[newstrpos] = *sptr;

		if (*sptr && (strstr (sptr, find) == sptr))
		{
			if (strdiffs > 0)
			{
				newstrlen += strdiffs;
				temp = bc_realloc (newstr, newstrlen);
				if (temp)
					newstr = temp;
				else
					goto badexit;
			}

			for (temp = (char *) replace; *temp; temp++, newstrpos++)
				newstr[newstrpos] = *temp;
			sptr += findlen;
		}
		else if (*sptr)
			newstr[newstrpos++] = *(sptr++);

	}

	newstr[newstrpos] = '\0';

	temp = bc_realloc (newstr, newstrpos + 1);
	if (temp)
		newstr = temp;

	return newstr;
badexit:
	bc_free (newstr);
	return NULL;
}
