FDIRK_PLUGIN_PATH="/usr/local/lib/fdirk/"
#Uncomment the OS you are compiling for
#Linux
#OSDEFINES=-DLINUX_OS
#OSCCLIBS=-ldl

#OpenBSD
OSDEFINES=-DBSD_OS
OSCCLIBS=


#Change PROGNAME for final release
PROGNAME=fdirktest

CC=gcc
INCLUDES=-I./ -Ilibfdirk/
CCFLAGS=-g -Wall -W -pedantic -ansi -Wtraditional -Wundef -Wshadow \
	-Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align \
	-Wwrite-strings -Wconversion -Wsign-compare -Waggregate-return \
	-Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
	-Wnested-externs -Winline #-Wredundant-decls
CCLIBS=$(OSCCLIBS) -Llibfdirk/ -lfdirk
DEFINES=-DDEBUG -DVERSION=\"0.1beta\" $(OSDEFINES) -DFDIRK_PLUGIN_PATH=\"$(FDIRK_PLUGIN_PATH)\"
SOURCE=main.c linklist.c fdirk.c plugin.c fdirkmisc.c  flags.c \
	stack.c 
OBJECTS=$(SOURCE:.c=.o)
SUBDIRS=libfdirk

all: subdirs $(OBJECTS) plugintest.so
	$(CC) $(DEFINES) $(INCLUDES) $(CCFLAGS) -o $(PROGNAME) $(OBJECTS) $(CCLIBS) 

plugintest.so: plugintest.c
	$(CC) -shared -fPIC $(DEFINES) $(INCLUDES) $(CCFLAGS) -o plugintest.so plugintest.c $(CCLIBS)
.c.o:
	$(CC) $(DEFINES) $(INCLUDES) $(CCFLAGS) -c -o $@ $<

$(OBJECTS):$(SOURCE)

test: all
	./$(PROGNAME) -p=plugintest -d=./ -r ./ 

clean:
	rm -f $(OBJECTS) $(PROGNAME) plugintest.so

install: all subdirsinstall
	cp $(PROGNAME) /usr/local/bin/

uninstall:
	rm /usr/local/bin/$(PROGNAME)

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
