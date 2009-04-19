LIBNAME=postgres.so
CC=gcc
INCLUDES=-I./ -I../../../src/libfdirk -I/usr/local/pgsql/include
LINKFLAGS=-shared -fPIC
CCFLAGS=-g -Wall -W -pedantic -ansi 
CCLIBS=-L../../../src/libfdirk -L/usr/local/pgsql/lib -lfdirk -lpq
DEFINES=-DDEBUG -DBSD_OS
SOURCES=pgsql.c
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
