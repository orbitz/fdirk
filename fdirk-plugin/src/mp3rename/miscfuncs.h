#ifndef MISCFUNCS_H__
#define MISCFUNCS_H__

#include <stdbool.h>


bool    strset (char **dst, const char *src);
bool    hasMP3Ext (const char *filename);
bool    strncopy (char *dst, const char *src, unsigned int size);

#endif /* MISCFUNCS_H__ */
