#include <stdio.h>
#include <boolean.h>
#include <flags.h>
#include <fdirk.h>

#define PLUGINVERSION "0.1 beta"

void *fdirkInitPlugin (int argc, char **argv)
{
  return TRUE;
}

boolean fdirkCatalogueFile (struct FILEINFO * finfo, void *pData)
{
}

void fdirkClosePlugin (void *pData)
{
  printf ("Leaving stdout plugin\n");
}


void fdirkPluginUsage (void)
{
  printf ("Stdout Plugin %s by Orbitz\n", PLUGINVERSION);

}
