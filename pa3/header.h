/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

/*
header.h, header for all source files
it will:
- structure definition
- global variable, lock declaration (extern)
- function declarations
*/

#ifndef _HEADER_H_
#define _HEADER_H_
#include <pthread.h>
#include <ctype.h>


// Structure definitions
typedef struct dictionary {
	char letter;
  int count;
} dict;

struct Node {
  char line[1024];
  struct Node* next;
};

struct Queue {
  struct Node *front, *rear;
};

struct condBuffer {
 char * filename;
 int logFlag;
 int num_items;
	struct Queue* queue;
	pthread_cond_t* cond;
	pthread_mutex_t* mutex;
};

// global variables, lock declerations
dict masterList[26];

// function declerations
struct Node* newNode(char * line);

struct Queue* createQueue();

void add(struct Queue* q, char* line);

struct Node* extract(struct Queue* q);

void condConsumer(void* arg);

void condProducer(void* arg);



#endif
