#ifndef __DFS_STACK_H
#define __DFS_STACK_H

#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

// Currently prints the tokens in the list as seperate entities
int parse(char * str){
 target firstTarget;
	int init_size = strlen(str);
	char delim[] = " ";
 int count = 0;

	char *ptr = strtok(str, delim);

	while(ptr != NULL)
	{
		printf("'%s'\n", ptr);
		ptr = strtok(NULL, delim);
	}

	return 0;
}

#endif
