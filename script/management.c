#include "management.h"

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
			if (lockerstatus[i]) { // in use
				char tmp[] = " : IN USE\n";
				strcat(st, tmp);
			} else { // void
				char tmp[] = " : VOID\n";
				strcat(st, tmp);
			}
			strcat(str, st);
		}
		
		write(fd, str, strlen(str) + 1);		
		
	} else if (order == '2') { // load
		char str[] = "You selected 2.\n";
		printf("Client selected 2.\n");
		write(fd, str, strlen(str) + 1);
		
	} else if (order == '3') { // take
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
