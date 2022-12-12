#include "management.h"

void server_on(int* lockerlen)
{
	int num, lockerstatus, recordFd;
	
	fprintf(stderr, "Prepare to open locker server.\n");
	fprintf(stderr, "Please type total size of locker : ");
	scanf("%d", &num);
	*lockerlen = num;
	
	lockerstatus = open("../resource/status", O_RDWR|O_CREAT, 0640); // make locker status file
	lseek(lockerstatus, 0, SEEK_SET);
	for (int i = 0; i < num; i++) {
		char chtemp = '0';
		write(lockerstatus, &chtemp, sizeof(char));
	}
	fprintf(stderr, "Locker has been initialized with size of %d.\n", num);
	
	recordFd = open("../resource/record", O_RDWR|O_CREAT, 0640); // make record file
	
	close(lockerstatus);
	close(recordFd);
}

void basic_info(const int fd) // introduce server to client
{
	char str[MAXLINE] = "";
	
	for (int i = 0; i < 50; i++) strcat(str, "=");
	strcat(str, "\n");
	strcat(str, "Welcome to locker server!\n");
	strcat(str, "You can store or take your cargo on this server.\n");
	for (int i = 0; i < 50; i++) strcat(str, "=");
	strcat(str, "\n");
	write(fd, str, strlen(str) + 1);
}

int selection(const int clientFd, const int lockerlen) // view menu
{
	char msg[MAXSUBLINE], str[MAXLINE] = "";
	
	strcat(str, "\n** MENU **\nCheck locker status : 1\nStore your cargo : 2\nManage your cargo : 3\nExit : 4\nYour select >> ");
	write(clientFd, str, strlen(str) + 1);
	
	readLine(clientFd, msg);
	
	if (msg[0] == '1') check(clientFd, lockerlen); // check
	else if (msg[0] == '2') store(clientFd, lockerlen); // store
	else if (msg[0] == '3') { // manage
		char tmp[] = "\nYour selection was 3.\n";
		write(clientFd, tmp, strlen(tmp) + 1);
	} else if (msg[0] == '4') { // terminate
		char tmp[] = "\nClient terminates.\n";
		write(clientFd, tmp, strlen(tmp) + 1);
		return 1;
	} else {
		char tmp[] = "\nWrong selection.\n";
		write(clientFd, tmp, strlen(tmp) + 1);
	}
	
	return 0;
}


