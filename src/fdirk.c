#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <fdirkmisc.h>
#include <stack.h>
#include <globaldefs.h>
#include <cexception.h>
#include <fileinfo.h>


/*
 * Functions
 */
/*
 * Function - openDirectory
 * Return - true or false on sucess or failure of openign the directory
 */
static bool openDirectory (const char *dirname, DIR ** dirp)
{
	bool    retval = true;
	*dirp = opendir (dirname);

	if (*dirp == NULL)
		retval = false;

	return retval;
}

/*
 * Function - prependDirname
 * Description - Puts dirname infront of filename
 */
static bool prependDirname (const char *dirname, char *filename)
{
	char    temp[MAXNAMLEN + 1];
	snprintf ((char *) &temp, (size_t) MAXNAMLEN, "%s%s", dirname, filename);
	strncpy (filename, temp, (size_t) MAXNAMLEN);

	return true;
}

/*
 * Function - getNextFile
 * Description - Gets the next file from the directory stream and gets it's
 *							information.
 * Return - true or false on success or failure
 */
static int getNextFile (char *filename, DIR * dirp)
{
	struct dirent *dp;
	int     len;
	int     retval = 1;

	dp = readdir (dirp);
	if (dp)
	{
		/*
		   This junk is here to avoid reading . and .. as directorys 
		 */
		len = strlen (dp->d_name);
		while (dp != NULL && ((strncmp (dp->d_name, ".", (size_t) len) == 0) ||
													(strncmp (dp->d_name, "..", (size_t) len) == 0)))
		{
			dp = readdir ((DIR *) dirp);

			if (dp)
				len = strlen (dp->d_name);
			else
				retval = dp;
		}

		if (dp != NULL)
		{
			/*
			   This stuff is just for testing purposes right now 
			 */
			strncpy (filename, dp->d_name, (size_t) len + 1);
		}

	}
	else if(!dp)
					retval = dp;
	else
	{
		CExCreate (NULLVAL_E, "Error reading directory");
		retval = dp;
	}

	return retval;
}


/*
 * Function - catalogueDir
 * Description - Catalogues a directory
 */
bool catalogueDir (const char *dirname, const Flags * flags,
									 const FPlugin * pgn)
{
	Stack   fileStack;
	Fileinfo fileInfo;
	char   *temp = 0;
	int     retval, done = 0;
	DIR    *dirp = 0;


	stackCreate (&fileStack);
	if (stackPush (&fileStack, dirname, strlen (dirname) + 1) == false)
		goto badexit;

	while (!done)
	{
		temp = malloc (stackGetTopSize (&fileStack));
		stackPop (&fileStack, temp);

		retval = openDirectory (temp, &dirp);
		if (retval == false)
		{
			CExCreate (DIR_E, "Error opening directory");
			goto badexit;
		}

		temp = appendSlash (temp);

		/*
		   The following code DEFINATLY need some work 
		 */
		/*
		   These if statements are very explicit fori a reason 
		 */
		retval = getNextFile (fileInfo.filename, dirp);
		while (retval)
		{
			prependDirname (temp, fileInfo.filename);
			if (haveAccess (fileInfo.filename))
			{
				retval = fiGetFileinfo (&fileInfo, fiGetFilename (&fileInfo));
				switch (fiGetType (&fileInfo))
				{
				case T_DIR:
					if (flags->subdirs)
					{
						if (!fiIsHidden (&fileInfo) ||
								(fiIsHidden (&fileInfo) && flags->hidden))
						{
							if (stackPush
									(&fileStack, fileInfo.filename,
									 strlen (fileInfo.filename) + 1) == false)
								goto badexit;
						}
					}
					break;
				case T_REG:
					if (!fiIsHidden (&fileInfo) ||
							(fiIsHidden (&fileInfo) && flags->hidden))
					{
						if(!pgn->fileCatalogue (&fileInfo))
							goto badexit;
					}
					break;
				case T_SYM:
					retval = isDir (fileInfo.filename);
					if (flags->symlink)
					{
						if (retval && flags->subdirs)
						{
							if (!fiIsHidden (&fileInfo) ||
									(flags->hidden && fiIsHidden (&fileInfo)))
							{
								if (stackPush
										(&fileStack, fileInfo.filename,
										 strlen (fileInfo.filename) + 1) == false)
									goto badexit;
							}
						}
						else
							if ((!retval
									 && !fiIsHidden (&fileInfo))
									|| (!retval && flags->hidden && fiIsHidden (&fileInfo)))
						{
							if(!pgn->fileCatalogue (&fileInfo))
								goto badexit;
						}
					}

					break;
				}
			}
			else
			{
			}
			retval = getNextFile (fileInfo.filename, dirp);
		}

		if (stackGetNumElm (&fileStack) == 0)
			done = 1;

		free (temp);
		closedir (dirp);
	}
	stackDestroy (&fileStack);

	return true;
badexit:
	return false;
}
