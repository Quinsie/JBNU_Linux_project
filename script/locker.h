#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 100
#define BASE_NUM 1

struct locker {
	char passwd[MAXSIZE];
	char content[MAXSIZE];
};
