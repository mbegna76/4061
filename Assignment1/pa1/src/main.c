#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "parser.h"

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//Parse the input makefile to determine targets, dependencies, and recipes
int process_file(char *fname)
{
	FILE* fp = fopen(fname, "r");
	char line[LINE_SIZE];
	int i = 0;

	if (!fp) {
		printf("Failed to open the file: %s \n", fname);
		return -1;
	}

	//Read the contents and store in lines
	while (fgets(line, LINE_SIZE, fp))
		strncpy(lines[i++], line, strlen(line));

	fclose(fp);
	return 0;
}

int compareStr(char *str1, char *str2) {
	for (int i = 0; i < strlen(str1); i ++) {
		if (str1[i] != str2[i]) {
			return 1;
		}
	}
	return 0;
}

//$./mymake -r Makefile
void printRecipes(struct target_block current) {
	for (int i = 0; i < current.dep_count; i++) {
		for (int j = 0; j < targetLength; j++) {
			if (targets[j].name != NULL) {
				if (compareStr(targets[j].name, current.depend[i]) == 0) {
					printRecipes(targets[j]);
				}
			}
		}
	}
	for (int i = 0; i < current.recipe_count; i++) {
		if (current.recipe[i] != NULL) {
			printf("%s", current.recipe[i]);
		}
	}
}

//$./mymake -p Makefile
void printTargets(struct target_block targets[128]) {
	for(int i = 0; i < targetLength; i++) {
		if (targets[i].name != NULL) {
			printf("\nTarget '%s' has %d dependencies and %d recipes\n",
			targets[i].name,targets[i].dep_count, targets[i].recipe_count);
			for (int j = 0; j < MAX_DEP; j++) {
				if (targets[i].depend[j] != NULL) {
					printf("Dependency %d is %s\n", j, targets[i].depend[j]);
				}
			}
			for (int j = 0; j < MAX_RECIPES_PT; j++) {
				if (targets[i].recipe[j] != NULL) {
					printf("Recipe %d is %s", j, targets[i].recipe[j]);
				}
			}
		}
	}
}

void execute(struct target_block current) {

	for (int i = 0; i < current.dep_count; i++) {
		for (int j = 0; j < targetLength; j++) {
			if (targets[j].name != NULL) {
				if (compareStr(targets[j].name, current.depend[i]) == 0) {
					execute(targets[j]);
				}
			}
		}
	}

	for (int i = 0; i < current.recipe_count; i++) {
		if (current.recipe[i] != NULL) {
			// tokenize the recipe array to make passable arguments into exec
			char delim[] = " ";
			char* rectok[100] = {0};
			char *ptr = strtok(current.recipe[i], delim);
			int rectok_index = 0;
			int argsize = 0;
			// ptr points to each recipe token
			while(ptr != NULL) {
				rectok[rectok_index] = ptr;
				rectok_index++;
				ptr = strtok(NULL, delim);
			}

			char *cmd = rectok[0];	// set the first cmd parameter of execv

			for (int j = 1; j < 100 ; j++) {	// find the size of the arg array
				if (rectok[j] == NULL) {
					break;
				}
				else {
					argsize++;
				}
			}

		char* arg[argsize + 1];	// create arg array
		arg[argsize] = NULL;
		int j = 0;

		for (int h = 1; h < argsize +1 ; h++, j++) {	// make the argument array with the tokenized recipe
			arg[j] = rectok[h];
		}

		// now we have const char cmd* and const char args*

		pid_t kidpid = fork();	// create child process

		if (kidpid > 0) {		// if in parent wait
			wait(NULL);
		}

		else {					// in child, execute
			execvp(cmd, arg);
			exit(1);
		}


		}
	}
}

//$./mymake Makefile target
int executeTarget(struct target_block targets[128], char* tar) {
	for(int i = 0; i < targetLength; i++) {
		if (targets[i].name != NULL) {
			if (compareStr(targets[i].name, tar) == 0) {
				printRecipes(targets[i]);
				execute(targets[i]);
				return 0;
			}
		}
	}
	printf("Target does not exist\n");
	return -1;
}

//Validate the input arguments, bullet proof the program
int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Too little arguments given\n");
	}

	//$./mymake Makefile
	if (argc == 2 && !process_file(argv[1])) {
		parse(lines);
		executeTarget(targets, targets[0].name);
		//TODO
	}
	if (argc == 3) {
		//$./mymake Makefile target
		if (strncmp(argv[1], "-p", 2) && strncmp(argv[1], "-r", 2)
		 	 && !process_file(argv[1])) {
				 if (!strncmp(argv[2], "-p", 2) || !strncmp(argv[2], "-r", 2)) {
					 printf("Incorrect argument order\n");
				 } else {
					 parse(lines);
					 executeTarget(targets, argv[2]);
				 }

		} else if (!strncmp(argv[1], "-p", 2)) {
			//$./mymake -p Makefile
			if (!process_file(argv[2])) {
				parse(lines);
				printTargets(targets);
			}
		} else if (!strncmp(argv[1], "-r", 2)) {
			//$./mymake -r Makefile
			if (!process_file(argv[2])) {
				parse(lines);
				printRecipes(targets[0]);
			}
		}
	}
	if (argc > 3) {
		printf("Too many arguments given\n");
	}
	exit(EXIT_SUCCESS);
}
