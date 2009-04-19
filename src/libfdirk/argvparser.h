#ifndef ARGOPT_H__
#define ARGOPT_H__

/* Constants */
/* These are for return values */
#define AP_ERR 0
#define AP_DONE -1
#define AP_FREE 2
#define AP_UNK 3
#define AP_LONG 4

/* These are error values */
#define AP_MISSARG_E 1
#define AP_UNKARG_E 2

struct argvparser_t
{
	int     argc;
	char  **argv;
	char   *shortopt;
	char  **longopt;
	int     curropt;
	char   *fullopt;
	char   *optarg;
	int     argcp;
	int     argvp;
};
typedef struct argvparser_t ArgvParser;

void    apSetArg (ArgvParser * ap, int argc, const char **argv);
void    apSetOpt (ArgvParser * ap, const char *shortopt,
									const char **longopt);
void    apReset (ArgvParser * ap);
char    apGetOpt (const ArgvParser * ap);
const char *apGetOptArg (const ArgvParser * ap);
const char *apGetLongOpt (const ArgvParser * ap);
int     apGetNextOpt (ArgvParser * ap);

#endif /* ARGOPT_H__ */
