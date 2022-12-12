#include "manage.h"

void manage(const int clientFd, const int lockerlen)
{
	char str[MAXLINE] = "", num[5], chtemp, recvmsg[MAXSUBLINE];
	int inum, fd, recordFd, flag, repeat, flag2;
	int* lockerstatus;
	struct locker record;
	
	fd = open("../resource/status", O_RDWR);
	lseek(fd, 0, SEEK_SET);
	lockerstatus = (int*)calloc(lockerlen, sizeof(int));
	
	for (int i = 0; i < lockerlen; i++) {
		read(fd, &chtemp, sizeof(chtemp));
		if (chtemp == '1') lockerstatus[i] = 1;
	}
	
	strcat(str, "\n** MANAGE CARGO **\n");
	strcat(str, "You can manage stored cargo on this server.\n\n");
	write(clientFd, str, strlen(str) + 1);
	memset(str, 0, sizeof(str));
	
	strcat(str, "Press your locker's number. If press 0, you will return to menu.\n");
	strcat(str, "Your input >> ");
	
	while (1) {
		write(clientFd, str, strlen(str) + 1); // send selection
		readLine(clientFd, recvmsg);
		
		inum = atoi(recvmsg);
		if (inum > lockerlen || inum < 0) { // out of order
			char tmp[] = "2";
			write(clientFd, tmp, strlen(tmp) + 1);
			continue;
		} else if (inum == 0) { // go to menu
			free(lockerstatus);
			close(fd);
			return;
		} else if (lockerstatus[inum - 1]) { // in use
			char tmp[] = "1";
			write(clientFd, tmp, strlen(tmp) + 1);
			break;
		} else { // void
			char tmp[] = "0";
			write(clientFd, tmp, strlen(tmp) + 1);
			continue;
		}
	}
	
	// get locker data from record
	recordFd = open("../resource/record", O_RDWR);
	lseek(recordFd, (long)(inum - 1) * sizeof(record), SEEK_SET);
	read(recordFd, &record, sizeof(record));
	
	// password
	repeat = 3; flag2 = 1;
	while (repeat--) {
		memset(str, 0, sizeof(str));
		strcat(str, "\nPlease type this locker's password.\nYour input >> ");
		write(clientFd, str, strlen(str) + 1);
		
		readLine(clientFd, recvmsg);
		for (int i = 0; i < strlen(recvmsg); i++) { // removing '\n'
			if (recvmsg[i] == '\n') {
				recvmsg[i] = 0;
				break;
			}
		}
		
		flag = 1; // compare
		for (int i = 0; i < 100; i++) {
			if (record.passwd[i] != recvmsg[i]) { flag = 0; break; }
			if (record.passwd[i] == 0 && recvmsg[i] == 0) break;
		}
		
		if (flag) { // correct
			char tmp[] = "1";
			write(clientFd, tmp, strlen(tmp) + 1);
			break;
		} else {
			if (!repeat) flag2 = 0;
			char tmp[] = "0";
			write(clientFd, tmp, strlen(tmp) + 1);
		}
	}
	
	memset(str, 0, sizeof(str));
	if (!flag2) { // wrong 3 times
		strcat(str, "\nYou have typed wrong answer 3 times. Access denied.\n");
		write(clientFd, str, strlen(str) + 1);
		free(lockerstatus);
		close(fd);
		close(recordFd);
		return;
	} 
	
	// give locker info and get order
	sprintf(num, "%d", inum);
	strcat(str, "\n** Locker ");
	strcat(str, num);
	strcat(str, " info **\nCargo name = ");
	strcat(str, record.name);
	strcat(str, "\nLocker password = ");
	strcat(str, record.passwd);
	strcat(str, "\n\nTake cargo : 1\nModify cargo : 2\nChange password : 3\nExit : 4\nYour input >> ");
	write(clientFd, str, strlen(str) + 1);
	
	readLine(clientFd, recvmsg);
	if (recvmsg[0] == '1') { // take
	} else if (recvmsg[0] == '2') { // modify
	} else if (recvmsg[0] == '3') { // change passwd
	} else if (recvmsg[0] == '4') { // exit
	} else { // error
	}
}

void client_manage(const int clientFd)
{
	char str[MAXLINE], sendmsg[MAXSUBLINE];
	int repeat, flag;
	
	readLine(clientFd, str);
	printf("%s", str); // print basic info about manage

	while (1) {
		readLine(clientFd, str);
		printf("%s", str);
		
		fgets(sendmsg, MAXSUBLINE, stdin); // client answer (number);
		write(clientFd, sendmsg, strlen(sendmsg) + 1);
		
		if (sendmsg[0] == '0') { // return to menu
			printf("Return to menu...\n\n");
			sleep(1);
			return;
		}
		
		memset(sendmsg, 0, sizeof(sendmsg));
		
		readLine(clientFd, str);
		if (str[0] == '0') {
			printf("This locker has nothing! Please retry.\n\n");
			sleep(1);
		} else if (str[0] == '1') {
			break;
		} else {
			printf("This number is unavailable. Please retry.\n\n");
			sleep(1);
		}
	}
	
	// password
	repeat = 3; flag = 1;
	while (repeat--) {
		readLine(clientFd, str);
		printf("%s", str);
		memset(sendmsg, 0, sizeof(sendmsg));
		fgets(sendmsg, MAXSUBLINE, stdin);
		write(clientFd, sendmsg, strlen(sendmsg) + 1);
		
		readLine(clientFd, str);
		if (str[0] == '1') {
			break;
		}
		if (str[0] == '0' && !repeat) flag = 0;
	}
	
	if (!flag) {
		readLine(clientFd, str);
		printf("%s", str);
		printf("Return to menu...\n\n");
		sleep(1);
		return;
	}
	
	// get locker info and give order
	readLine(clientFd, str);
	printf("%s", str);
	memset(sendmsg, 0, sizeof(sendmsg));
	fgets(sendmsg, MAXSUBLINE, stdin);
	write(clientFd, sendmsg, strlen(sendmsg) + 1);
	
	
	printf("Return to menu...\n\n");
	sleep(1);
}
