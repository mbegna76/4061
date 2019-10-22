// You are free to use your own logic. The following points are just for getting started
/* 	Data Partitioning Phase - Only Master process involved
	1) 	Create 'MapperInput' folder
	2) 	Traverse the 'Sample' folder hierarchy and insert the text file paths
		to Mapper_i.txt in a load balanced manner
	3) 	Ensure to keep track of the number of text files for empty folder condition

*/
#include "./../include/phase1.h"

int mapperId = 1;

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
      fclose(fp);
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

void traversal(DIR* dir, char* dirName, int mapperTotal){
  struct dirent *de;
  while ((de = readdir(dir)) != NULL) {
    char* str = de->d_name;
    struct stat *states;
    // This is a file
    if ((strstr(str, ".") != NULL)) {
      if (strlen(str) < 4) {
        continue;
      }
      int len = strlen(dirName) + strlen(str);
      char *fileNameBuffer = (char*)malloc(len * sizeof(char));
      sprintf(fileNameBuffer, "%s/%s\n", dirName, str);
      printf("THIS IS A FILE: %s", fileNameBuffer);

      int mapperNameSize = 0;
      if (mapperId >= 10){
        mapperNameSize = 27;
      } else {
        mapperNameSize = 26;
      }

      char *mapperName = (char*)malloc(mapperNameSize* sizeof(char));
      sprintf(mapperName, "./MapperInput/Mapper_%d.txt", mapperId);
      FILE *fp;
      fp = fopen(mapperName, "a");  //write and read mode
      fprintf(fp, fileNameBuffer);
      fclose(fp);

      mapperId++;
      if (mapperId > mapperTotal) {
        mapperId = 1;
      }
    }
    // This is a directory
    else if (stat(str, states) == -1){
      DIR *dirp;
      int len = strlen(dirName) + strlen(str) - 1;
      char *folderNameBuffer = (char*)malloc(len * sizeof(char));
      sprintf(folderNameBuffer, "%s/%s", dirName, str);
      printf("THIS IS A DIR:%s\n", folderNameBuffer);
      dirp = opendir(folderNameBuffer);
      traversal(dirp, folderNameBuffer, mapperTotal);
    } else {
      printf("THIS IS A FILE: %s\n", str);
    }
  }
}
