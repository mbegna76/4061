#ifndef __DFS_STACK_H
#define __DFS_STACK_H

#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

void removeSpaces(char* s) {
    const char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}

// Currently prints the tokens in the 2d list as seperate entities
int parse(char str[MAX_LINES][LINE_SIZE]){
  int isRecipe = -1;
  for (int i = 0; i < MAX_LINES; i++) {
    // Ignore blank lines
    if (strlen(str[i]) < 2) {
      continue;
    } else {
      // Target lines contain ':'
      if (strstr(str[i], ":") != NULL) {
        isRecipe = 0;
        printf("-----This is a target line-----\n");
      } else {
        isRecipe = -1;
        printf("-----This is a recipe line-----\n");
      }
      // Tokenize the dependancies
      if (!isRecipe) {
        char delim[] = " ";

        char *ptr = strtok(str[i], delim);

        // ptr points to each token
        while(ptr != NULL)
        {
          removeSpaces(ptr);
          printf("%s\n", ptr);
          ptr = strtok(NULL, delim);
        }
      } else {
        printf("%s\n", str[i]);
      }
    }
  }

	return 0;
}


#endif
