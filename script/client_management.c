#include "client_management.h"

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
	else if (outmsg[0] == '3') { // management
		readLine(clientFd, inmsg);
		printf("%s", inmsg);
	} else if (outmsg[0] == '4') { // terminate
		readLine(clientFd, inmsg);
		printf("%s", inmsg);
		return 1;
	} else {
		readLine(clientFd, inmsg);
		printf("%s", inmsg);
	}
	
	return 0;
}
