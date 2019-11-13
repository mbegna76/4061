/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

void condProducer(void* arg) {
 struct condBuffer* cq = (struct condBuffer*) arg;

   if (cq->logFlag == 1) { //Producer Launched
     char *fileNameBuffer = (char*)malloc(10*sizeof(char));
     sprintf(fileNameBuffer, "producer\n");
     fprintf(cq->fp, fileNameBuffer);
     free(fileNameBuffer);
   }

 FILE * fp;
 fp = fopen (cq->filename,"r");
 char fileBuff[1024];
 int lineNum = 0;
 while (fgets(fileBuff, 1024, (FILE*) fp)){
   //CS START
   if (cq->logFlag == 1) {
     char *fileNameBuffer = (char*)malloc(20*sizeof(char));
     sprintf(fileNameBuffer, "producer %d\n", lineNum);
     fprintf(cq->fp, fileNameBuffer);
     free(fileNameBuffer);
   }
   pthread_mutex_lock(cq->mutex);
   add(cq->queue, fileBuff, lineNum);
   cq->num_items++;
   lineNum++;
   pthread_cond_broadcast(cq->cond);
   pthread_mutex_unlock(cq->mutex);
   //CS END
 }
 if (cq->logFlag == 1) {
   char *fileNameBuffer = (char*)malloc(4*sizeof(char));
   sprintf(fileNameBuffer, "-1\n", lineNum);
   fprintf(cq->fp, fileNameBuffer);
   free(fileNameBuffer);
 }
 add(cq->queue, "Balls", lineNum);
 cq->num_items++;


}
