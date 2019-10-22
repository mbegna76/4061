// You are free to use your own logic. The following points are just for getting started
/* 	Data Partitioning Phase - Only Master process involved
	1) 	Create 'MapperInput' folder
	2) 	Traverse the 'Sample' folder hierarchy and insert the text file paths
		to Mapper_i.txt in a load balanced manner
	3) 	Ensure to keep track of the number of text files for empty folder condition

*/
#include "./../include/phase1.h"

void partition(int n) {
  mkdir("MapperInput",777);
  if (n < 10) {
    while (n > 0) {
      FILE * fp;
      int i;
      char *fileNameBuffer = (char*)malloc(26 * sizeof(char));
      sprintf(fileNameBuffer, "./MapperInput/Mapper_%d.txt", n);
      fp = fopen (fileNameBuffer,"w");
      free(fileNameBuffer);
      n--;
    }

  } else {
    while (n > 0) {
      FILE * fp;
      int i;
      char *fileNameBuffer = (char*)malloc(27 * sizeof(char));
      sprintf(fileNameBuffer, "./MapperInput/Mapper_%d.txt", n);
      fp = fopen (fileNameBuffer,"w");
      free(fileNameBuffer);
      n--;
    }
  }
}
