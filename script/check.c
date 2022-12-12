#include "check.h"

void check(const int clientFd, const int lockerlen)
{
	char str[MAXLINE] = "", num[5], chtemp;
	int fd;
	
	fd = open("../resource/status", O_RDONLY); // lockerstatus filedescriptor
	lseek(fd, 0, SEEK_SET);
	
	strcat(str, "\n\n** LOCKER STATUS **\n");
	for (int i = 0; i < lockerlen; i++) { // repeat till lockerlen
		strcat(str, "LOCKER ");
		sprintf(num, "%d", i + 1);
		strcat(str, num);
		
		read(fd, &chtemp, sizeof(chtemp)); // read lockerstatus file
		if (chtemp == '0') strcat(str, " : VOID\n"); // if now index is 0 -> VOID, else -> IN USE
		else strcat(str, " : IN USE\n");
	}
	strcat(str, "\n");
	
	write(clientFd, str, strlen(str) + 1); // send lockerstatus info to client
	close(fd);
}

void client_check(const int clientFd)
{
	char str[MAXLINE];
	
	readLine(clientFd, str); // recieve lockerstatus info
	printf("%s", str);
	printf("Return to menu...\n\n");
	sleep(1);
}
