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

#define DEFAULT_PROTOCOL 0
#define MAXLINE 100

int main()
{
	int clientFd, serverLen, result;
	char inmsg[MAXLINE], outmsg[MAXLINE];
	struct sockaddr_un serverSOCKaddr;
	struct sockaddr* serverSOCKaddrPtr;

	serverLen = sizeof(serverSOCKaddr);
	serverSOCKaddrPtr = (struct sockaddr*)&serverSOCKaddr;

	clientFd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
	serverSOCKaddr.sun_family = AF_UNIX; // server domain
	strcpy(serverSOCKaddr.sun_path, "locker"); // server name

	do { // loop until get connection to server
		result = connect(clientFd, serverSOCKaddrPtr, serverLen);
		if (result == -1) sleep(1); // wait and retry
	} while (result == -1);

	readLine(clientFd, inmsg); // recieve basic info about server
	printf("%s", inmsg);
	
	while (1) {
		readLine(clientFd, inmsg); // recieve selection info
		printf("\n%s", inmsg);
		fgets(outmsg, MAXLINE, stdin); // input selection
		
		if (outmsg[0] == '4') {
			printf("\nClient Terminates.\n");
			break;
		}
		
		write(clientFd, outmsg, strlen(outmsg) + 1); // send
		
		readLine(clientFd, inmsg);
		printf("\n%s", inmsg);
	}

	close(clientFd);

	return 0;
}
