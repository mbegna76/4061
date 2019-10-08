#ifndef __PARSER_H
#define __PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "graph.h"

#include <limits.h>

// Array Stack //

struct Stack {
	int top;
	unsigned capacity;
	int* array;
};

int isFull(struct Stack* stack) {
	return stack->top == stack->capacity - 1;
}

int isEmpty(struct Stack* stack) {
	return stack->top == -1;
}

void push (struct Stack* stack, int item) {
	if (isFull(stack)) {
		return;
	}
	stack->array[++stack->top] = item;
	printf("%d pushed to stack\n", item);

}

int peek(struct Stack* stack)
{
    if (isEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top];
}


int pop(struct Stack* stack) {
    if (isEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top--];
}

char* removeSpaces(char* input){
     while(isspace(*input)) input++;
     return input;
}

char* removeColon(char* input){
     char *output = input;
     int i = 0;
     for (i; i < strlen(input); i++) {
       if (input[i] == ':') {
         break;
       }
       output[i] = input[i];
     }
     output[i] = '\0';
     return output;
}

// End of Array Stack Operation //

// Currently prints the tokens in the 2d list as seperate entities
int parse(char str[MAX_LINES][LINE_SIZE]){
  int isRecipe = -1;
  int targetIndex = -1;
  int recipeIndex = -1;
  for (int i = 0; i < MAX_LINES; i++) {
    // Ignore blank lines
    if (strlen(str[i]) < 2) {
      continue;
    } else {
      // This is a target line contain ':'
      if (strstr(str[i], ":") != NULL) {
        isRecipe = 0;
        targetIndex++;
        recipeIndex = -1; // reset index of recipe
      } else { // This is a recipe line
        isRecipe = -1;
        recipeIndex ++;
      }
      // This is a target string
      if (!isRecipe) {
        char delim[] = " ";

        char *ptr = strtok(str[i], delim);

        int firstToken = 1;
        int dependIndex = 0;
        // ptr points to each token
        while(ptr != NULL)
        {
          //only the first token is the targetName
          if (firstToken == 1) {
            targets[targetIndex].name = removeColon(ptr);
            firstToken--;
          } else {
            if (strlen(ptr) >= 2) { //Excludes ':' and extra spaces
              targets[targetIndex].dep_count++;
              targets[targetIndex].depend[dependIndex] = ptr;
              dependIndex++;
            }
          }
          ptr = strtok(NULL, delim);
        }
      } else {
        targets[targetIndex].recipe_count++;
        targets[targetIndex].recipe[recipeIndex] = removeSpaces(str[i]);
      }
    }
  }

	return 0;
}


#endif
