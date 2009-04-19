
CC=cc
INCLUDES=-I./ -I../ -I/usr/local/pgsql/include
CCFLAGS=-shared -g -fPIC  -Wall -W -pedantic -ansi -L/usr/local/pgsql/lib
CCLIBS=-lpq
DEFINES=-DDEBUG
SOURCES=pgsql.c
OBJECTS=$(SOURCES:.c=.so)


all: $(OBJECTS)

%.so: %.c
	$(CC) $(DEFINES) $(CCFLAGS) $(INCLUDES) -o $@ $< $(CCLIBS)


clean:
	rm -f $(OBJECTS)
