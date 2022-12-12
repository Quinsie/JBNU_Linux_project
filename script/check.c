#include "check.h"

void check(const int clientFd, const int lockerlen)
{
	char str[MAXLINE] = "", num[5], chtemp;
	int fd;
	
	fd = open("../resource/status", O_RDONLY);
	lseek(fd, 0, SEEK_SET);
	
	strcat(str, "\n** LOCKER STATUS **\n");
	for (int i = 0; i < lockerlen; i++) {
		strcat(str, "LOCKER ");
		sprintf(num, "%d", i + 1);
		strcat(str, num);
		
		read(fd, &chtemp, sizeof(chtemp));
		if (chtemp == '0') strcat(str, " : VOID\n");
		else strcat(str, " : IN USE\n");
	}
	strcat(str, "\n");
	
	write(clientFd, str, strlen(str) + 1);
	close(fd);
}

void client_check(const int clientFd)
{
	char str[MAXLINE];
	
	readLine(clientFd, str);
	printf("%s", str);
	printf("Return to menu...\n\n");
	sleep(1);
}
