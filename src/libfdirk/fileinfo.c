#define _GNU_SOURCE
#include <string.h>
#include <fileinfo.h>
#include <plugindefs.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifdef DEBUG
#include <debug.h>
#include <stdio.h>
#endif /* DEBUG */

/*
Func: fiGetFileinfo(Fileinfo *fileInfo, const char * filename)
Description: Gets file info from filename and puts it in struct
*/
bool fiGetFileinfo (Fileinfo * fileInfo, const char *filename)
{
	struct stat filestat;
	int     retval;
	char   *basename;

	fiSetFilename (fileInfo, filename);
	retval = lstat (filename, &filestat);
	if (retval == 0)
	{
		/*
		 * This sets fileInfo->st_mode to filestat.mode and sets 
		 * fileInfo->type to whatever it figures out.  This way we have
		 * quick reference as to what it is and then we can also see more
		 * indepth modes
		 */
		fileInfo->modes = filestat.st_mode;
		fiSetTypeByMode (fileInfo, filestat.st_mode);

#if defined(BSD_OS) || defined(LINUX_OS)
		basename = getBasename (filename, DIRDELIM);
		debug ("basename: %s\n", basename);
		if (basename[0] == '.')
			fileInfo->hidden = true;
		else
			fileInfo->hidden = false;
#endif /* BSD_OS || LINUX_OS */
#ifdef BSD_OS
		memcpy (&fileInfo->atime, &filestat.st_atimespec.tv_sec, sizeof (time_t));
		memcpy (&fileInfo->ctime, &filestat.st_ctimespec.tv_sec, sizeof (time_t));
		memcpy (&fileInfo->mtime, &filestat.st_mtimespec.tv_sec, sizeof (time_t));
		fileInfo->flags = filestat.st_flags;
#endif /* BSD_OS */
#ifdef LINUX_OS
		memcpy (&fileInfo->atime, &filestat.st_atime, sizeof (time_t));
		memcpy (&fileInfo->ctime, &filestat.st_ctime, sizeof (time_t));
		memcpy (&fileInfo->mtime, &filestat.st_mtime, sizeof (time_t));
#endif /* LINUX_OS */

		fileInfo->filesize = filestat.st_size;
		fileInfo->owner = filestat.st_uid;
		fileInfo->group = filestat.st_gid;

		retval = true;
	}
	else
	{
		retval = false;
	}

	return retval;

}

/*
Func: fiGetFilename (const Fileinfo *fi)
Desc: Get filename, returns pointer to the filename
*/
const char *fiGetFilename (const Fileinfo * fi)
{
	return fi->filename;
}

/*
Func: fiSetFilename (Fileinfo *fi, const char *fn)
Desc: Copies fn into filename
*/
void fiSetFilename (Fileinfo * fi, const char *fn)
{
	memcpy (fi->filename, fn, MAXNAMLEN + 1);
}

/* 
Function: fiGetType (const Fileinfo *fi)
Desc: Get the type variable
*/
int fiGetType (const Fileinfo * fi)
{
	return fi->type;
}

/*
Function: fiSetType (Fileinfo *fi, const int type)
Desc: Set the Type variable to an integer value
*/
void fiSetType (Fileinfo * fi, const int type)
{
	fi->type = type;
}

/*
Function: fiSetTypeByMode (Fileinfo *fi, const mode_t mode)
Desc: Set the type variable by a mode_t value, this has OS specific code
*/
void fiSetTypeByMode (Fileinfo * fi, const mode_t mode)
{
#ifdef LINUX_OS
	if (S_ISDIR (mode))
		fiSetType (fi, T_DIR);
	else if (S_ISREG (mode))
		fiSetType (fi, T_REG);
	else if (S_ISLNK (mode))
		fiSetType (fi, T_SYM);
	else
		fiSetType (fi, T_UNK);
#endif /* LINUX_OS */

#ifdef BSD_OS
	if ((mode & S_IFLNK) == S_IFLNK)
		fiSetType (fi, T_SYM);
	else if ((mode & S_IFDIR) == S_IFDIR)
		fiSetType (fi, T_DIR);
	else
		fiSetType (fi, T_REG);
#endif /* BSD_OS */
}

/*
Func: fiSetHidden (Fileinfo *fi, bool hid)
Description: Sets if file is hidden
*/
void fiSetHidden (Fileinfo * fi, bool hid)
{
	fi->hidden = hid;
}

/*
Func: fiIsHidden (Fileinfo *fi)
Description: Returns true/false if file is hidden
*/
bool fiIsHidden (const Fileinfo * fi)
{
	return fi->hidden;
}


/*
Function: fiGetAtime (const Fileinfo *fi)
Desc: Gets the atime
*/
time_t fiGetAtime (const Fileinfo * fi)
{
	return fi->atime;
}

/*
Function: fiGetCtime (const Fileinfo *fi)
Desc: Gets the ctime
*/
time_t fiGetCtime (const Fileinfo * fi)
{
	return fi->ctime;
}

/*
Function: fiGetMtime (const Fileinfo *fi)
Desc: Gets the mtime
*/
time_t fiGetMtime (const Fileinfo * fi)
{
	return fi->mtime;
}

/*
Func: fiSetAtime (Fileinfo *fi, time_t * t)
Desc: Sets atime
*/
void fiSetAtime (Fileinfo * fi, const time_t * t)
{
	memcpy (&fi->atime, t, sizeof (time_t));
}

/*
Func: fiSetCtime (Fileinfo *fi, time_t * t)
Desc: Sets ctime
*/
void fiSetCtime (Fileinfo * fi, const time_t * t)
{
	memcpy (&fi->ctime, t, sizeof (time_t));
}

/*
Func: fiSetMtime (Fileinfo *fi, time_t * t)
Desc: Sets mtime
*/
void fiSetMtime (Fileinfo * fi, const time_t * t)
{
	memcpy (&fi->mtime, t, sizeof (time_t));
}

/*
Func: fiGetFilesize (const Fileinfo *fi)
Desc: Gets filesize
*/
off_t fiGetFilesize (const Fileinfo * fi)
{
	return fi->filesize;
}

/*
Func: fiSetFilesize (Fileinfo *fi, off_t fs)
Desc: Sets filesize
*/
void fiSetFilesize (Fileinfo * fi, const off_t fs)
{
	fi->filesize = fs;
}

/*
Func: fiGetModes (const Fileinfo *fi)
Desc: Gets mode
*/
mode_t fiGetModes (const Fileinfo * fi)
{
	return fi->modes;
}

/*
Func: fiSetModes (Fileinfo *fi, const mode_t modes)
Desc: Sets modes
*/
void fiSetModes (Fileinfo * fi, const mode_t modes)
{
	fi->modes = modes;
}

/*
Func: fiGetOwner (const Fileinfo *fi)
Desc: Get owner
*/
uid_t fiGetOwner (const Fileinfo * fi)
{
	return fi->owner;
}

/*
Func: fiSetOwner (Fileinfo *fi, const uid_t owner)
Desc: Set owner
*/
void fiSetOwner (Fileinfo * fi, const uid_t owner)
{
	fi->owner = owner;
}

/*
Func: fiGetGroup (const Fileinfo *fi)
Desc: Get group
*/
gid_t fiGetGroup (const Fileinfo * fi)
{
	return fi->group;
}

/*
Func: fiSetGroup (Fileinfo *fi, const gid_t group)
Desc: Set group
*/
void fiSetGroup (Fileinfo * fi, const gid_t group)
{
	fi->group = group;
}

/*
Func: fiGetFlags (const Fileinfo *fi)
Desc: Get flags
*/
u_int32_t fiGetFlags (const Fileinfo * fi)
{
	return fi->flags;
}

/*
Func: fiSetFlags (Fileinfo *fi, const u_int32_t flags)
Desc: Set flags
*/
void fiSetFlags (Fileinfo * fi, const u_int32_t flags)
{
	fi->flags = flags;
}
