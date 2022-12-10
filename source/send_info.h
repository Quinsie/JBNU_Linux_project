#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>

void send_basic_info(const int fd);
void send_select(const int fd);

void select_1(const int fd);
void select_2(const int fd);
void select_3(const int fd);
void select_4(const int fd);
