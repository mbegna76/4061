#include "./../include/phase4.h"
// You are free to use your own logic. The following points are just for getting started
/* Final Result
	1)	The master process reads the "ReducerResult.txt"
	2) 	Print the contents to standard output which is redirected to "FinalResult.txt"
			i.e., on using printf, the statement should be written to "FinalResult.txt"
*/

void finalResult() {
  printf("This is the master\n");
  int out = open("FinalResult.txt", O_RDWR|O_CREAT|O_TRUNC);
  int tempOut = dup(fileno(stdout));
  dup2(out, fileno(stdout));

  FILE * in;
  in = fopen ("ReducerResult.txt","r");
  char fileBuff[255];

  if (in == NULL) {
    printf("failed to open file\n");
  } else {
    while (fscanf(in, "%s", fileBuff) != -1){
      printf("%s ", fileBuff);
      fscanf(in, "%s", fileBuff);
      printf("%s\n", fileBuff);
    }
  }

  fflush(stdout);
  close(out);

  //Restore standard io;
  dup2(tempOut, fileno(stdout));
  printf("Done\n");

  fclose(in);
}
