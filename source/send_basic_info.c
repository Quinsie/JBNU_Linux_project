#include "send_basic_info.h"

void send_basic_info(const int fd)
{
    char str[] = "Hi, this is locker program!\n";
    write(fd, str, sizeof(str));
}