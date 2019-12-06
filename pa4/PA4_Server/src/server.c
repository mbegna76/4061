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
int count = 0; // help us find spots for a new thread he's like a valet
pthread_mutex_t currentConn_lock;
pthread_mutex_t updateLock;


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
struct tableEntry updateStatus[33]; // updateStatus table

int* checkIn(int request[]) {
	int mapperId = request[1];
	printf("[%d] CHECKIN\n", mapperId);
	//Looking for previous mapper
	if (updateStatus[mapperId].mapperID != 0) {
			updateStatus[mapperId].checkIO = 1;
	} else { 	//Adding new mapper
		updateStatus[mapperId].mapperID = mapperId;
		updateStatus[mapperId].checkIO = 1;
	}
	static int returnBuf[28];
	returnBuf[0] = 1;
	returnBuf[1] = 0;
	returnBuf[2] = mapperId;
	return returnBuf;

}
int * updateAZList(int request[]) {
	int mapperId = request[1];

	int i, j;
	//Update azLlist
	for(i = 0, j = 2; i < 26; i++, j++) {
		azList[i] = azList[i] + request[j];
	}
	//increment updateStatus numUpdates
	updateStatus[mapperId].numUpdates = updateStatus[mapperId].numUpdates + 1;

	static int returnBuf[28];
	returnBuf[0] = 1;
	returnBuf[1] = 0;
	returnBuf[2] = mapperId;
	return returnBuf;
	}

int * getAZList(int request[]) {
	int mapperId = request[1];
	printf("[%d] GET_AZLIST\n", mapperId);
	static int returnBuf[28];
	returnBuf[0] = 3;
	returnBuf[1] = 0;
	int i, j;
	for(i = 0, j = 2; i < 26; i++, j++) {
		returnBuf[j] = azList[i];
	}
	return returnBuf;
}
int * getMapperUpdates(int request[]) {
	int mapperId = request[1];
	printf("[%d] GET_MAPPER_UPDATES\n", mapperId);
	static int returnBuf[28];
	returnBuf[0] = 4;
	returnBuf[1] = 0;
	returnBuf[2] = updateStatus[mapperId].numUpdates;
	return returnBuf;
}
int * getAllUpdates(int request[]) {
	printf("[%d] GET_ALL_UPDATES\n", request[1]);
	static int returnBuf[28];
	returnBuf[0] = 5;
	returnBuf[1] = 0;
	returnBuf[2] = 0;
	for(int i = 0; i < 33; i++){
		returnBuf[2] = returnBuf[2] + updateStatus[i].numUpdates;
	}
	return returnBuf;

}
int * checkOut(int request[]) {
	int mapperId = request[1];
	printf("[%d] CHECKOUT\n", mapperId);
	//checkOut
	updateStatus[mapperId].numUpdates;

	static int returnBuf[28];
	returnBuf[0] = 1;
	returnBuf[1] = 0;
	returnBuf[2] = mapperId;
	return returnBuf;
}

void * threadFunction(void * arg) {
	struct threadArg * tArg = (struct threadArg *) arg;
	int readbuf[28];
	int *returnBuf;


	read(tArg->clientfd, readbuf, MAX_MSG_SIZE);
	pthread_mutex_lock(&updateLock);
	switch(readbuf[0]) {
		case 1:
			returnBuf = checkIn(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
			break;
		case 2:
			returnBuf = updateAZList(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
			break;
		case 3:
			returnBuf = getAZList(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
			break;
		case 4:
			returnBuf = getMapperUpdates(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
			break;
		case 5:
			returnBuf = getAllUpdates(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
			break;
		case 6:
			returnBuf = checkOut(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
		default:
			break;

	}
	pthread_mutex_unlock(&updateLock);
	close(tArg->clientfd);
	free(tArg);
	pthread_mutex_lock(&currentConn_lock);
	currentConn--;
	pthread_mutex_unlock(&currentConn_lock);
	printf("close connection from %s:%d\n", tArg->clientip, tArg->clientport);
	return NULL;
}

int main(int argc, char *argv[]) {

    int server_port;
    int count;
    pthread_t threads[MAX_CONCURRENT_CLIENTS];
    pthread_mutex_init(&currentConn_lock, NULL);
		pthread_mutex_init(&updateLock, NULL);


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
		printf("server is listening\n");
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
        count = 0;
        close(clientfd);
        free(arg);
        continue;
      }
      else {
				printf("open connection from %s:%d\n", arg->clientip, arg->clientport);

        pthread_create(&threads[count], NULL, threadFunction, (void*) arg);
				count++;
        pthread_mutex_lock(&currentConn_lock);
        currentConn++;
        pthread_mutex_unlock(&currentConn_lock);
      }

  }
	return 0;
}
