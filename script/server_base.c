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

void base_helper(int lockerlen)
{
	int record;
	
	printf("Server has opened.\n");
	record = open(
	
	printf("Please type size of total locker.");
	scanf("%d", &lockerlen);
	
	
}
