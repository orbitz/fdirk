#define _GNU_SOURCE
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <flags.h>
#include <argvparser.h>

bool parseCommandLine (Flags * flags, ArgvParser * ap)
{
	int     ch;
	bool    done = false;
	while (!done && ((ch = apGetNextOpt (ap)) != AP_DONE))
	{
		switch (ch)
		{
		case AP_FREE:
			snprintf (flags->filename, MAXNAMLEN, "%s", apGetOptArg (ap));
			done = true;
			break;
		case 'h':
			flags->help = true;
			break;
		case 'r':
			flags->subdirs = true;
			break;
		case 's':
			flags->symlink = true;
			break;
		case 'a':
			flags->hidden = true;
			break;
		case 'd':
			flags->plugindir = apGetOptArg (ap);
			break;
		case 'v':
			flags->verbose = true;
			break;
		case 'p':
			snprintf (flags->plugin, MAXNAMLEN, "%s", apGetOptArg (ap));
			break;
		case AP_ERR:
		default:
			return false;
		}
	}

	return true;
}
