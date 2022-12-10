#include "send_info.h"

void send_basic_info(const int fd)
{
	char str[] = "Welcome to locker server!\nYou can load or take your cargo on this server.\n";
	write(fd, str, sizeof(str));
}

void send_select(const int fd)
{
	char str[] = "Please select:\nCheck locker status : 1\nLoad cargo : 2\nTake cargo: 3\nExit : Press any key\nYour select : ";
	write(fd, str, sizeof(str));
}

void select_1(const int fd)
{
	char str[] = "Your selection was 1.\n";
	write(fd, str, sizeof(str));
}
void select_2(const int fd)
{
	char str[] = "Your selection was 2.\n";
	write(fd, str, sizeof(str));
}
void select_3(const int fd)
{
	char str[] = "Your selection was 3.\n";
	write(fd, str, sizeof(str));
}
void select_4(const int fd)
{
	char str[] = "Please type again.\n";
	write(fd, str, sizeof(str));
}
