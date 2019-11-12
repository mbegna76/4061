/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

void condProducer(void* arg) {

	// // Random delay. DO NOT REMOVE!
	// usleep(rand() % 1000);
 //
	struct condBuffer* cq = (struct condBuffer*) arg;
 //
 //
	// // Add an element to the buffer.
	// pthread_mutex_lock(cq->mutex);
	// insert(cq->q, in);
	// cq->num_items++;
	// pthread_cond_signal(cq->cond);
	// pthread_mutex_unlock(cq->mutex);

}
