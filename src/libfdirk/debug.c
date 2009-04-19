#include <stdio.h>
#include <stdarg.h>

void debug (const char *format, ...)
{
#ifdef DEBUG
	va_list args;
	fprintf (stderr, "DEBUG: ");
	va_start (args, format);
	vfprintf (stderr, format, args);
	va_end (args);
#endif
}
