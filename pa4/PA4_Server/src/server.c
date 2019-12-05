#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <zconf.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>
#include "../include/protocol.h"

#define MAX_MSG_SIZE 1000

int currentConn = 0;

struct threadArg {
	int clientfd;
	char * clientip;
	int clientport;
};

struct tableEntry{
  int mapperID;
  int numUpdates;
  int checkIO;
};

int azList[26]; // keep track of letter counts here
struct tableEntry updateStatus[26]; // updateStatus table

void * threadFunction(void * arg) {
	struct threadArg * tArg = (struct threadArg *) arg;
	char readbuf[28];

	read(tArg->clientfd, readbuf, MAX_MSG_SIZE);
	printf("%s\n", readbuf);
	// write(tArg->clientfd, (void *) "Acknowledge", 12);
	// close(tArg->clientfd);
	// free(tArg);
	// pthread_mutex_lock(&currentConn_lock);
	// currentConn--;
	// pthread_mutex_unlock(&currentConn_lock);
	return NULL;
}

int main(int argc, char *argv[]) {

    int server_port;
    int count;
    pthread_t threads[MAX_CONCURRENT_CLIENTS];

    if (argc == 2) { // 1 arguments
        server_port = atoi(argv[1]);
    }
    else {
        printf("Invalid or less number of arguments provided\n");
        printf("./server <server Port>\n");
        exit(0);
    }

    // socket create and verification
    int sockfd = socket(AF_INET , SOCK_STREAM , 0);

    // Filling server information
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(server_port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    // Server (Reducer) code
    listen(sockfd, MAX_CONCURRENT_CLIENTS);

    while(1) {

      struct sockaddr_in clientAddress;
		  socklen_t size = sizeof(struct sockaddr_in);
      int clientfd = accept(sockfd, (struct sockaddr*) &clientAddress, &size);

      struct threadArg *arg = (struct threadArg *) malloc(sizeof(struct threadArg));
      arg->clientfd = clientfd;
      arg->clientip = inet_ntoa(clientAddress.sin_addr);
      arg->clientport = clientAddress.sin_port;

      if(currentConn == MAX_CONCURRENT_CLIENTS) {
        printf("Server is too busy\n");
        close(clientfd);
        free(arg);
        continue;
      }
      else {
				printf("CONNECTION ESTABLISHED\n");
				// pthread_create(&threads[currentConn], NULL, threadFunction, (void*) arg);
    //  #pthread_create(&threads[count], , , (void*) arg);
        count++;
      }

  }
	return 0;
}
