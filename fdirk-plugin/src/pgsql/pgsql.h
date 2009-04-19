#include <libpq-fe.h>

struct pgsql_t
{
	PGconn *conn;
	char *columns;
	char *format;
	char *table;
};

