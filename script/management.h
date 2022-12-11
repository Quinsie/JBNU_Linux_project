#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void base_helper(int* lockerlen, int** lockerstatus);
int menu(const int fd, const char order, const int lockerlen, int* lockerstatus);
