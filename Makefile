CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g ${GNU} ${BSD}

all: httpd list-dir

httpd: httpd.o base64.o auth.o
	${CC} -o $@ httpd.o base64.o auth.o

list-dir: list-dir.o
	${CC} -o $@ list-dir.o

clean:
	rm -f list-dir httpd *.core *.o

debug:
	gdb httpd httpd.core

.SUFFIXES: .c .o
.c.o:
	${CC} ${CFLAGS} -c $<

httpd.o: httpd.h config.h
auth.o: auth.h
base64.o: base64.h
