#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MAXLINE 10000
#define MAXSUBLINE 100

struct locker {
	char name[MAXSUBLINE];
	char passwd[MAXSUBLINE];
};
