#ifdef LINUX_OS
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif													/* LINUX_OS */

#include <string.h>
#include <fdirkmisc.h>
#include <globaldefs.h>
#include <fileinfo.h>

#ifdef DEBUG
#include <stdio.h>
#endif													/* DEBUG */

/*
 * Functions
 */

/*
 * Name - isDir
 * Parameters - char * dirname
 * Return - bool, true or false diranem is a directory or not
 * NOTE - this is NOT static beacuse it is used elsewhere in the program
 */
bool isDir(const char *dirname)
{
	struct stat fstats;
	int     retval;

	retval = stat(dirname, &fstats);
	if (retval == 0)
	{
#ifdef BSD_OS
		if ((fstats.st_mode & S_IFDIR) == S_IFDIR)
#endif													/* BSD_OS */
#ifdef LINUX_OS
			if (S_ISDIR(fstats.st_mode))
#endif													/* LINUX_OS */
			{
				retval = true;
			}
			else
			{
				retval = false;
			}
	}
	return (bool) retval;
}


/*
 * Function - haveAccess
 * Return - true or false if teh usre has access
 */
bool haveAccess(const char *dirname)
{
	bool    retval = false;
	int     readAcc;

	readAcc = access(dirname, R_OK);
	if (readAcc == 0)
	{
		retval = true;
	}

	return retval;
}

/*
 * Function - appendSlash
 * Return - Pointer to a string with a slash appended to it *ONLY IF ONE WAS
 *					NOT PRESENT*
 */
const char *appendSlash(char *myString)
{
	char   *retString;
	int     len;

	if (myString[strlen(myString) - 1] != '/')
	{
		retString = malloc(strlen(myString) + 2);

		strncpy(retString, myString, strlen(myString));
		len = strlen(myString);
		/*
		 * Fix FOr portability 
		 */
		retString[len] = '/';
		retString[len + 1] = 0;
		free(myString);
	}
	else
	{
		retString = myString;
	}

	return retString;
}
