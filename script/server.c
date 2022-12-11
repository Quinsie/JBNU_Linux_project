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
#define MAXLINE 10000

int main()
{
	int serverFd, clientFd, serverLen, clientLen;
	int lockerlen;
	int* lockerstatus;
	char recieve[MAXLINE];
	struct sockaddr_un serverSOCKaddr, clientSOCKaddr;
	struct sockaddr *serverSOCKaddrPtr, *clientSOCKaddrPtr;

	signal(SIGCHLD, SIG_IGN); // prevent zombie

	serverLen = sizeof(serverSOCKaddr);
	clientLen = sizeof(clientSOCKaddr);
	serverSOCKaddrPtr = (struct sockaddr*)&serverSOCKaddr;
	clientSOCKaddrPtr = (struct sockaddr*)&clientSOCKaddr;
	
	base_helper(&lockerlen, &lockerstatus); // initialize locker
	
	// preparing to open server
	serverFd = socket(AF_UNIX, SOCK_STREAM, DEFAULT_PROTOCOL);
	serverSOCKaddr.sun_family = AF_UNIX;
	strcpy(serverSOCKaddr.sun_path, "../resource/locker"); // server name
	unlink("../resource/locker"); // if file already exists, remove
	bind(serverFd, serverSOCKaddrPtr, serverLen); // make server file
	listen(serverFd, 5); // server queue, normally set to 5.

	while (1) {
		clientFd = accept(serverFd, clientSOCKaddrPtr, &clientLen); // accept client connect
		printf("Server Called!\n");

		if (fork() == 0) { // workspace
			send_basic_info(clientFd); // introduce about this server
			
			while (1) {
				send_select(clientFd); // send selection
				readLine(clientFd, recieve);
				if (menu(clientFd, recieve[0], lockerlen, lockerstatus)) break;
			}

			close(clientFd); // close socket
			exit(0);
		} else close(clientFd); // close socket
		
	}
	
	free(lockerstatus); // free assign

	exit(0);
}
