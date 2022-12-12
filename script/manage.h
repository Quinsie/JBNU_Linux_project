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
#include "locker.h"

#define MAXLINE 10000
#define MAXSUBLINE 100

void manage(const int clientFd, const int lockerlen);
void client_manage(const int clientFd);
