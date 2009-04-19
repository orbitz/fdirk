#ifndef MP3FLAGS_H__
#define MP3FLAGS_H__

#include <stdbool.h>

struct mp3flags_t
{
	bool    assume;
};

typedef struct mp3flags_t MP3Flags;

bool    mp3fCreate (MP3Flags * mf);
void    mp3fDestroy (MP3Flags * mf);

bool    mp3fSetAssume (MP3Flags * mf, bool assume);

bool    mp3fGetAssume (const MP3Flags * mf);

#endif /* MP3FLAGS_H__ */
