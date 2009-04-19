LIBNAME=mp3rename.so
CC=gcc
INCLUDES=-I./ -I../../../src/libfdirk
LINKFLAGS=-shared -fPIC
CCFLAGS=-g -Wall -W -pedantic -ansi 
CCLIBS=-L../../../src/libfdirk -lfdirk
DEFINES=-DDEBUG -DLINUX_OS
SOURCES=mp3rename.c id3tag.c mp3flags.c miscfuncs.c
OBJECTS=$(SOURCES:.c=.o)


all: $(OBJECTS)
	$(CC) $(DEFINES) $(LINKFLAGS) $(CCFLAGS) $(INCLUDES) -o $(LIBNAME) $(OBJECTS) $(CCLIBS)

%.o: %.c
	$(CC) $(DEFINES) $(CCFLAGS) $(INCLUDES) -c -o $@ $< 


clean:
	rm -f $(OBJECTS) $(LIBNAME)

install: all
	mkdir /usr/local/lib/fdirk > /dev/null 2>&1
	cp $(LIBNAME) /usr/local/lib/fdirk

uninstall:
	rm /usr/local/lib/fdirk/$(LIBNAME)
