#include <stdbool.h>

#ifdef _Bool
#define boolean _Bool
#else
#define boolean char
#endif

typedef struct cexception_t *CException;

#define CExCreate(code, errmsg) \
	CExCreateWA(code, errmsg, __PRETTY_FUNCTION__, __FILE__, __LINE__)
#define CExCreatef(code, errmsg, ...) \
	CExCreatefA(code, __PRETTY_FUNCTION__, __FILE__, __LINE__, errmsg, ## __VA_ARGS__)

CException CExCreateWA (unsigned int code, const char *errmsg,
												const char *infunc, const char *infile,
												unsigned int linenum);
CException CExCreatefA (unsigned int code, const char *infunc,
												const char *infile, unsigned int linenum,
												const char *format, ...);
boolean CExDestroy (CException cex);
CException CExGetException (void);
void    CExPrintInfo (CException cex);
unsigned int CExGetCode (const CException cex);
const char *CExGetErrmsg (const CException cex);
unsigned int CExGetLinenum (const CException cex);
const char *CExGetFunc (const CException cex);
const char *CExGetFile (const CException cex);
