#ifndef FLAGS_H__
#define FLAGS_H__

/* Include */
#include <stdbool.h>
#include <argvparser.h>

#ifndef MAXNAMLEN
#define MAXNAMLEN 255
#endif

#ifndef MAXFMTLEN
#define MAXFMTLEN 255
#endif

/* Defines */
#define OPT_ERR 0

/* Struct */
struct flags_t
{
	bool help;
	bool verbose;
	bool symlink;
	bool subdirs;
	bool hidden;
	char    plugin[MAXNAMLEN + 1];
	char    *plugindir;
	char    filename[MAXNAMLEN + 1];
};

typedef struct flags_t Flags;

/* Functions */
bool parseCommandLine (Flags *flags, ArgvParser *ap);

#endif /* _FLAGS_H_ */
