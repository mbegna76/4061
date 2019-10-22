#include "./../include/main.h"


int main(int argc, char *argv[]){
	if (argc < 3) {
		printf("Too little arguments\n");
	} else if (argc > 3){
		printf("Too many arguments\n");
	} else {
		partition(atoi(argv[2]));
		DIR *dirp;
		dirp = opendir(argv[1]);
		traversal(dirp, argv[1], atoi(argv[2]));
	}



	// argument count check

	//just make a function call to code in phase1.c
	//phase1 - Data Partition Phase

	//create pipes

	//just make a function call to code in phase2.c
	//phase2 - Map Function

	//just make a function call to code in phase3.c
	//phase3 - Reduce Function

	//phase4
	//wait for all processes to reach this point

	//just make a function call to code in phase4.c
	//master process reports the final output

	return 0;

}
