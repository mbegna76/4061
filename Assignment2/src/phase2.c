#include "./../include/phase2.h"
// You are free to use your own logic. The following points are just for getting started
/* 	Map Function
	1)	Each mapper selects a Mapper_i.txt to work with
	2)	Creates a list of letter, wordcount from the text files found in the Mapper_i.txt
	3)	Send the list to Reducer via pipes with proper closing of ends
*/

void MaptoProc(mapperTotal) { // Function to create an array of processes
  pid_t* mappedProcs[mapperTotal];
  pid_t oldboy = fork(); // Need an initial fork call to work in child process
  if (oldboy > 0) {
    wait(NULL);
  }
  else {
    int i = 0;
    while (i < mapperTotal) {
        mappedProcs[i] = fork();
        i++;
    }
  }
}
