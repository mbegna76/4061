/*test machine: CSELAB_machine_name * date: 11/13/19
* name: Moti Begna , Wally Lu
* x500: begna002 , luxx0460 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
// pthread.h included in header.h

// implement shared queue, final histogram here..

char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

void initializeDict() {
	for (int j = 0; j < 26; j++) {
			masterList[j].letter = toupper(alphabet[j]);
			masterList[j].count = 0;
	}
}

// A utility function to create a new linked list node.
struct Node* newNode(char * line, int lineNum)
{
  struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
  int len = sizeof(line)/sizeof(char);//Method
  for (int i = 0; i < 1024; i++) {
    temp->line[i] = line[i];
  }
  temp->next = NULL;
	temp->lineNum = lineNum;
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
void add(struct Queue* q, char* line, int lineNum)
{
  // Create a new LL node
  struct Node* temp = newNode(line, lineNum);

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

  q->front = q->front->next;

  // If front becomes NULL, then change rear also as NULL
  if (q->front == NULL)
      q->rear = NULL;
  return temp;
}
