#include <sys/types.h>

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list-dir.h"

void
usage(void)
{
	fprintf(stderr, "list-dir [PATH]\n");
}

int
main(int argc, char**argv)
{
	DIR *dir = NULL;
	struct dirent *entry;
	char *path = ".";

	if (argc > 1)
		path = strdup(argv[1]);

	if ((dir = opendir(path)) == NULL)
		goto err;

	puts(DOCUMENT_HEADER);

	while ((entry = readdir(dir)) != NULL) {
		printf("\t<li><a href=\"%s\">%s</a></li>\n", entry->d_name,
		    entry->d_name);
	}

	puts(DOCUMENT_FOOTER);

	if (closedir(dir) != 0)
		goto err;

	return EXIT_SUCCESS;
 err:
	if (errno != 0)
		perror(argv[0]);

	return EXIT_FAILURE;
}
