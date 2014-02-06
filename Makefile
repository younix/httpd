CFLAGS=-std=c99 -pedantic -Wall -Wextra

httpd: httpd.c httpd.h
	gcc ${CFLAGS} -o $@ httpd.c

PRE:
	gcc ${CFLAGS} -E httpd.c

list-dir: list-dir.c list-dir.h
	gcc ${CFLAGS} -o $@ list-dir.c

clean:
	rm -f list-dir httpd
