/*
 * These are various plugin functions that the plugin writer will probebly
 * need.  Put them here so they are standard and don't have to be
 * rewritten everytime a plugin is written.
 */
#define _GNU_SOURCE
#define __USE_POSIX
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <plugindefs.h>
#include <cexception.h>
#include <fdirkerrcode.h>

/*
 * Function - file_exists(char *filepath)
 * Return - true or false if file exists
 */
bool file_exists (const char *filepath)
{
	struct stat sb;
	bool    retval;

	if (stat (filepath, &sb) == -1)
		retval = false;
	else
		retval = true;

	return retval;
}

/*
 * Function - getBasename(char *filepath, char delim)
 * Return - A pionter to the part of the string where the actualy file name
 *         starts.  Delim is whatever character is the directory divider.
 *         This function does not create a seperate string, just a pointer
 *         to the place in the string where the filename begins.
 */
char   *getBasename (const char *filepath, int delim)
{
	int     c = strlen (filepath);
	char   *ret;

	while ((filepath[c - 1] != delim) && (c > 0))
		c--;

	ret = &filepath[c];

	return ret;
}

/*
 * Function - getFilepath(char *filepath, char delim)
 * Return - A new string that contains the filepath.  String must be freed
 *       when done unless you like using up extra space
 */
char   *getFilepath (const char *filepath, int delim)
{
	char   *temp = strdup (filepath);
	char   *basename = getBasename (temp, delim);
	char   *ret;

	basename[0] = '\0';

	ret = bc_malloc (strlen (temp) + 1);
	sprintf (ret, "%s", temp);
	free (temp);

	return ret;
}


/*
 * Function - getVarValue(Fileinfo *finfo, int varname)
 * Return - Returns the a string containing the value of the variable
 *         that was requested. Stores it in static variable
 */
char   *getVarValue (const Fileinfo * finfo, int varname)
{
	char   *ret;

	switch (varname)
	{
	case 'f':
		ret =
			bc_malloc (strlen (getBasename (fiGetFilename (finfo), DIRDELIM)) + 1);
		sprintf (ret, "%s", getBasename (fiGetFilename (finfo), DIRDELIM));
		break;
	case 'F':
		ret = bc_malloc (strlen (fiGetFilename (finfo)) + 1);
		sprintf (ret, "%s", fiGetFilename (finfo));
		break;

	case 'd':
		ret =
			bc_malloc (strlen (getFilepath (fiGetFilename (finfo), DIRDELIM)) + 1);
		sprintf (ret, "%s", getFilepath (fiGetFilename (finfo), DIRDELIM));
		break;

	case 's':
		ret = bc_malloc (LLONG_DIGITS + 1);
		snprintf (ret, LLONG_DIGITS, "%lld", fiGetFilesize (finfo));
		break;

	case 'A':
		ret = bc_malloc (27);
		ret = ctime_r (&finfo->atime, ret);
		ret[strlen (ret) - 1] = '\0';
		break;
	case 'M':
		ret = bc_malloc (27);
		ret = ctime_r (&finfo->mtime, ret);
		ret[strlen (ret) - 1] = '\0';
		break;
	case 'C':
		ret = bc_malloc (27);
		ret = ctime_r (&finfo->mtime, ret);
		ret[strlen (ret) - 1] = '\0';
		break;

	default:
		CExCreatef (INPUT_E, "Unknown Variable option: %c", varname);
		ret = NULL;
	}

	return ret;
}

/*
Func: getVarName(const char varname)
Desc: Returns a 'column name' for the variable
*/
const char *getVarName (int varname)
{
	switch (varname)
	{
	case 'f':
		return "Base Name";
		break;
	case 'F':
		return "Filepath";
		break;
	case 'd':
		return "Directory Name";
		break;
	case 'A':
		return "Access Time";
		break;
	case 'C':
		return "Change Time";
		break;
	case 'M':
		return "Modified Time";
		break;

	default:
		CExCreatef (VARNAMEUNK_E, "%c is an unknown variable name", varname);
		return NULL;
	}
}
