/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Moti Begna , Wally Lu
* x500: begna002 , luxx0460 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

void finalResult() {
  FILE * fp;
  fp = fopen ("result.txt","w+");
  for (int i = 0; i < 26; i++) {
    char *fileNameBuffer = (char*)malloc(sizeof(dict));
    sprintf(fileNameBuffer, "%c %d\n", masterList[i].letter, masterList[i].count);
    fprintf(fp, fileNameBuffer);
    free(fileNameBuffer);
  //  printf("%c: %d\n", masterList[i].letter, masterList[i].count);
  }
  fclose(fp);
}

int main(int argc, char** argv) {
  // Initializing condBuffer
  initializeDict();
  struct condBuffer* cq = (struct condBuffer*) malloc(sizeof(struct  condBuffer));
  cq->filename = argv[2];
  cq->num_items = 0;
  cq->EOFFLAG = 0;
  cq->queue = createQueue();
  cq->cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
	cq->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	pthread_cond_init(cq->cond, NULL);
	pthread_mutex_init(cq->mutex, NULL);

  int conCount = atoi(argv[1]);
  c_thread = (pthread_t*) malloc(conCount*sizeof(pthread_t));


  if (argc < 3 ) { // passes through too little arguments
    printf("Too little arguments given\n");
    exit(1);
  }
  if (argc == 3 && argv[1] <= 0) { // passes through standard arguments
    cq->logFlag = 0;
    // do the thing
  }
  if (argc == 4 ) { // passes through standard arguments + flag
    if (!strncmp(argv[3], "-p", 2)) { //
      cq->logFlag = 1;
      cq->fp = fopen ("log.txt","w+");
    }
    else {
      printf("Please use a valid option flag\n");
      exit(1);
    }
  }
  if (argc >= 5) { // passes through too many arguments
    printf("Too many arguments given\n");
    exit(1);
  }

  pthread_create(&p_thread, NULL, condProducer, (void*) cq);
  for (int i=0; i < conCount; i++) {
    if (cq->logFlag == 1) {
      char *fileNameBuffer = (char*)malloc(60*sizeof(char));
      sprintf(fileNameBuffer, "consumer %d\n", i);
      fprintf(cq->fp, fileNameBuffer);
      free(fileNameBuffer);
    }
    consumerIdInc = i;
		pthread_create(&c_thread[i], NULL, condConsumer, (void*) cq);
	}

  pthread_join(p_thread, NULL);
  for (int i = 0; i < conCount; i++) {
    pthread_join(c_thread[i], NULL);
  }
  free(cq->queue);
  finalResult();
  if (cq->logFlag == 1) {
    fclose(cq->fp);
  }


 return 0;
}
