// Add Guard to header file (google what are guards)
// Function prototypes to
//			Traverse the Folder
//			Partition the text file paths to 'm' files
#ifndef PHASE1_H
#define PHASE1_H

#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dirent.h"

void partition(int n);

void traversal(DIR* dir, char* dirName);


#endif
