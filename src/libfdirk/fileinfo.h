#ifndef _FILEINFO_H_

#ifdef LINUX_OS
#ifndef MAXNAMLEN
#define MAXNAMLEN 255
#endif
#endif /* LINUX_OS */

#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


/* Defines */
/* These will be used for the 'type' attribute of FILEINFO */
#define T_REG 1
#define T_DIR 2
#define T_SYM 3
#define T_UNK 4

/* Structures */
#ifndef FILEINFO_def
#define FILEINFO_def
struct FILEINFO
{
	char    filename[MAXNAMLEN + 1];
	int     type;

	bool    hidden;

	time_t  atime;
	time_t  mtime;
	time_t  ctime;

	off_t   filesize;
	mode_t  modes;
	uid_t   owner;
	gid_t   group;
	u_int32_t flags;
};

typedef struct FILEINFO Fileinfo;
#endif /* FILEINFO_def */

/* Function Prototypes */
/* Get/Set Filename */
const char *fiGetFilename (const Fileinfo * fi);
void    fiSetFilename (Fileinfo * fi, const char *fn);

/* Get/Set Type values */
int     fiGetType (const Fileinfo * fi);
void    fiSetType (Fileinfo * fi, const int type);
void    fiSetTypeByMode (Fileinfo * fi, const mode_t mode);

/* Get/Set if it is hidden */
bool    fiIsHidden (const Fileinfo * fi);
void    fiSetHidden (Fileinfo * fi, bool hid);

/* Get/Set Time value */
void    fiSetAtime (Fileinfo * fi, const time_t * t);
void    fiSetCtime (Fileinfo * fi, const time_t * t);
void    fiSetMtime (Fileinfo * fi, const time_t * t);
time_t  fiGetAtime (const Fileinfo * fi);
time_t  fiGetCtime (const Fileinfo * fi);
time_t  fiGetMtime (const Fileinfo * fi);

/* Get/Set Filesize */
off_t   fiGetFilesize (const Fileinfo * fi);
void    fiSetFilesize (Fileinfo * fi, const off_t fs);

/* Get/Set Modes */
mode_t  fiGetModes (const Fileinfo * fi);
void    fiSetModes (Fileinfo * fi, const mode_t modes);

/* Get/Set Owner */
uid_t   fiGetOwner (const Fileinfo * fi);
void    fiSetOwner (Fileinfo * fi, const uid_t owner);

/* Get/Set Group */
gid_t   fiGetGroup (const Fileinfo * fi);
void    fiSetGroup (Fileinfo * fi, const gid_t group);

/* Get/Set Flags */
u_int32_t fiGetFlags (const Fileinfo * fi);
void    fiSetFlags (Fileinfo * fi, const u_int32_t flags);
#endif /* _FILEINFO_H_ */
