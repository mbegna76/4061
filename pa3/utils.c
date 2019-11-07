/*test machine: CSELAB_machine_name * date: mm/dd/yy
* name: full_name1 , [full_name2]
* x500: id_for_first_name , [id_for_second_name] */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

// implement shared queue, final histogram here..

typedef struct dictionary {
	char letter;
  int count;
} dict;

dict masterList[26];


// A linked list (LL) node to store a queue entry
struct Node {
  char line[1024];
  struct Node* next;
};

// The queue, front stores the front node of LL and rear stores the
// last node of LL
struct Queue {
  struct Node *front, *rear;
};

// A utility function to create a new linked list node.
struct Node* newNode(char * line)
{
  struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
  int len = sizeof(line)/sizeof(char);//Method
  for (int i = 0; i < len; i++) {
    temp->line[i] = line[i];
  }
  temp->next = NULL;
  return temp;
}

// A utility function to create an empty queue
struct Queue* createQueue()
{
  struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
  q->front = q->rear = NULL;
  return q;
}

// The function to add a key k to q
void add(struct Queue* q, char* line)
{
  // Create a new LL node
  struct Node* temp = newNode(line);

  // If queue is empty, then new node is front and rear both
  if (q->rear == NULL) {
      q->front = q->rear = temp;
      return;
  }

  // Add the new node at the end of queue and change rear
  q->rear->next = temp;
  q->rear = temp;
}

// Function to remove a key from given queue q
struct Node* extract(struct Queue* q)
{
  // If queue is empty, return NULL.
  if (q->front == NULL)
      return NULL;

  // Store previous front and move front one node ahead
  struct Node* temp = q->front;
  free(temp);

  q->front = q->front->next;

  // If front becomes NULL, then change rear also as NULL
  if (q->front == NULL)
      q->rear = NULL;
  return temp;
}
