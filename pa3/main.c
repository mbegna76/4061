/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int main(int argc, char** argv) {

 // Initializing condBuffer
 struct condBuffer* cq = (struct condBuffer*) malloc(sizeof(struct condBuffer));
 cq->filename = argv[2];
 cq->num_items = 0;
 cq->queue = createQueue();
	cq->cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
	cq->mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
	pthread_cond_init(cq->cond, NULL);
	pthread_mutex_init(cq->mutex, NULL);

 return 0;
}
