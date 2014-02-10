#ifndef HTTP_H
#define HTTP_H

#include <sys/queue.h>

/* http request string */
#define METHOD 	"(OPTIONS|GET|HEAD|POST|PUT|DELETE|TRACE|CONNECT)"
#define SP 	"[ ]+"
#define URI 	"([^ ]+)"
#define VERSION "(HTTP/[[:digit:]]\\.[[:digit:]])"
#define CRLF 	"([\r\n]*)"

#define REQUEST "^" METHOD SP URI SP VERSION CRLF "$"

struct req {
	char *method;
#define REQ_POS_METHOD 1
	char *uri;
#define REQ_POS_URI 2
	char *version;
#define REQ_POS_VERSION 3
};

/* http header message strings */
#define MESSAGE_NAME "([[:alnum:]]+)"
#define MESSAGE_VALUE "([^[:space:]]*)[ ]*"

#define HEADER_MESSAGE "^" MESSAGE_NAME ":" "[ ]*" MESSAGE_VALUE CRLF "$"

struct mesg {
	char *name;
#define MSG_POS_NAME 1
	char *value;
#define MSG_POS_VALUE 3
	TAILQ_ENTRY(mesg) listp;
};

TAILQ_HEAD(mesg_head, mesg);

#define add_header_field(header, name_, value_) do {			\
		struct mesg *mesg = malloc(sizeof *mesg);		\
		mesg->name = (name_);					\
		mesg->value = (value_);					\
		TAILQ_INSERT_TAIL((header), mesg, listp); } while (0)

#define free_head(head)						\
		while ((m = TAILQ_FIRST(head))) {		\
			TAILQ_REMOVE(head, m, listp);	\
			free(m->name);				\
			free(m->value);				\
			free(m);				\
		}

/* html documents */
#define ERR_DOCUMENT		\
	"<!DOCTYPE html>\n"	\
	"<html>\n"		\
	"<head>\n"		\
	"\t<title>%d - %s</title>\n"\
	"</head>\n"		\
	"<body>\n"		\
	"\t<h1>%d - %s</h2>"	\
	"</body>\n"		\
	"</html>"

#endif
