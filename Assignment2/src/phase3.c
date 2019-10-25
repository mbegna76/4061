#include "./../include/phase3.h"
#include "./../include/phase2.h"
// You are free to use your own logic. The following points are just for getting started
/* Reduce Function
	1)	The reducer receives the list from 'm' mappers via pipes
	2)	Ensure proper closing of pipe ends
	3) 	Combine the lists to create a single list
	4) 	Write the list to "ReducerResult.txt" in the current folder
*/
void processPipe(int p[], int mapperTotal){

  dict masterList[26];
  int nbytes;
  nbytes = read(p[0], masterList, 26*sizeof(dict));

  for(int i = 2; i < mapperTotal*2; i+=2) {
    dict mapperList[26];
    nbytes = read(p[i], mapperList, 26*sizeof(dict));
    for (int j = 0; j < 26; j++){
      masterList[j].count = masterList[j].count + mapperList[j].count;
    }
  }
  FILE * fp;
  fp = fopen ("./ReducerResult.txt","w+");
  for (int i = 0; i < 26; i++){
    char *fileNameBuffer = (char*)malloc(sizeof(dict));
    sprintf(fileNameBuffer, "%c %d\n", masterList[i].letter, masterList[i].count);
    fprintf(fp, fileNameBuffer);
    free(fileNameBuffer);
  }
  fclose(fp);


  exit(1);
}
