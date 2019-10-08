#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "dfs_stack.h"

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

void printTargets(struct target_block items[8]) {
	for(int i = 0; i < targetLength; i++) {
		if (targets[i].name != NULL) {
			printf("\nTarget '%s' has %d dependancies and %d recipes\n", 
			targets[i].name,targets[i].dep_count, targets[i].recipe_count);
			for (int j = 0; j < MAX_DEP; j++) {
				if (targets[i].depend[j] != NULL) {
					printf("Dependancy %d is %s\n", j, targets[i].depend[j]);
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

int dep[5]; // array of dependencies to keep track of dep[0] = dep 0, dep[1] = dep 1, etc. 



void Traversal(struct target_block items[8]) {
	for(int i = 0; i < targetLength; i++) {
		if (targets[i].name != NULL) {
			for (int j = 0; j < MAX_DEP; j++) {
				if (targets[i].depend[j] != NULL) {
					dep[j] = 1; // if there's dependencies, keep track of all of them
					
				}
			}
		}
	}
}

//Validate the input arguments, bullet proof the program
int main(int argc, char *argv[])
{
	//$./mymake Makefile
	//Similarly account for -r flag
	if (argc == 2 && strncmp(argv[1], "-p", 2)) {
		process_file(argv[1]);
		//TODO
	}

	if (argc == 3) {
		//$./mymake Makefile target
		if (strncmp(argv[1], "-p", 2) && !process_file(argv[1])) {
			//TODO

		} else if (!strncmp(argv[1], "-p", 2)) {
			//$./mymake -p Makefile
			if (!process_file(argv[2])) {
				parse(lines);
				printTargets(targets);
			}
		}
	}
	exit(EXIT_SUCCESS);
}
