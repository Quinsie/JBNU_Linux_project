#include "manage.h"

void manage(const int clientFd, const int lockerlen)
{
	char str[MAXLINE] = "", num[5], chtemp, recvmsg[MAXSUBLINE];
	int inum, fd, recordFd, flag, repeat, flag2;
	int* lockerstatus;
	struct locker record;
	
	fd = open("../resource/status", O_RDWR); // lockerstatus file descriptor (option = read ans write)
	lseek(fd, 0, SEEK_SET);
	lockerstatus = (int*)calloc(lockerlen, sizeof(int)); // declaration and initialize
	
	for (int i = 0; i < lockerlen; i++) {
		read(fd, &chtemp, sizeof(chtemp)); // read info from lockerstatus file
		if (chtemp == '1') lockerstatus[i] = 1; // if '1' in lockerstatus file, set now index is on. (flag table)
	}
	
	strcat(str, "\n\n** MANAGE CARGO **\n");
	strcat(str, "You can manage stored cargo on this server.\n\n");
	write(clientFd, str, strlen(str) + 1); // send basic information about this option
	memset(str, 0, sizeof(str));
	
	strcat(str, "Press your locker's number. If press 0, you will return to menu.\n");
	strcat(str, "Your input >> "); // get client's answer
	
	while (1) {
		write(clientFd, str, strlen(str) + 1); // send selection
		readLine(clientFd, recvmsg); // get client's answer
		
		inum = atoi(recvmsg); // inum = target index
		if (inum > lockerlen || inum < 0) { // out of order
			char tmp[] = "2"; // send error code
			write(clientFd, tmp, strlen(tmp) + 1);
			continue;
		} else if (inum == 0) { // go to menu
			free(lockerstatus); // deallocation (needed)
			close(fd);
			return;
		} else if (lockerstatus[inum - 1]) { // in use
			char tmp[] = "1";
			write(clientFd, tmp, strlen(tmp) + 1);
			break;
		} else { // void
			char tmp[] = "0"; // send serror code
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
		} else { // incorrect
			if (!repeat) flag2 = 0; // if it was last chance, turn off flag2
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
	strcat(str, "\n\n** Locker ");
	strcat(str, num);
	strcat(str, " info **\nCargo name = ");
	strcat(str, record.name);
	strcat(str, "\nLocker password = ");
	strcat(str, record.passwd);
	strcat(str, "\n\nTake cargo : 1\nModify cargo : 2\nChange password : 3\nExit : 4\nYour input >> ");
	write(clientFd, str, strlen(str) + 1);
	
	readLine(clientFd, recvmsg); // recieve client's answer
	if (recvmsg[0] == '1') { // take
		// remove data on record
		memset(record.name, 0, sizeof(record.name));
		memset(record.passwd, 0, sizeof(record.passwd));
		lseek(recordFd, (long)(inum - 1) * sizeof(record), SEEK_SET);
		write(recordFd, &record, sizeof(record));
		
		// remove data on lockerstatus
		chtemp = '0';
		lseek(fd, (long)(inum - 1) * sizeof(char), SEEK_SET);
		write(fd, &chtemp, sizeof(chtemp));
		
		char tmplst[] = "\nYour cargo has been successfully removed on the server.\n";
		write(clientFd, tmplst, strlen(tmplst) + 1);
	} else if (recvmsg[0] == '2') { // modify
		memset(str, 0, sizeof(str));
		strcat(str, "\nType your new cargo's name. It will be saved on the server.\nYour input >> ");
		write(clientFd, str, strlen(str) + 1);
		
		readLine(clientFd, recvmsg);
		for (int i = 0; i < strlen(recvmsg); i++) { // removing '\n'
			if (recvmsg[i] == '\n') {
				recvmsg[i] = 0;
				break;
			}
		}
		
		memset(record.name, 0, sizeof(record.name));
		strcpy(record.name, recvmsg);
		lseek(recordFd, (long)(inum - 1) * sizeof(record), SEEK_SET);
		write(recordFd, &record, sizeof(record));
		
		char tmplst[] = "\nYour cargo has been successfully changed.\n";
		write(clientFd, tmplst, strlen(tmplst) + 1);
	} else if (recvmsg[0] == '3') { // change passwd
		memset(str, 0, sizeof(str));
		strcat(str, "\nType new password for your locker.\nYour input >> ");
		write(clientFd, str, strlen(str) + 1);
		
		readLine(clientFd, recvmsg);
		for (int i = 0; i < strlen(recvmsg); i++) { // removing '\n'
			if (recvmsg[i] == '\n') {
				recvmsg[i] = 0;
				break;
			}
		}
		
		char recvmsg2[MAXSUBLINE];
		memset(str, 0, sizeof(str));
		strcat(str, "\nPlease type your password one more time.\nYour input >> ");
		write(clientFd, str, strlen(str) + 1);
		
		readLine(clientFd, recvmsg2);
		for (int i = 0; i < strlen(recvmsg2); i++) { // removing '\n'
			if (recvmsg2[i] == '\n') {
				recvmsg2[i] = 0;
				break;
			}
		}
		
		// check if it is same
		flag = 1;
		for (int i = 0; i < 100; i++) {
			if (recvmsg[i] != recvmsg2[i]) { flag = 0; break; }
			if (recvmsg[i] == 0 && recvmsg2[i] == 0) break;
		}
		
		memset(str, 0, sizeof(str));
		if (flag) {
			strcat(str, "\nYour cargo is completly stored!\n\n");
			write(clientFd, str, strlen(str) + 1);
		} else {
			strcat(str, "\nYou typed wrong password. Please try again later.\n\n");
			write(clientFd, str, strlen(str) + 1);
			
			free(lockerstatus);
			close(fd);
			close(recordFd);
			return;
		}
		
		memset(record.passwd, 0, sizeof(record.passwd));
		strcpy(record.passwd, recvmsg);
		
		lseek(recordFd, (long)(inum - 1) * sizeof(record), SEEK_SET);
		write(recordFd, &record, sizeof(record)); // write on record file
		
	} else if (recvmsg[0] == '4') { // exit
		char tmplst[] = "\nNothing changed.\n";
		write(clientFd, tmplst, strlen(tmplst) + 1);
	} else { // error
		char tmplst[] = "\nInput error, nothing changed.\n";
		write(clientFd, tmplst, strlen(tmplst) + 1);
	}
	
	free(lockerstatus);
	close(fd);
	close(recordFd);
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
		if (str[0] == '0') { // void
			printf("This locker is clean! Please retry with other number.\n\n");
			sleep(1);
		} else if (str[0] == '1') { // in use
			break;
		} else { // error
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
	
	if (sendmsg[0] == '1') { // remove
		readLine(clientFd, str);
		printf("%s", str);
	} else if (sendmsg[0] == '2') { // modify
		readLine(clientFd, str);
		printf("%s", str);
		
		memset(sendmsg, 0, sizeof(sendmsg));
		fgets(sendmsg, MAXSUBLINE, stdin);
		write(clientFd, sendmsg, strlen(sendmsg) + 1);
		
		readLine(clientFd, str);
		printf("%s", str);
	} else if (sendmsg[0] == '3') { // change password
		readLine(clientFd, str);
		printf("%s", str);
		
		memset(sendmsg, 0, sizeof(sendmsg)); // password input
		fgets(sendmsg, MAXSUBLINE, stdin);
		write(clientFd, sendmsg, strlen(sendmsg) + 1);
		
		readLine(clientFd, str);
		printf("%s", str);
		
		memset(sendmsg, 0, sizeof(sendmsg)); // again password input
		fgets(sendmsg, MAXSUBLINE, stdin);
		write(clientFd, sendmsg, strlen(sendmsg) + 1);
		
		readLine(clientFd, str);
		printf("%s", str);
	} else if (sendmsg[0] == '4') { // exit
		readLine(clientFd, str);
		printf("%s", str);
	} else { // error
		readLine(clientFd, str);
		printf("%s", str);
	}
	
	
	printf("Return to menu...\n\n");
	sleep(1);
}
