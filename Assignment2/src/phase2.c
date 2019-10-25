#include "./../include/phase2.h"
// You are free to use your own logic. The following points are just for getting started
/* 	Map Function
	1)	Each mapper selects a Mapper_i.txt to work with
	2)	Creates a list of letter, wordcount from the text files found in the Mapper_i.txt
	3)	Send the list to Reducer via pipes with proper closing of ends
*/


char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int MaptoProc(int mapperTotal, int p[]) { // Function to create an array of processes
  pid_t mappedProcs[mapperTotal];
  int status;

  // Create 'mapperTotal' pipes

  pid_t reducer;
  reducer = fork();

  //This is the master process
  if (reducer > 0){
    // Close read end of pipes
    for (int i = 0; i < mapperTotal*2; i+=2) {
      close(p[i]);
    }
    int i = 0;
    int mapperIndex = 1;
    int pipeWriteIndex = 1;
    int pathSize;
    while (i < mapperTotal) {
        mappedProcs[i] = fork();
        if (mappedProcs[i] > 0) {
          if (i < 10) {
            pathSize = 26;
          } else {
            pathSize = 27;
          }
          FILE * fp;
          char *fileNameBuffer = (char*)malloc(pathSize * sizeof(char));
          sprintf(fileNameBuffer, "./MapperInput/Mapper_%d.txt", mapperIndex);
          fp = fopen (fileNameBuffer,"r");
          char fileBuff[255];
          char lineBuff[255];
          dict structList[26];

          for (int j = 0; j < 26; j++) {
            structList[j].letter = toupper(alphabet[j]);
            structList[j].count = 0;
          }

          while (fscanf(fp, "%s", fileBuff) != -1){
            FILE * tp;
            tp = fopen(fileBuff, "r");
            while (fscanf(tp, "%s", lineBuff) != -1){
              for (int j = 0; j < 26; j++) {
                if (structList[j].letter == toupper(lineBuff[0])) {
                  structList[j].count++;
                  break;
                }
              }
            }
            fclose (tp);
          }
          write(p[pipeWriteIndex], structList, 26*sizeof(dict));

          free(fileNameBuffer);
          fclose(fp);
          int returnStatus;
          waitpid(mappedProcs[i], &returnStatus, 0);
          exit(1);
        } else {
          i++;
          mapperIndex++;
          pipeWriteIndex += 2;
          continue;
        }
    }
    // Close write end of pipes
    for (int i = 1; i < mapperTotal*2; i+=2) {
      close(p[i]);
    }
    // For whatever reason, wait()/waitpid() doesn't work
    sleep(.5); //THIS IS TEMPORARY--or maybe permenant...idk
  }
  //This is the reducer process
  else {
    // Close write end of pipes
    for (int i = 1; i < mapperTotal*2; i+=2) {
      close(p[i]);
    }
    // return value for reducer process
    return 1;
  }
  // return value for master process
  return 0;
}
