#ifndef PLUGINDEFS_H__
#define PLUGINDEFS_H__

/* includes */
#include <stdbool.h>
#include <unistd.h>
#include <dirent.h>
#include <fileinfo.h>

/* defines */
#define DIRDELIM (int)'/'				/* This one needs to be fixed for other OS's */
#define LLONG_DIGITS 18

/* Functions */
bool file_exists (const char *filepath);
char   *getBasename (const char *filepath, int delim);
char   *getFilepath (const char *filepath, int delim);
char   *getVarValue (const Fileinfo *finfo, int varname);
const char *getVarName (int varname);
#endif /* PLUGINDEFS_H__ */
