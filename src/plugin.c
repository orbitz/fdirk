#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <globaldefs.h>
#include <plugin.h>
#include <dlfcn.h>
#include <cexception.h>
#include <fdirkerrcode.h>
#include <debug.h>

/* Random functions we'll need */
char   *getNextPath (const char *path)
{
	static char buf[MAXNAMLEN + 1];
	char   *retval = buf;
	static char *start, *samebuf = NULL;
	char   *end;

	if (samebuf != path)
	{
		samebuf = path;
		start = samebuf;
	}


	memset (retval, 0, MAXNAMLEN + 1);
	end = strchr (start, ':');

	if (strlen (start) > 0)
	{
		if (!end)
			end = start + strlen (start);

		if ((end - start) <= MAXNAMLEN)
		{
			snprintf (retval, (unsigned) (end - start) + 1, "%s", start);
			if (!(*end))
				start = end;
			else
				start = end + 1;
			debug ("retval: %s\n", retval);
		}
		else
			retval = NULL;
	}
	else
		retval = NULL;

	return retval;
}

void   *tryAllLocations (const char *plugin, const char *extrapath)
{
	char    pname[MAXNAMLEN + 1];
	char   *ret;
	void   *handle;
	if (extrapath != NULL)
	{
		/*
		   Try to open everything in the extrapath var 
		 */
		while ((ret = getNextPath (extrapath)))
		{
			/*
			   we can do this because we know it is less than MAXNAMLEN allready 
			 */
			sprintf (pname, "%s", ret);
			if (pname[strlen (pname) - 1] != '/')
			{
				int     len = strlen (pname);
				pname[len] = '/';
				pname[len + 1] = '\0';
			}
			snprintf (&pname[strlen (pname)], MAXNAMLEN - strlen (pname), "%s",
								plugin);
			debug ("plugin name: %s\n", pname);
			if ((handle = dlopen (pname, RTLD_LAZY)))
				return handle;
		}
	}

	/*
	   That didn't work, try the default directory 
	 */
	snprintf (pname, MAXNAMLEN, "%s%s", FDIRK_PLUGIN_PATH, plugin);
	debug ("plugin name: %s\n", pname);
	if ((handle = dlopen (pname, RTLD_LAZY)))
		return handle;

	/*
	   Finally, try the current directory 
	 */
	if (getcwd (pname, MAXNAMLEN))
	{
		snprintf (&pname[strlen (pname)], MAXNAMLEN - strlen (pname), "/%s",
							plugin);
		debug ("plugin name: %s\n", pname);
		if ((handle = dlopen (pname, RTLD_LAZY)))
			return handle;
	}

	/*
	   Nothign worked! 
	 */
	return NULL;

}

/*
 * These are various wrappers for the dlopen, dlclose, dlerror, and dlsym
 * functions.
 */

/*
 * Function - pluginSym
 * Parameters - handle - handle to the open shared object
 *              function - function name we are getting the pointer to
 * Description - Simple wrapper for dlsym, trys to get pointer to a function
 *              in a shared object that is allready open.
 */
static void *pluginSym (const void *handle, char *funcName)
{
	void   *funchandle;
	char   *funcname;

/*#ifdef LINUX_OS*/
	funcname = funcName;
	funchandle = dlsym (handle, funcname);
	debug ("funcname - %s\n", funcname);
/*#endif */ /* LINUX_OS */
/*#ifdef BSD_OS
	funcname = malloc (strlen (funcName) + 2);
	funcname[0] = '\0';
	strcat (funcname, "_");
	strcat (funcname, funcName);
	funchandle = dlsym (handle, funcname);
	debug ("funcname - %s\n", funcname);
	free (funcname);
#endif*/ /* BSD_OS */

	return funchandle;
}

/*
 * Function - pluginOpen
 * Parameters - plugin - the name of the plugin, can be absolute path
 *              extrapath - this can also be the absolute path do the dir
 *                          the object is in.
 * Description - Returns the handle given by dlopen depending on the success 
 *              of opening the plugin.
 */
int pluginLoad (FPlugin * pgn, const char *plugin, const char *extrapath)
{
	char   *pErr;
	bool    retval = true;
	char    pname[MAXNAMLEN + 1];

	memset (pgn, 0, sizeof (FPlugin));

	pgn->handle = tryAllLocations (plugin, extrapath);
	if (!pgn->handle)
	{
		retval = false;
		pErr = dlerror();
		goto badexit;
	}

	pgn->pluginInit = pluginSym (pgn->handle, "InitPlugin");
	pErr = dlerror ();
	if (pErr != NULL)
	{
		retval = false;
		pluginUnload (pgn);
		goto badexit;
	}

	pgn->fileCatalogue = pluginSym (pgn->handle, "CatalogueFile");
	pErr = dlerror ();
	if (pErr != NULL)
	{
		retval = false;
		pluginUnload (pgn);
		goto badexit;
	}

	pgn->pluginClose = pluginSym (pgn->handle, "ClosePlugin");
	pErr = dlerror ();
	if (pErr != NULL)
	{
		retval = false;
		pluginUnload (pgn);
		goto badexit;
	}

	pgn->pluginUsage = pluginSym (pgn->handle, "Usage");
	pErr = dlerror ();
	if (pErr != NULL)
	{
		retval = false;
		pluginUnload (pgn);
		goto badexit;
	}

badexit:
	if (pErr)
		CExCreatef (PLUGIN_E, "Error loading plugin: %s", pErr);
	return retval;
}

/*
 * Function - pluginClose
 * Parameters - handle - handle of the plugin we are closing
 * Description - Closes the plugin that is loaded in handle, returns TRUE
 *             all the time, just incase somethign is added where it would 
 *             have to return FALSE;
 */
bool pluginUnload (FPlugin * pgn)
{
	if (pgn->handle)
		dlclose (pgn->handle);
	return true;
}

/*
 * Function - pluginError
 * Description - returns a pointer to the string returned by dlerror.
 * OBSOLETE
 */
/*
const char *pluginError (void)
{
	return dlerror ();
}
*/
