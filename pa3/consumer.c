/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Moti Begna , Wally Lu
* x500: begna002 , luxx0460 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

void condConsumer(void* arg) {
  struct condBuffer* cq = (struct condBuffer*) arg;
  int consumerId = consumerIdInc;

  while (cq->EOFFLAG != 1) {
    //CS START
    pthread_mutex_lock(cq->mutex);
    while(cq->num_items == 0 &&  cq->EOFFLAG != 1) {
      pthread_cond_wait(cq->cond, cq->mutex);
    }
    if (cq->EOFFLAG == 1) {
      pthread_mutex_unlock(cq->mutex);
      break;
    }

    struct Node *package = extract(cq->queue);

    if(!strncmp(package->line, "Balls", 5)) {
      cq->EOFFLAG = 1;
      pthread_cond_broadcast(cq->cond);
      free(package);
      pthread_mutex_unlock(cq->mutex);
      break;
    } else { //Parsing
      if (cq->logFlag == 1) {
        char *fileNameBuffer = (char*)malloc(60*sizeof(char));
        sprintf(fileNameBuffer, "consumer %d: %d\n", consumerId, package->lineNum);
        fprintf(cq->fp, fileNameBuffer);
        free(fileNameBuffer);
      }
      int front = 1; //flag for first letter in word, 1 means the next character
      // we find is a first letter
      int lineLength = strlen((package->line));
      for(int i = 0; i < lineLength; i++) {
        int j = 0;
        for (j; j < 26; j++) {
          if (masterList[j].letter == toupper(package->line[i])) {
            if (front == 1) {
              masterList[j].count++;
              front = 0;
            }
            break;
          }
        }
        if (j == 26) { //This means that the character isn't a letter
          if (package->line[i] != NULL){
            // printf("%c : %ld\n", package->line[i], pthread_self());
            front = 1;
          }
        }
      }
    }
    cq->num_items--;
    free(package);
    pthread_mutex_unlock(cq->mutex);

    //CS END
  }

	// // // Random delay. DO NOT REMOVE!
	// // usleep(rand() % 1000);
 // //
	// struct condBuffer* cq = (struct condBuffer*) arg;
 // //
	// // // Remove an element from the buffer.
 //  while(cq->EOFFLAG != 1) {
 //    pthread_mutex_lock(cq->mutex);
 //    while(cq->num_items == 0) {
 //      pthread_cond_wait(cq->cond, cq->mutex);
 //    }
 //    //GRAB FROM QUEUE and do stuff
 //    cq->num_items--;
 //    pthread_mutex_unlock(cq->mutex);
 //  }

}
