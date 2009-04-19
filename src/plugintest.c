#include <stdio.h>
#include <stdbool.h>
#include <plugindefs.h>
#include <argvparser.h>

static struct plugtest
{
	int     count;
} pgtest;

void Usage (void)
{
	printf ("In Usage\n");
}

bool InitPlugin (ArgvParser *ap)
{
	pgtest.count = 0;
	return true;
}


bool CatalogueFile (const Fileinfo* finfo)
{
	pgtest.count++;
	printf ("%s: %s\n", getVarName ('F'), getVarValue(finfo, 'F'));
	printf ("%s: %s\n", getVarName ('A'), getVarValue(finfo, 'A'));
	printf ("%s: %s\n", getVarName ('M'), getVarValue(finfo, 'M'));
	printf ("%s: %s\n\n", getVarName ('C'), getVarValue(finfo, 'C'));
	return true;
}

void ClosePlugin (void)
{
	printf ("Num of files: %d\n", pgtest.count);
}
