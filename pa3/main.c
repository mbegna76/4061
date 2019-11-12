/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int main(int argc, char** argv) {

  // Initializing condBuffer
  initializeDict();
  struct condBuffer* cq = (struct condBuffer*) malloc(sizeof(struct  condBuffer));
  cq->filename = argv[2];
  cq->num_items = 0;
  cq->queue = createQueue();
  cq->cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
	cq->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	pthread_cond_init(cq->cond, NULL);
	pthread_mutex_init(cq->mutex, NULL);

  if (argc < 3 ) { // passes through too little arguments
    printf("Too little arguments given");
  }
  if (argc == 3 && argv[1] <= 0) { // passes through standard arguments
    cq->logFlag = 0;
    // do the thing 
  }
  if (argc == 4 ) { // passes through standard arguments + flag
    if (strncmp(argv[3], "-p", 2)) { // 
      cq->logFlag = 1; 
    }
    else {
      printf("Please use a valid option flag");
      exit(1);
    }
  }
  if (argc >= 5) { // passes through too many arguments 
    printf("Too many arguments given");
    exit(0);
  }

 return 0;
}



