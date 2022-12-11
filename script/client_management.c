#include "client_management.h"

int client_menu(const int fd)
{
	char inmsg[MAXLINE], outmsg[MAXLINE];
	
	readLine(fd, inmsg); // recieve selection info
	printf("\n%s", inmsg);
	fgets(outmsg, MAXSUBLINE, stdin); // input selection
	write(fd, outmsg, strlen(outmsg) + 1);
	
	if (outmsg[0] == '1') { // check
		readLine(fd, inmsg);
		printf("\n%s", inmsg);
	} else if (outmsg[0] == '2') { // load
		readLine(fd, inmsg); // recieve basic load info (management line 69)
		printf("\n%s", inmsg); 
		while (1) {
			readLine(fd, inmsg); // recieve order info (management line 74)
			printf("\n%s", inmsg);
			fgets(outmsg, MAXSUBLINE, stdin);
			write(fd, outmsg, strlen(outmsg) + 1); // send locker num (man line 75)
			
			readLine(fd, inmsg); // recieve ans (man line 80 83 86)
			if (inmsg[0] == '1') { // in use
				printf("This number is unavailable. Please retry.\n");
				sleep(1);
				continue;
			} else break; 
		}
		
		readLine(fd, inmsg); // cargo's name (man line 95)
		printf("%s", inmsg);
		fgets(outmsg, MAXSUBLINE, stdin); 
		write(fd, outmsg, strlen(outmsg) + 1); // send cargo's name (man line 97)
		/*
		readLine(fd, inmsg); // recieve password (man line 102)
		printf("%s", inmsg);
		fgets(outmsg, MAXSUBLINE, stdin); // input password
		write(fd, outmsg, strlen(outmsg) + 1); // send password (man line 104)
		
		readLine(fd, inmsg); // recieve again password (man line 109)
		printf("%s", inmsg);
		fgets(outmsg, MAXSUBLINE, stdin); // input again password
		write(fd, outmsg, strlen(outmsg) + 1); // send again password (man line 111)
		
		readLine(fd, inmsg); // recieve ans (man line 114, 117)
		if (inmsg[0] == '1') printf("Your password is wrong! Please retry later.\n"); // correct
		else printf("Your cargo has been successfully loaded!\n"); // wrong
		*/
		
	} else if (outmsg[0] == '3') { // take
	} else if (outmsg[0] == '4') { // terminate
		printf("Client terminates.\n");
		return 1;
	} else {
		readLine(fd, inmsg);
		printf("\n%s", inmsg);
	}
	
	return 0;
}
