#include "management.h"
#include "locker.h"

void base_helper(int* lockerlen, int** lockerstatus)
{
	int num, record;
	// int number, big, mid, small;
	
	printf("Server has opened.\n");
	record = open("../resource/record", O_WRONLY|O_CREAT|O_TRUNC, 0640); // make locker file
	
	printf("Please type total size of locker : ");
	scanf("%d", &num);
	*lockerlen = num;
	//number = lockerlen;
	
	*lockerstatus = (int*)calloc(*lockerlen, sizeof(int)); // initialize status of locker
	printf("Locker has initialized with size of %d.\n", *lockerlen);
	
	
	/*
	while (1) {
		printf("Please type the number of big size locker. It must be less than %d.\nInput : ", number);
		scanf("%d", &big);
		
		if (big > number) {
			printf("Input error. Please type again.\n");
			continue;
		}
		
		number -= big;
	}
	*/
	
	close(record);
}

int menu(const int fd, const char order, const int lockerlen, int* lockerstatus)
{
	if (order == '1') { // check
		char str[MAXLINE] = "** LOCKER STATUS **\n";
		
		for (int i = 0; i < lockerlen; i++) {
			char st[MAXSUBLINE] = "LOCKER "; char num[5];
			
			sprintf(num, "%d", i + 1);
			strcat(st, num);
			if (lockerstatus[i]) strcat(st, " : IN USE\n");
			else strcat(st, " : VOID\n");
			strcat(str, st);
		}
		
		write(fd, str, strlen(str) + 1);		
		
	} else if (order == '2') { // load
		char str[MAXLINE] = "** LOAD CARGO **\nYou can load your cargo on this tab.\n\nVOID lockers are here : \n";
		char recv[MAXSUBLINE], chtemp[MAXSUBLINE];
		int num, record_fd;
		struct locker record;
		
		for (int i = 0; i < lockerlen; i++) {
			char num[5];
			if (!lockerstatus[i]) {
				sprintf(num, "%d ", i + 1);
				strcat(str, num);
			}	
		}
		
		write(fd, str, strlen(str) + 1); // send selection
		memset(str, 0, sizeof(str));
		strcat(str, "\nSelect a locker number where you want to load your cargo.\nYour input >> ");
		
		while (1) {
			write(fd, str, strlen(str) + 1); // send order
			readLine(fd, recv); // recieve client's answer
			num = atoi(recv);
			
			if (num > lockerlen || num <= 0) {
				char tmp[] = "1";
				write(fd, tmp, strlen(tmp) + 1);
				continue;
			} else if (lockerstatus[num - 1]) { // in use
				char tmp[] = "1";
				write(fd, tmp, strlen(tmp) + 1);
				continue;
			} else { // void
				char tmp[] = "0";
				write(fd, tmp, strlen(tmp) + 1);
				break;
			}
		}
		
		memset(str, 0, sizeof(str));
		strcat(str, "\nPlease type your cargo's name.\nYour input >> ");
		write(fd, str, strlen(str) + 1);
		
		readLine(fd, recv); // recieve cargo name
		recv[strlen(recv) - 1] = '\0';
		strncpy(record.name, recv, strlen(recv));
		fprintf(stderr, "%s %d\n", record.name, strlen(record.name));
		/*
		memset(str, 0, sizeof(str));
		strcpy(str, "\nLocker Password : ");
		write(fd, str, strlen(str + 1));
		
		sleep(1);
		readLine(fd, recv); // recieve password
		recv[strlen(recv) - 1] = '\0';
		strncpy(record.passwd, recv, strlen(recv));
		//fprintf(stderr, "%s %d\n", record.passwd, strlen(record.passwd));

		memset(str, 0, sizeof(str));
		strcat(str, "\nType your password again : ");
		write(fd, str, strlen(str + 1));
		
		sleep(1);
		readLine(fd, recv); // recieve again password
		recv[strlen(recv) - 1] = '\0';
		strncpy(chtemp, recv, strlen(recv));
		if (chtemp == record.passwd) { // correct
			char tmp[] = "1";
			write(fd, tmp, strlen(tmp) + 1);
		} else { // wrong
			char tmp[] = "0";
			write(fd, tmp, strlen(tmp) + 1);
			return 0;
		}
		
		record_fd = open("../resource/record", O_RDWR);
		lseek(record_fd, (long)(num - BASE_NUM) * sizeof(record), SEEK_SET);
		write(record_fd, &record, sizeof(record));
		close(record_fd);
		lockerstatus[num - BASE_NUM] = 1;
*/
	} else if (order == '3') { // check or take cargo
		char str[] = "You selected 3.\n";
		printf("Client selected 3.\n");
		write(fd, str, strlen(str) + 1);
		
	} else if (order == '4') { // terminate
		return 1;
	} else {
		char str[] = "Invaild input. Please type again.\n";
		write(fd, str, strlen(str) + 1);
	}
	
	return 0;
}
