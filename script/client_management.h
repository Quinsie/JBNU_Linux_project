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

void client_terminate(int clientFd);
void client_wrong(int clientFd);
int menu(int clientFd);
