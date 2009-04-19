#ifndef PLUGIN_H__
#define PLUGIN_H__

#include <stdbool.h>
#include <fileinfo.h>
#include <argvparser.h>

#ifndef MAXNAMLEN
#define MAXNAMLEN 255
#endif

/* Structure */
struct PLUGIN
{
	void   *handle;
	        bool (*pluginInit) (ArgvParser *);
	void    (*pluginClose) (void);
	        bool (*fileCatalogue) (const Fileinfo *);
	void    (*pluginUsage) (void);
};

typedef struct PLUGIN FPlugin;

/* Plugin prototypes */
int     pluginLoad (FPlugin * pgn, const char *plugin, const char *extrapath);
bool    pluginUnload (FPlugin * pgn);

#endif /* _PLUGIN_H_ */
