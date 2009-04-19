#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "cexception.h"

struct cexception_t
{
	unsigned int code;
	char   *errmsg;
	unsigned int linenum;
	char   *infunc;
	char   *infile;
};

static CException currex__ = NULL;

/*
Func: CException CExCreateWA (unsigned int major, unsigned int minor,
          const char *errmsg, const char *infunc,
					const char *infile, unsigned int linenum)
Description: Creates an error with all of the arguments given.
   Most other create functions are merely macros for this.
*/
CException CExCreateWA (unsigned int code, const char *errmsg,
												const char *infunc, const char *infile,
												unsigned int linenum)
{
	CException ret = malloc (sizeof (struct cexception_t));

	if (ret != NULL)							/* If malloc was successfull */
	{
		ret->code = code;
		ret->errmsg = strdup (errmsg);
		ret->infunc = strdup (infunc);
		ret->infile = strdup (infile);
		ret->linenum = linenum;
	}

	if (ret->errmsg == NULL || ret->infunc == NULL || ret->infile == NULL)
	{
		free (ret->errmsg);
		free (ret->infunc);
		free (ret->infile);
		free (ret);
		ret = NULL;
	}

	currex__ = ret;
	return ret;
}

/*
Func: CException CExCreatefA (unsigned int code, const char *infunc,
          const char *infile, unsigned int linenum,
					const char *format, ...)
Description: Creates an error using format variables
*/
CException CExCreatefA (unsigned int code, const char *infunc,
												const char *infile, unsigned int linenum,
												const char *format, ...)
{
	va_list args;
	char   *s;
	CException cex;
	va_start (args, format);
	vasprintf (&s, format, args);
	va_end (args);
	cex = CExCreateWA (code, s, infunc, infile, linenum);
	free (s);
	return cex;
}

/*
Func: boolean CExDestroy (CExeption cex)
Description: Returns true if cex object equals the current error
  and free's the object
*/
boolean CExDestroy (CException cex)
{
	if (currex__ == cex)
	{
		free (currex__->errmsg);
		free (currex__->infunc);
		free (currex__->infile);
		free (currex__);
		currex__ = NULL;
		return true;
	}
	else
		return false;
}

/*
Func: CException CExGetException (void)
Description: Returns the current exception of one exists
*/
CException CExGetException (void)
{
	return currex__;
}

/*
Func: void CExPrintInfo (CException cex)
Description: Prints out all the information in a 
  CException object
*/
void CExPrintInfo (CException cex)
{
	if (cex == NULL)
		printf ("No error information to report\n");
	else
	{
		printf ("An exception occured in file %s near line %d in function %s.\n",
						cex->infile, cex->linenum, cex->infunc);
		printf ("Error Value: %d\n", cex->code);
		printf ("Error message: %s\n", cex->errmsg);
	}
}

unsigned int CExGetCode (const CException cex)
{
	return cex->code;
}

const char *CExGetErrmsg (const CException cex)
{
	return cex->errmsg;
}

unsigned int CExGetLinenum (const CException cex)
{
	return cex->linenum;
}

const char *CExGetFunc (const CException cex)
{
	return cex->infunc;
}

const char *CExGetFile (const CException cex)
{
	return cex->infile;
}
