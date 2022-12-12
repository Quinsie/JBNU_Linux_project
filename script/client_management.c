#include "client_management.h"

void client_terminate(int clientFd) // function terminates client
{
	char inmsg[MAXLINE];
	readLine(clientFd, inmsg);
	printf("%s", inmsg);
}

void client_wrong(int clientFd) // function express that client pressed wrong button
{
	char inmsg[MAXLINE];
	readLine(clientFd, inmsg);
	printf("%s", inmsg);
}

int menu(int clientFd) // function gives client some options
{
	char inmsg[MAXLINE], outmsg[MAXLINE];
	
	readLine(clientFd, inmsg); // get menu info from server
	printf("%s", inmsg);
	
	fgets(outmsg, MAXLINE, stdin);
	outmsg[strlen(outmsg)] = 0;
	write(clientFd, outmsg, strlen(outmsg) + 1); // send client's order
	
	if (outmsg[0] == '1') client_check(clientFd); // check
	else if (outmsg[0] == '2') client_store(clientFd); // store
	else if (outmsg[0] == '3') client_manage(clientFd); // manage
	else if (outmsg[0] == '4') { client_terminate(clientFd); return 1; } // terminate
	else client_wrong(clientFd); // wrong input
	
	return 0;
}
