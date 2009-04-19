#include <stdio.h>
#include <stdbool.h>
#include <globaldefs.h>
#include <flags.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <fdirkmisc.h>
#include <dlfcn.h>
#include <plugin.h>
#include <fdirkerrcode.h>
#include <libfdirk.h>

/*
 * Function - printUsage
 * Description - Prints the main usage for this program
 */
static void printUsage (const char *progname)
{
	printf ("%s version %s by Orbitz\n", progname, VERSION);
	printf
		("Usage:\n %s [general options] -p <plugin> dirname/filename "
		 "[plugin options]\n", progname);
	printf ("General Options:\n");
	printf ("  -h             This screen. -h along with -p will give that\n");
	printf ("                 plug-in's help screen.\n");
	printf ("  -r             Recursive, Go into subdirectories\n");
	printf ("  -a             Hidden files are catalogued\n");
	printf ("  -s             Follow symlinks, -a will not turn symbolic\n");
	printf ("                 links on. If -r is used then symlinks to\n");
	printf ("                 directories are followed too\n");
	printf ("  -l             List available plug-ins *UNSUPPORTED*\n");
	printf ("  -d <directory> Specify directory to look for plugin in\n");
	printf ("  -v             Verbose\n");
	printf ("  -p <plugin>    Use specific plugin\n");
	printf ("NOTE: Most of these flags only apply to directories\n");
}

/*
 * Function - main
 * Description - This initiates the program, handles the command line then
 *             sends the program control off to open the plugin and whatnot
 */
int main (int argc, char **argv)
{
	Flags   flags;								/* Holds the flags from command line */
	int     c;										/* Counting variable */
	char    filename[MAXNAMLEN + 1];	/* Holds the (dir/file)name from command line */
	Fileinfo finfo;
	int     retval;
	ArgvParser ap;
	char   *arguments[] =
		{ "plugin:?p", "verbose?v", "list?l", "help?h", NULL };

	/*
	   Plugin related variables 
	 */
	FPlugin pgn;

	memset (&flags, 0, sizeof (Flags));	/* Clears everything out */
	memset (&flags.filename, 0, MAXNAMLEN + 1);

	if (argc < 2)									/* If we don't have more than 2 arguments then quit */
	{
		printUsage (argv[0]);
		exit (0);
	}

	/*
	   Becaues I am a bum, I decided to implement my own command line flag
	   handler rather than installing/using getopt.  This does everything my way 
	   which is good because I wrote it, HAH.  Anyways, I may switch over to 
	   getopt thingy someday. There is soem redundancy here that should be 
	   taken care of.
	 */
	apSetArg (&ap, argc - 1, (const char **) &argv[1]);
	apSetOpt (&ap, "hrasld:vp:", (const char **) arguments);
	if (!parseCommandLine (&flags, &ap))
		goto badexit;

	/*
	   This section does the checking for us after, to make sure all required
	   flags set and also prints out the appropriate help screens.
	 */

	if (flags.help && !flags.plugin[0])	/* They want the general help screen */
	{
		printUsage (argv[0]);
		exit (0);
	}

	if (!flags.help && !flags.filename[0])
	{
		CExCreate (INPUT_E, "Directory or filename required");
		goto badexit;
	}

	/*
	   THIS CHUNK NEEDS SOME MOVIN 
	 */
	strncat (flags.plugin, ".so", 4);	/* Add .so to the end of the plugin */

	/*
	   Load The Plugin 
	 */
	retval = pluginLoad (&pgn, flags.plugin, flags.plugindir);

	if (!retval)
		goto badexit;

	/*
	   Let's check if they just wanted a help screen and give it to them 
	 */
	if (flags.help)
	{
		pgn.pluginUsage ();
		exit (0);
	}



	if (!pgn.pluginInit (&ap))		/* Initilize the plugin */
		goto badexit;

	if (isDir (flags.filename))
	{
		if (!catalogueDir (flags.filename, &flags, &pgn))
			goto badexit;

	}
	else
	{
		fiSetFilename (&finfo, flags.filename);
		if (haveAccess (flags.filename) && fiGetFileinfo (&finfo, flags.filename))
		{
			if (!pgn.fileCatalogue (&finfo))
				goto badexit;
		}
		else
			fprintf (stderr, "Error processing file\n");

	}

	pgn.pluginClose ();						/* Call the end function in the plugin */
	pluginUnload (&pgn);					/* Get rid of the plugin from memory */
	bc_free_all ();
	return EXIT_SUCCESS;
badexit:
	fprintf (stderr, "%s\n", CExGetErrmsg (CExGetException ()));
	bc_free_all ();
	return CExGetCode (CExGetException ());
}
