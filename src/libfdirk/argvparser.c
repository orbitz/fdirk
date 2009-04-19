#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <argvparser.h>
#include <cexception.h>
#include <debug.h>

#ifndef NUL
#define NUL ((char)0)
#endif


/* random functions we'll need */
static bool isLongOpt (const ArgvParser * ap);
static char *findLongOpt (const char **searchin, const char *searchfor);
static char *getLongOptArg (ArgvParser * ap);
static bool requireLongOptArg (const char *opt);
static bool wantLongOptArg (const char *opt);
static char shortLongArgForm (const char *opt);
static bool isShortOpt (ArgvParser * ap);
static bool wantShortOptArg (const char *opt);
static bool requireShortOptArg (const char *opt);
static char *getShortOptArg (ArgvParser * ap);
static char *findShortOpt (const char *shortopt, char curropt);
static void handleLongOpt (ArgvParser * ap);
static void handleShortOpt (ArgvParser * ap);
static bool isDanglingOpt (const ArgvParser * ap);
static void handleDanglingOpt (ArgvParser * ap);

/*
Function: void apSetArg (ArgvParser * ap, int argc, const char **argv)
Description: Set's the argument count, and strings
	it also resets the count variables.
*/
void apSetArg (ArgvParser * ap, int argc, const char **argv)
{
	ap->argc = argc;
	ap->argv = (char **) argv;
	ap->argcp = 0;								/* Should fix this */
	ap->argvp = 0;
}

/*
Function: void apSetOpt (ArgvParser * ap, const char *shortopt, const char **longopt)
Description: Sets the options to scan for, both long and short
*/
void apSetOpt (ArgvParser * ap, const char *shortopt, const char **longopt)
{
	ap->shortopt = (char *) shortopt;
	ap->longopt = (char **) longopt;
}

/*
Function: void apReset (ArgvParser * ap)
Description: Resets the counts to 0.
*/
void apReset (ArgvParser * ap)
{
	ap->argcp = 0;								/* this too */
	ap->argvp = 0;
}

/*
Function: char apGetOpt (const ArgvParser * ap)
Description: returns the current opt, '-' if a longopt
*/
char apGetOpt (const ArgvParser * ap)
{
	return ap->curropt;
}

/*
Function: const char *apGetOptArg (const ArgvParser * ap)
Description: Returns a const char * to the options argument. NULL if
	there is none
*/
const char *apGetOptArg (const ArgvParser * ap)
{
	return ap->optarg;
}

/*
Function: const char *apGetLongOpt (const ArgvParser * ap)
Description: Returns a const char * to the long option if exists.
	returns NULL if there is none
*/
const char *apGetLongOpt (const ArgvParser * ap)
{
	return ap->fullopt;
}

/*
Function: char apGetNextOpt (ArgvParser * ap)
Description: Gets the next operand and argument. if the argument
	is a single letter, it returnst hat, if not it returns '-' and use
	apGetLongOpt() to read it.  NUL is returned on bad argument
*/
int apGetNextOpt (ArgvParser * ap)
{
	if (ap->argcp < ap->argc)
	{
		if (isLongOpt (ap))
		{
			handleLongOpt (ap);
		}
		else if (isShortOpt (ap))
		{
			handleShortOpt (ap);
		}
		else if (isDanglingOpt (ap))
		{
			handleDanglingOpt (ap);
		}
		else
		{
			debug ("Unknown situation!\n");
			ap->curropt = NUL;
		}
	}
	else
		ap->curropt = AP_DONE;

	return ap->curropt;

}



/* static fucntions */

static bool isLongOpt (const ArgvParser * ap)
{
	if (!ap->argvp && (strlen (ap->argv[ap->argcp]) > 2)
			&& (strncmp (ap->argv[ap->argcp], "--", 2) == 0))
		return true;
	else
		return false;
}

static char *findLongOpt (const char **searchin, const char *searchfor)
{
	int     c;
	int     len = strlen (searchfor);
	int     currlen;
	char   *signtest = strchr (searchfor, '=');

	if (signtest)
		len = (int) (signtest - searchfor);



	for (c = 0; searchin[c]; c++)
	{
		currlen = strlen (searchin[c]);
		if (currlen > len)
		{
			/*
			   if currlen is larger tindent: Standard input:317: Error:Stmt nesting error.
			   han len AND if those extra characters
			   are just the : or ? modifiers, compare the strings up to len - 1
			 */
			if (((searchin[c][len] == ':') || (searchin[c][len] == '?'))
					&& (strncmp (searchfor, searchin[c], len) == 0))
				return (char *) searchin[c];

		}
		/*
		   othewise, if currlen equals len, and strcmp says they are the same
		   then return that
		 */
		else if ((currlen == len) && (strncmp (searchin[c], searchfor, len) == 0))
			return (char *) searchin[c];
	}

	/*
	   didn't find anything?
	 */
	return NULL;
}

static char *getLongOptArg (ArgvParser * ap)
{
	char   *temp = strchr (ap->argv[ap->argcp], '=');

	/*
	   this is the only place our argv are edited 
	 */
	if (temp)
	{
		debug ("found =\n");
		*temp = NUL;
		temp++;
	}
	else
	{
		debug ("= not found\n");
		if (((ap->argcp + 1) < ap->argc) && (ap->argv[ap->argcp + 1][0] != '-'))
		{
			temp = ap->argv[ap->argcp + 1];
			ap->argcp++;
		}
	}

	return temp;
}

static bool requireLongOptArg (const char *opt)
{
	if (strchr (opt, ':'))
		return true;
	else
		return false;
}

static bool wantLongOptArg (const char *opt)
{
	if (strstr (opt, "::"))
		return true;
	else
		return false;
}

static char shortLongArgForm (const char *opt)
{
	char   *temp = strchr (opt, '?');

	if (temp)
		return *(temp + 1);
	else
		return NUL;
}

/* Short option functinos */
static bool isShortOpt (ArgvParser * ap)
{
	if ((!ap->argvp && (ap->argv[ap->argcp][0] == '-')) || (ap->argvp))
		return true;
	else
		return false;
}

static bool requireShortOptArg (const char *opt)
{
	if (*(opt + 1) == ':')
		return true;
	else
		return false;
}

static bool wantShortOptArg (const char *opt)
{
	if (*(opt + 1) == ':' && *(opt + 2) == ':')
		return true;
	else
		return false;
}

static char *getShortOptArg (ArgvParser * ap)
{
	char   *temp = &ap->argv[ap->argcp][ap->argvp + 1];

	/*
	   this is the only place our argv are edited 
	 */
	if (!(*temp))
	{
		debug ("= not found\n");
		if (((ap->argcp + 1) < ap->argc) && (ap->argv[ap->argcp + 1][0] != '-'))
		{
			temp = ap->argv[ap->argcp + 1];
			ap->argcp++;
		}
		else
			temp = NULL;
	}
	else
	{
		if ((*temp == '='))
		{
			debug ("found =\n");
			*temp = NUL;
			temp++;
		}
		else
			temp = NUL;
	}

	return temp;
}

static char *findShortOpt (const char *shortopt, char curropt)
{
	char   *curr;

	for (curr = (char *) shortopt; *curr; curr++)
	{
		if (*curr == curropt)
			return curr;
	}

	return NULL;
}

/* 
	 code to handle dangling options 
	 This is situations found in alot of applications where you can
	 specify a file but you don't do it with a - or -- operator
 */
static bool isDanglingOpt (const ArgvParser * ap)
{
	if (!ap->argvp && ap->argv[ap->argcp][ap->argvp] != '-')
		return true;
	else
		return false;
}

static void handleDanglingOpt (ArgvParser * ap)
{
	ap->curropt = AP_FREE;
	ap->optarg = ap->argv[ap->argcp];
	ap->argcp++;
}

/* Code to handle various situations */
static void handleLongOpt (ArgvParser * ap)
{
	char   *findopt;
	debug ("Is a long opt\n");
	ap->fullopt = &ap->argv[ap->argcp][2];
	findopt = findLongOpt ((const char **) ap->longopt, ap->fullopt);
	if (findopt)
	{
		if (!(ap->curropt = shortLongArgForm (findopt)))
			ap->curropt = AP_LONG;

		if (wantLongOptArg (findopt))
			ap->optarg = getLongOptArg (ap);
		else if (requireLongOptArg (findopt))
		{
			if ((ap->optarg = getLongOptArg (ap)) == NULL)
			{
				ap->curropt = AP_ERR;
				CExCreatef (AP_MISSARG_E, "%s requires an argument.", ap->fullopt);
			}
		}
		else
		{
			/*
			   if they try to access it, they know it's not present 
			 */
			ap->optarg = NULL;
		}


	}
	else
	{
		ap->curropt = AP_UNK;
		CExCreatef (AP_UNKARG_E, "%s is an unknown argument.", ap->fullopt);
	}

	ap->argcp++;
	ap->argvp = 0;
}

static void handleShortOpt (ArgvParser * ap)
{
	char   *findopt;
	if (!ap->argvp)
		ap->argvp++;
	ap->curropt = ap->argv[ap->argcp][ap->argvp];
	findopt = findShortOpt (ap->shortopt, ap->curropt);
	if (findopt)
	{
		debug ("findopt = %c\n", ap->curropt);
		if (wantShortOptArg (findopt))
			ap->optarg = getShortOptArg (ap);
		if (requireShortOptArg (findopt))
		{
			if ((ap->optarg = getShortOptArg (ap)) == NULL)
			{
				CExCreatef (AP_MISSARG_E, "%c requires an argument.", ap->curropt);
				ap->curropt = AP_ERR;
			}
		}
		else
			ap->optarg = NULL;

	}
	else
	{
		ap->curropt = AP_UNK;
		CExCreatef (AP_UNKARG_E, "%c is an unknown argument.", ap->curropt);
	}

	ap->argvp++;
	if (((ap->argcp < ap->argc) && !(ap->argv[ap->argcp][ap->argvp])) || ap->curropt)
	{
		ap->argvp = 0;
		ap->argcp++;
	}
}
