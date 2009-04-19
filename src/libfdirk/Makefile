LIBNAME=libfdirk.so
CC=gcc
INCLUDES=-I./ -I../
LINKFLAGS=-shared -fPIC
CCFLAGS=-g -Wall -W -pedantic -ansi 
CCLIBS=
DEFINES=-DBSD_OS -DDEBUG
SOURCES=fileinfo.c pluginfuncs.c argvparser.c cexception.c debug.c buffcontainer.c rplcstr.c
OBJECTS=$(SOURCES:.c=.o)


all: $(OBJECTS)
	$(CC) $(DEFINES) $(LINKFLAGS) $(CCFLAGS) $(INCLUDES) -o $(LIBNAME) $(OBJECTS) $(CCLIBS)

%.o: %.c
	$(CC) $(DEFINES) $(CCFLAGS) $(INCLUDES) -c -o $@ $< $(CCLIBS)


clean:
	rm -f $(OBJECTS) $(LIBNAME)

install: all
	cp $(LIBNAME) /usr/local/lib/

uninstall: 
	rm /usr/local/lib/$(LIBNAME)
