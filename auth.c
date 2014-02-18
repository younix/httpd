#include <stdio.h>
#include <stdbool.h>

#ifdef BSD_AUTH
#include <bsd_auth.h>
#endif

bool
user_auth(char *user, char *pass)
{
#ifdef BSD_AUTH
	if (auth_userokay(user, NULL, NULL, pass) != 0)
		return true;
#endif

	return false;
}
