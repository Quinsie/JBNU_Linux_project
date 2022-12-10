#include "readline.h"

int readLine(int fd, char* str)
{
	int n;
	do {
		n = read(fd, str, 1);
		} while(n > 0 && *str++ != NULL);
	return(n > 0);
}
