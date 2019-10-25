/*test machine: CSELAB_machine_name * date: 10/25/19
* name: Moti Begna , Wally Lu
* x500: begna002 , luxx0460 */


#include "./../include/main.h"


int main(int argc, char *argv[]){
	if (argc < 3) {
		printf("Too little arguments\n");
	} else if (argc > 3){
		printf("Too many arguments\n");
	} else {
		//phase1 - Data Partition Phase
		partition(atoi(argv[2]));
		DIR *dirp;
		dirp = opendir(argv[1]);
		traversal(dirp, argv[1], atoi(argv[2]));

		//phase2 - Map Function
		//Creating Pipes to pass to phase2
		int p[atoi(argv[2])*2];
		for (int i = 0; i < atoi(argv[2]); i++) {
			pipe(p + (2 * i));
		}
		int processValue = MaptoProc(atoi(argv[2]), p);

		//phase3 - Reduce Function
		//only reducer process calls phase3
		if (processValue == 1) {
			processPipe(p, atoi(argv[2]));
		}
		// phase4
		finalResult();
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
