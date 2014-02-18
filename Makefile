CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g ${GNU} ${BSD}

all: httpd list-dir

httpd: httpd.c httpd.h config.h base64.c base64.h auth.c auth.h
	${CC} ${CFLAGS} -o $@ httpd.c base64.c auth.c

PRE:
	${CC} ${CFLAGS} -E httpd.c

list-dir: list-dir.c list-dir.h
	${CC} ${CFLAGS} -o $@ list-dir.c

clean:
	rm -f list-dir httpd httpd.core

debug:
	gdb httpd httpd.core
