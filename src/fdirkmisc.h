#ifndef FDIRKMISC_H__
#define FDIRKMISC_H__

/* includes */
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>

#ifdef LINUX_OS
#include <time.h>
#endif /* LINUX_OS */

#include <dirent.h>
#include <flags.h>
#include <fdirkerrcode.h>
#include <fileinfo.h>
#include <plugin.h>

/* Defines */
#define NOTADIR -1

/* Macros */
#define FILE_EXISTS(filename) haveAccess(filename)

/* Structures */

/* Functions that will be used elsewhere in the program */
bool    catalogueDir (const char *dirname, const Flags *flags,
											const FPlugin * pgn);
bool    isDir (const char *dirname);
bool    haveAccess (const char *dirname);
const char *appendSlash (char *myString);


#endif /* FDIRKMISC_H__ */
