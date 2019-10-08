#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "parser.h"

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
			printf("\nTarget '%s' has %d dependancies and %d recipes\n",
																									targets[i].name,
																				 		 		  targets[i].dep_count,
																					        targets[i].recipe_count);
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

//$./mymake Makefile target
int executeTarget(struct target_block targets[128], char* tar) {
	for(int i = 0; i < targetLength; i++) {
		if (targets[i].name != NULL) {
			if (compareStr(targets[i].name, tar) == 0) {
				printRecipes(targets[i]);
				// NEED TO IMPLEMENT (Call the printRecipes offshoot method that executes instead of prints with this target[i])
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

	//$./mymake Makefile
	//Similarly account for -r flag
	if (argc == 2 && strncmp(argv[1], "-p", 2)) {
		process_file(argv[1]);
		//TODO
	}
	if (argc == 3) {
		//$./mymake Makefile target
		if (strncmp(argv[1], "-p", 2) && strncmp(argv[1], "-r", 2)
		 	 && !process_file(argv[1])) {
				 if (!strncmp(argv[2], "-p", 2) || !strncmp(argv[2], "-r", 2)) {
					 printf("Incorrect execution order\n");
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
