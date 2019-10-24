// Add Guard to header file
// Function prototypes for
//			map function that will go through text files in the Mapper_i.txt to get the (letter, wordcount)

#ifndef PHASE2_H
#define PHASE2_H

#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include "dirent.h"

typedef struct dictionary {
	char letter;
  int count;
} dict;

int MaptoProc(int n, int p[]);


#endif
