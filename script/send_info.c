#include "send_info.h"

void send_basic_info(const int fd)
{
	char str[] = "Welcome to locker server!\nYou can load or take your cargo on this server.\n";
	write(fd, str, strlen(str) + 1);
}

void send_select(const int fd)
{
	char str[] = "Check locker status : 1\nLoad cargo : 2\nCheck or take cargo: 3\nExit : 4\nYour select >> ";
	write(fd, str, strlen(str) + 1);
}
