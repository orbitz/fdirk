#include <stdio.h>
#include <fdirk.h>
#include <boolean.h>
#include <html4.h>

#define PLUGINVERSION "0.1 beta"

void fdirkPluginUsage (void)
{
	printf ("html4 plugin version %s by Orbitz\n", PLUGINVERSION);
	printf ("Options:\n");
	printf ("  -o              Over write the output file if it exists\n");
	printf ("  -f <filename>   File to output to, - for stdout\n");
	printf ("  -t <title>      What goes in the <TITLE> tags\n");
	printf ("  --fmt <format>  Format for output\n");
	printf ("Example:\n");
	printf (" -f fubar.html --fmt \"FILE:%%f SIZE:%%s DISK:5\"\n");
	printf ("These options will create a file called fubar.html with ");
	printf ("a table in it with the column names FILE, SIZE, and DISK.  ");
	printf ("Then it will insert the filename, size, and number 5 in for ");
	printf ("the record of each file. A list of all the generic ");
	printf ("identifiers can be found in the main help screen.\n\n");
}

/*
 * Function - fdirkInitPlugin
 * Description - initialises the plugin, handles any input
 * NOTE: should be edited so there are no fprintf's, or exit()'s
 */
void   *fdirkInitPlugin (int argc, char **argv)
{
	int     c;
	boolean overwrite = FALSE;
	char   *filename;
	char   *title;
	char   *format;
	struct html4_t *phtml4 = malloc (sizeof (struct html4_t));

	for (c = 0; c < argc; c++)
		{
			if (argv[c][0] == '-')
				{
					switch (argv[c][1])
						{
						case 'f':
							if ((c + 1) < argc)
								{
									filename = argv[c + 1];
									c++;					/* Advance the counter beyond the options arg */
								}
							else
								goto badparamsdie;
							break;
						case 't':
							if ((c + 1) < argc)
								{
									title = argv[c + 1];
									c++;
								}
							else
								goto badparamsdie;
							break;
						case 'o':
							overwrite = TRUE;
							break;
						case '-':
							if ((c + 1) < argc)
								{
									if (strncmp ("fmt", &argv[c][2], strlen (&argv[c][2])))
										format = argv[c + 1];
								}
							else
								goto badparamsdie;
							break;
						default:
							fprintf (stderr, "Error in plugin\n");
							exit (-1);
							break;
						badparamsdie:
							fprintf (stderr, "Error parsing command line\n");
							exit (-1);
						}
				}
		}

	if (strncmp ("-", (char *) &filename, strlen (argv[c + 1])))
		phtml4->fptr = stdout;
	else
		{
			if (!file_exists (filename) || (file_exists (filename) && overwrite))
				{
					/*
					   Add code to check for if file exists 
					 */
					phtml4->fptr = fopen ((char *) &filename, "w");
					if (phtml4->fptr == NULL)
						{
							fprintf (stderr, "Error opening file for write\n");
							exit (-1);
						}
				}
		}


	fprintf (phtml4->fptr, "<HTML>\n<BODY>\n");
	fprintf (phtml4->fptr, "<TITLE>%s</TITLE>\n", title);
	return phtml4;
}

void fdirkClosePlugin (void *pData)
{
	struct html4_t *phtml = (struct html4_t *) pData;
	fclose (phtml->fptr);
	free (phtml);
	pData = NULL;
}

boolean file_exists (char *filename)
{
				/* Fix this */

				return FALSE;
}
