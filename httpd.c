#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "httpd.h"
#include "config.h"

#define get_regex_match(buf, pmatch, pos) (			\
	(pmatch)[(pos)].rm_so == -1 ? NULL :			\
	    strndup((buf) + pmatch[(pos)].rm_so,		\
		pmatch[(pos)].rm_eo - pmatch[(pos)].rm_so))

static void
error_response(int err_code, char *err_str)
{
	printf(ERR_DOCUMENT, err_code, err_str, err_code, err_str);
}

void
method_get(struct req *req, struct mesg_head *mesg_head)
{
	struct mesg *m;
	struct stat path_stat;
	char path[PATH_MAX] = "";
	char buf[BUFSIZ];
	ssize_t len = 0;

	fprintf(stderr, "file: %s\n", req->uri);

	TAILQ_FOREACH(m, mesg_head, listp) {
		fprintf(stderr, "head: %s\n", m->name);
	}

	strlcpy(path, DOCUMENT_ROOT, sizeof path);
	strlcat(path, req->uri, sizeof path);

	stat(path, &path_stat);
	if (S_ISDIR(path_stat.st_mode)) {
		fprintf(stderr, "just a path\n");
	} else if (S_ISREG(path_stat.st_mode)) {
		printf("HTTP/1.1 200 OK\r\n\r\n");
		fflush(stdout);
		int fd = open(path, O_RDONLY);

		while ((len = read(fd, buf, sizeof buf)) > 0)
			write(STDOUT_FILENO, buf, len);

		close(fd);
	} else
		fprintf(stderr, "path: '%s' is nothing!\n", path);
}

static int
read_header(struct mesg_head *mesg_head, FILE *fh)
{
	regmatch_t *pmatch = NULL;
	regex_t reg;
	char buf[BUFSIZ];
	int errcode = 0;
	struct mesg *mesg;

	if ((errcode = regcomp(&reg, HEADER_MESSAGE, REG_EXTENDED) != 0))
		return errcode;

	pmatch = calloc(reg.re_nsub, sizeof *pmatch);

	while (fgets(buf, BUFSIZ, fh) != NULL && strcmp(buf, "\r\n") != 0) {
		if ((errcode = regexec(&reg, buf, reg.re_nsub, pmatch, 0)) != 0) {
			fprintf(stderr, "mismatch: '%s'", buf);
			continue;
		}

		mesg = malloc(sizeof(*mesg));

		mesg->name  = get_regex_match(buf, pmatch, MSG_POS_NAME);
		mesg->value = get_regex_match(buf, pmatch, MSG_POS_VALUE);
		TAILQ_INSERT_TAIL(mesg_head, mesg, listp);
	}

	fprintf(stderr, "end of header!\n");

	return 0;
}

static int
read_request(struct req *req, FILE *fh)
{
	regmatch_t *pmatch = NULL;
	regex_t reg;
	char buf[BUFSIZ];
	int errcode;

	if ((errcode = regcomp(&reg, REQUEST, REG_EXTENDED) != 0))
		return errcode;

	pmatch = calloc(reg.re_nsub, sizeof *pmatch);

	fgets(buf, BUFSIZ, fh);
	if ((errcode = regexec(&reg, buf, reg.re_nsub, pmatch, 0)) != 0)
		return errcode;

	req->method  = get_regex_match(buf, pmatch, REQ_POS_METHOD);
	req->uri     = get_regex_match(buf, pmatch, REQ_POS_URI);
	req->version = get_regex_match(buf, pmatch, REQ_POS_VERSION);

	fprintf(stderr, "req: '%s'", buf);

	return 0;
}

int
//main(int argc, char**argv, char**envp)
main(void)
{
	FILE *fh = NULL;
	int fd = STDIN_FILENO;
	int errcode;
	char errbuf[BUFSIZ];
	struct req req;

	struct mesg_head mesg_head;
	TAILQ_INIT(&mesg_head);

	fh = fdopen(fd, "r");

	if ((errcode = read_request(&req, fh)) != 0)
		goto err;

	if ((errcode = read_header(&mesg_head, fh)) != 0)
		goto err;

	if (strcmp(req.method, "GET") == 0)
		method_get(&req, &mesg_head);
	else
		error_response(501, "Not Implemented");

	return EXIT_SUCCESS;
 err:
	if (errcode != 0) {
		regerror(errcode, NULL, errbuf, BUFSIZ);
		fprintf(stderr, "%s\n", errbuf);
	}

	if (errno != 0)
		perror(NULL);

	return EXIT_FAILURE;
}
