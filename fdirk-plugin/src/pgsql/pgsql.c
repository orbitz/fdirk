#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pgsql.h>
#include <libpq-fe.h>
#include <pgsqlerr.h>
#include <libfdirk.h>

#define VERSION "0.0.1 beta"

static struct pgsql_t pgsqld;

void Usage(void)
{
	printf("pgsql fdirk plugin version %s\n", VERSION);
	printf("[-h host] [-u user] [-p pass] [-P port] -d db -t table"
			"--fmt format\n");
	printf("Usage:\n");
	printf("  -h host      Host to connect to\n");
	printf("  -u user      Database user\n");
	printf("  -p password  Users password\n");
	printf("  -P port      Port database is on (for TCP connections)\n");
	printf("  -d db        Database to use\n");
	printf("  -t table     Table to put data into\n");
	printf("  --col cols   If you only want to update specific columns\n");
	printf("  --fmt format Format to insert the data in, seperated by commas\n");
	printf("Example:\n");
	printf("  -h localhost -u elmo -p tickleme -d Files -t MyFiles "
			"--fmt \"'%%f',%%s,%%o\"\n");
	printf("\nYou can also specify which columns the data should be input:\n");
	printf("  --col \"filename size disk\" --fmt \"'%%f',%%s,%%o\"\n");
	printf("All standard values are assumed for non-required variables\n");
}

bool InitPlugin(ArgvParser * ap)
{
	char   *host = NULL;
	char   *user = getenv("USER");
	char   *pass = NULL;
	char   *db = NULL;
	char   *port = strdup("5432");
	int     done = 0;
	char   *args[] = { "col:", "fmt:" };
	char    ch;

	apSetOpt(ap, "h:u:p:P:d:t:", args);
	memset(&pgsqld, 0, sizeof(struct pgsql_t));

	while (!done && ((ch = apGetNextOpt(ap)) != AP_DONE))
		switch (ch)
		{
		case 'h':
			host = strdup(apGetOptArg(ap));
			break;
		case 'u':
			free(user);
			user = strdup(apGetOptArg(ap));
			break;
		case 'p':
			/*
			 * Add code to handle stdin password 
			 */
			pass = strdup(apGetOptArg(ap));
			break;
		case 'P':
			free(port);
			port = strdup(apGetOptArg(ap));
			break;
		case 'd':
			db = strdup(apGetOptArg(ap));
			break;
		case 't':
			debug("optarg - %s\n", apGetOptArg(ap));
			pgsqld.table = strdup(apGetOptArg(ap));
			break;
		case AP_LONG:
			debug("LONG OPT ARG: %s\n", apGetLongOpt(ap));
			if (strncmp("fmt", apGetLongOpt(ap), 3) == 0)
				pgsqld.format = apGetOptArg(ap);
			else if (strncmp("col", apGetLongOpt(ap), 3) == 0)
				pgsqld.columns = rplcstr(apGetOptArg(ap), " ", ",");
			break;
		case AP_ERR:
		default:
			goto badexit;
			break;
		}


	if (pgsqld.table == NULL || db == NULL || pgsqld.format == NULL)
	{
		debug("Did not specify everything needed\n");
		CExCreate(PGSQL_E_CLI, "Missing required command line elements");
		goto badexit;
	}

	pgsqld.conn = PQsetdbLogin(host, port, NULL, NULL, db, user, pass);
	if (PQstatus(pgsqld.conn) == CONNECTION_BAD)
	{
		debug("Could not login\n");
		CExCreate(PGSQL_E_CONN, "Could not connect to server");
		goto badexit;
	}

	return true;
badexit:
	return false;
}

bool CatalogueFile(const Fileinfo * finfo)
{
	char    query[1024];					/* Who knows how big it will be */
	char   *varvalue, *varvalue1;
	unsigned int c = 0, i = 0;
	PGresult *res;

	memset(query, 0, 1024);
	strcpy(query, "INSERT INTO ");
	strcat(query, pgsqld.table);

	if (pgsqld.columns != NULL)
		snprintf(query, 1024, "%s (%s)", query, pgsqld.columns);

	strcat(query, " VALUES (");
	c = strlen(query);
	for (i = 0; (i < strlen(pgsqld.format)) && (c < 1024); i++)
	{
		if (pgsqld.format[i] == '%')
		{
			varvalue = getVarValue(finfo, pgsqld.format[++i]);
			varvalue = rplcstr(varvalue, "'", "\\'");
			strcat(query, varvalue);
			c = strlen(query);
		}
		else
		{
			query[c] = pgsqld.format[i];
			c++;
		}
	}
	strcat(query, ")");
	printf("query---%s\n", query);
	res = PQexec(pgsqld.conn, query);
	if (!res || PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		CExCreate(PGSQL_E_QUERY, "Querry failed");
		goto badexit;
	}

	PQclear(res);

	return true;
badexit:
	return false;
}

void ClosePlugin(void)
{
	PQfinish(pgsqld.conn);
}
