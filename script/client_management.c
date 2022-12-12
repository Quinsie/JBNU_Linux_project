#include "client_management.h"

void client_terminate(int clientFd)
{
	char inmsg[MAXLINE];
	readLine(clientFd, inmsg);
	printf("%s", inmsg);
}

void client_wrong(int clientFd)
{
	char inmsg[MAXLINE];
	readLine(clientFd, inmsg);
	printf("%s", inmsg);
}

int menu(int clientFd)
{
	char inmsg[MAXLINE], outmsg[MAXLINE];
	
	readLine(clientFd, inmsg);
	printf("%s", inmsg);
	
	fgets(outmsg, MAXLINE, stdin);
	outmsg[strlen(outmsg)] = 0;
	write(clientFd, outmsg, strlen(outmsg) + 1);
	
	if (outmsg[0] == '1') client_check(clientFd); // check
	else if (outmsg[0] == '2') client_store(clientFd); // store
	else if (outmsg[0] == '3') client_manage(clientFd); // manage
	else if (outmsg[0] == '4') { client_terminate(clientFd); return 1; }
	else client_wrong(clientFd);
	
	return 0;
}
