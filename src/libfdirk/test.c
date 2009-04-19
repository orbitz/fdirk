#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "argvparser.h"
#include "cexception.h"

struct flags
{
	char   *file;
	bool    num;
	char   *test;
};

void usage (void)
{
	printf ("test [--file|-f file] [--test2 test] [-n]\n\n");
}

int main (int argc, char *argv[])
{
	char   *acceptargs[] = { "file:?f", "test2::", NULL };
	struct flags myflags;
	ArgvParser ap;
	char    ch;

	memset (&myflags, 0, sizeof (struct flags));
	apSetArg (&ap, argc - 1, &argv[1]);
	apSetOpt (&ap, "f:n:", acceptargs);

	while ((ch = apGetNextOpt (&ap)) != AP_DONE)
	{
		switch (ch)
		{
			CException cex;

		case AP_ERR:
			usage ();
			if (cex = CExGetException ())
			{
				printf ("Error: %s\n", CExGetErrmsg (cex));
				CExDestroy (cex);
			}
			exit (1);
			break;
		case AP_LONG:
			debug ("opt: %s arg %s\n", apGetLongOpt (&ap), apGetOptArg (&ap));
			if (strcmp (apGetLongOpt (&ap), "test2") == 0)
				myflags.test = apGetOptArg (&ap);
			break;
		case AP_FREE:
			debug ("Dangling argument: opt: %s\n", apGetOptArg (&ap));
			break;
		case 'f':
			myflags.file = apGetOptArg (&ap);
			break;
		case 'n':
			myflags.num = true;
			break;
		case '?':
			printf ("unknown option\n");
			break;
		}
	}

	printf ("%s %d %s\n", myflags.file, myflags.num, myflags.test);
	return 0;
}
