/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

void condConsumer(void* arg) {
  struct condBuffer* cq = (struct condBuffer*) arg;
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
      pthread_mutex_unlock(cq->mutex);
      break;
    } else { //Parsing
      int front = 1; //flag for first letter in word, 1 means the next character
      // we find is a first letter
      for(int i = 0; i < 100; i++) {
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
            printf("%c : %ld\n", package->line[i], pthread_self());
            front = 1;
          }
        }
      }
    }
    cq->num_items--;
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
