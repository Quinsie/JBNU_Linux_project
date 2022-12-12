#include "store.h"

// server part
void store(const int clientFd, const int lockerlen)
{
	char str[MAXLINE] = "", num[5], chtemp, recvmsg[MAXSUBLINE];
	int inum, fd, flag, recordFd;
	int* voidlocker;
	struct locker record;
	struct flock lock;
	
	fd = open("../resource/status", O_RDWR);
	lseek(fd, 0, SEEK_SET);
	voidlocker = (int*)calloc(lockerlen, sizeof(int));
	
	strcat(str, "\n\n** STORE CARGO **\n");
	strcat(str, "You can store your cargo on this server.\n\nVOID lockers are here : ");
	
	for (int i = 0; i < lockerlen; i++) {
		sprintf(num, "%d", i + 1);
		read(fd, &chtemp, sizeof(chtemp));
		if (chtemp == '0') {
			strcat(str, num);
			strcat(str, " ");
		} else voidlocker[i] = 1;
	}
	strcat(str, "\n\n");
	
	write(clientFd, str, strlen(str) + 1);
	memset(str, 0, sizeof(str));
	strcat(str, "Select a locker number where you want to load your cargo. If press 0, you will return to menu.\n");
	strcat(str, "Your input >> ");
	
	while (1) {
		write(clientFd, str, strlen(str) + 1); // send selection
		readLine(clientFd, recvmsg);
		
		inum = atoi(recvmsg);
		if (inum > lockerlen || inum < 0) { // out of order
			char tmp[] = "1";
			write(clientFd, tmp, strlen(tmp) + 1);
			continue;
		} else if (voidlocker[inum - 1]) { // in use
			char tmp[] = "1";
			write(clientFd, tmp, strlen(tmp) + 1);
			continue;
		} else if (inum == 0) { // go to menu
			free(voidlocker);
			close(fd);
			return;
		} else { // void
			char tmp[] = "0";
			write(clientFd, tmp, strlen(tmp) + 1);
			break;
		}
	}
	
	// lock
	recordFd = open("../resource/record", O_RDWR);
	
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = (inum - 1) * sizeof(record);
	lock.l_len = sizeof(record);
	fcntl(recordFd, F_SETLKW, &lock);
	
	lseek(fd, (long)(inum - 1) * sizeof(chtemp), SEEK_SET);
	read(fd, &chtemp, sizeof(chtemp));
	
	if (chtemp == '1') {
		char tmp[] = "1";
		write(clientFd, tmp, strlen(tmp) + 1);
		free(voidlocker);
		close(fd);
		close(recordFd);
		return;
	} else {
		char tmp[] = "0";
		write(clientFd, tmp, strlen(tmp) + 1);
	}
	
	lseek(recordFd, (long)(inum - 1) * sizeof(record), SEEK_SET);
	
	// cargo name
	memset(str, 0, sizeof(str));
	strcat(str, "\nPlease type your cargo's name.\nYour input >> ");
	write(clientFd, str, strlen(str) + 1);
	
	readLine(clientFd, record.name);
	for (int i = 0; i < strlen(record.name); i++) { // removing '\n'
		if (record.name[i] == '\n') {
			record.name[i] = 0;
			break;
		}
	}
	
	// password
	memset(str, 0, sizeof(str));
	strcat(str, "\nPlease type your locker's password.\nYour input >> ");
	write(clientFd, str, strlen(str) + 1);
	
	readLine(clientFd, record.passwd);
	for (int i = 0; i < strlen(record.passwd); i++) { // removing '\n'
		if (record.passwd[i] == '\n') {
			record.passwd[i] = 0;
			break;
		}
	}
	
	
	// again password
	memset(str, 0, sizeof(str));
	strcat(str, "\nPlease type your password one more time.\nYour input >> ");
	write(clientFd, str, strlen(str) + 1);
	
	readLine(clientFd, recvmsg);
	for (int i = 0; i < strlen(recvmsg); i++) { // removing '\n'
		if (recvmsg[i] == '\n') {
			recvmsg[i] = 0;
			break;
		}
	}
	
	// match or not
	flag = 1;
	for (int i = 0; i < 100; i++) {
		if (record.passwd[i] != recvmsg[i]) { flag = 0; break; }
		if (record.passwd[i] == 0 && recvmsg[i] == 0) break;
	}
	
	memset(str, 0, sizeof(str));
	if (flag) {
		strcat(str, "\nYour cargo is completly stored!\n\n");
		write(clientFd, str, strlen(str) + 1);
	} else {
		strcat(str, "\nYou typed wrong password. Please try again later.\n\n");
		write(clientFd, str, strlen(str) + 1);
		lock.l_type = F_UNLCK;
		fcntl(recordFd, F_SETLK, &lock);
		free(voidlocker);
		close(fd);
		close(recordFd);
		return;
	}
	
	write(recordFd, &record, sizeof(record));
	
	chtemp = '1';
	lseek(fd, (long)(inum - 1) * sizeof(chtemp), SEEK_SET);
	write(fd, &chtemp, sizeof(chtemp));
	
	// unlock
	lock.l_type = F_UNLCK;
	fcntl(recordFd, F_SETLK, &lock);
	
	free(voidlocker);
	close(fd);
	close(recordFd);
}

// client part
void client_store(const int clientFd)
{
	char str[MAXLINE], sendmsg[MAXSUBLINE];
	
	readLine(clientFd, str);
	printf("%s", str); // check completed (debug)
	
	while (1) {
		readLine(clientFd, str);
		printf("%s", str);
		
		fgets(sendmsg, MAXSUBLINE, stdin); // client answer (number)
		write(clientFd, sendmsg, strlen(sendmsg) + 1);
		
		if (sendmsg[0] == '0') { // return to menu
			printf("Return to menu...\n\n");
			sleep(1);
			return;
		}
		
		memset(sendmsg, 0, sizeof(sendmsg));
		
		readLine(clientFd, str);
		if (str[0] == '1') {
			printf("This number is unavailable. Please retry.\n\n");
			sleep(1);
		} else break;
	}
	
	// lockmsg
	readLine(clientFd, str);
	if (str[0] == '1') {
		printf("Sorry, this locker has been filled just ago. Please try again.\n");
		printf("Return to menu...\n\n");
		sleep(1);
		return;
	}
	
	// cargo's name
	readLine(clientFd, str); // cargo's name
	printf("%s", str);
	memset(sendmsg, 0, sizeof(sendmsg));
	fgets(sendmsg, MAXSUBLINE, stdin);
	write(clientFd, sendmsg, strlen(sendmsg) + 1); // send cargo's name
	
	// password
	readLine(clientFd, str); // password
	printf("%s", str);
	memset(sendmsg, 0, sizeof(sendmsg));
	fgets(sendmsg, MAXSUBLINE, stdin);
	write(clientFd, sendmsg, strlen(sendmsg) + 1); // send password
	
	// password again
	readLine(clientFd, str);
	printf("%s", str);
	memset(sendmsg, 0, sizeof(sendmsg));
	fgets(sendmsg, MAXSUBLINE, stdin);
	write(clientFd, sendmsg, strlen(sendmsg) + 1); // send again password
	
	// stored?
	readLine(clientFd, str);
	printf("%s", str);
	
	printf("Return to menu...\n\n");
	sleep(1);
}
