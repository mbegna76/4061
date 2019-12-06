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

int azList[ALPHABETSIZE]; // keep track of letter counts here
struct tableEntry updateStatus[MAX_STATUS_TABLE_LINES]; // updateStatus table

int* checkIn(int request[]) {
	int mapperId = request[RQS_MAPPER_PID];
	printf("[%d] CHECKIN\n", mapperId);
	static int returnBuf[LONG_RESPONSE_MSG_SIZE];
	//Looking for previous mapper
	if (updateStatus[mapperId].mapperID != 0) {
			//ERROR: Mapper Client Already Checked-In
			if (updateStatus[mapperId].checkIO == 1) {
				returnBuf[RSP_COMMAND_ID] = CHECKIN;
				returnBuf[RSP_CODE] = RSP_NOK;
				for(int i = 2; i < LONG_RESPONSE_MSG_SIZE; i++) {
					returnBuf[i] = -1;
				}
				printf("[%d] Mapper Client Already Checked-In\n", mapperId);
				return returnBuf;
			}
			updateStatus[mapperId].checkIO = 1;
	} else { 	//Adding new mapper
		updateStatus[mapperId].mapperID = mapperId;
		updateStatus[mapperId].checkIO = 1;
	}
	returnBuf[RSP_COMMAND_ID] = CHECKIN;
	returnBuf[RSP_CODE] = RSP_OK;
	returnBuf[RQS_DATA] = mapperId;
	return returnBuf;

}
int * updateAZList(int request[]) {
	int mapperId = request[RQS_MAPPER_PID];

	int i, j;
	//Update azLlist
	for(i = 0, j = 2; i < ALPHABETSIZE; i++, j++) {
		azList[i] = azList[i] + request[j];
	}
	//increment updateStatus numUpdates
	updateStatus[mapperId].numUpdates = updateStatus[mapperId].numUpdates + 1;

	static int returnBuf[LONG_RESPONSE_MSG_SIZE];
	returnBuf[RSP_COMMAND_ID] = UPDATE_AZLIST;
	returnBuf[RSP_CODE] = RSP_OK;
	returnBuf[RQS_DATA] = mapperId;
	return returnBuf;
	}

int * getAZList(int request[]) {
	int mapperId = request[RQS_MAPPER_PID];
	printf("[%d] GET_AZLIST\n", mapperId);
	static int returnBuf[LONG_RESPONSE_MSG_SIZE];
	returnBuf[RSP_COMMAND_ID] = GET_AZLIST;
	returnBuf[RSP_CODE] = RSP_OK;
	int i, j;
	for(i = 0, j = 2; i < ALPHABETSIZE; i++, j++) {
		returnBuf[j] = azList[i];
	}
	return returnBuf;
}
int * getMapperUpdates(int request[]) {
	int mapperId = request[RQS_MAPPER_PID];
	printf("[%d] GET_MAPPER_UPDATES\n", mapperId);
	static int returnBuf[LONG_RESPONSE_MSG_SIZE];

	if (updateStatus[mapperId].mapperID > 0) {
		returnBuf[RSP_COMMAND_ID] = GET_MAPPER_UPDATES;
		returnBuf[RSP_CODE] = RSP_OK;
		returnBuf[RQS_DATA] = updateStatus[mapperId].numUpdates;
		return returnBuf;
	} else { //ERROR: Mapper Not in updateStatus Table
		returnBuf[RSP_COMMAND_ID] = GET_MAPPER_UPDATES;
		returnBuf[RSP_CODE] = RSP_NOK;
		for(int i = 2; i < LONG_RESPONSE_MSG_SIZE; i++) {
			returnBuf[i] = -1;
		}
		printf("[%d] Mapper Not in updateStatus Table\n", mapperId);
		return returnBuf;
	}
}
int * getAllUpdates(int request[]) {
	int mapperId = request[RQS_MAPPER_PID];
	printf("[%d] GET_ALL_UPDATES\n", mapperId);
	static int returnBuf[LONG_RESPONSE_MSG_SIZE];

	if (updateStatus[mapperId].mapperID > 0) {
		returnBuf[RSP_COMMAND_ID] = GET_ALL_UPDATES;
		returnBuf[RSP_CODE] = RSP_OK;
		returnBuf[RQS_DATA] = 0;
		for(int i = 0; i < MAX_STATUS_TABLE_LINES; i++){
			returnBuf[RQS_DATA] = returnBuf[RQS_DATA] + updateStatus[i].numUpdates;
		}
		return returnBuf;
	} else { //ERROR: Mapper Not in updateStatus Table
		returnBuf[RSP_COMMAND_ID] = GET_ALL_UPDATES;
		returnBuf[RSP_CODE] = RSP_NOK;
		for(int i = 2; i < LONG_RESPONSE_MSG_SIZE; i++) {
			returnBuf[i] = -1;
		}
		printf("[%d] Mapper Not in updateStatus Table\n", mapperId);
		return returnBuf;
	}

}
int * checkOut(int request[]) {
	int mapperId = request[RQS_MAPPER_PID];
	printf("[%d] CHECKOUT\n", mapperId);
	static int returnBuf[LONG_RESPONSE_MSG_SIZE];
	if (updateStatus[mapperId].checkIO == 1) {
		updateStatus[mapperId].checkIO = 0;
		returnBuf[RSP_COMMAND_ID] = CHECKOUT;
		returnBuf[RSP_CODE] = RSP_OK;
		returnBuf[RQS_DATA] = mapperId;
		return returnBuf;
	} else {//ERROR: Mapper Client is not Checked-In
		returnBuf[RSP_COMMAND_ID] = CHECKOUT;
		returnBuf[RSP_CODE] = RSP_NOK;
		for(int i = 2; i < LONG_RESPONSE_MSG_SIZE; i++) {
			returnBuf[i] = -1;
		}
		printf("[%d] Mapper Client is not Checked-In\n", mapperId);
		return returnBuf;

	}


}

void * threadFunction(void * arg) {
	struct threadArg * tArg = (struct threadArg *) arg;
	int readbuf[REQUEST_MSG_SIZE];
	int *returnBuf;


	read(tArg->clientfd, readbuf, MAX_MSG_SIZE);
	pthread_mutex_lock(&updateLock);

	//ERROR: Invalid Mapper ID
	if (readbuf[RQS_MAPPER_PID] <= 0) {
		returnBuf[RSP_COMMAND_ID] = readbuf[RQS_COMMAND_ID];
		returnBuf[RSP_CODE] = RSP_NOK;
		for(int i = 2; i < LONG_RESPONSE_MSG_SIZE; i++) {
			returnBuf[i] = -1;
		}
		printf("[%d] Invalid Mapper ID\n", readbuf[RQS_MAPPER_PID]);
		write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
	}

	switch(readbuf[RQS_COMMAND_ID]) {
		case CHECKIN:
			returnBuf = checkIn(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
			break;
		case UPDATE_AZLIST:
			returnBuf = updateAZList(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
			break;
		case GET_AZLIST:
			returnBuf = getAZList(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
			break;
		case GET_MAPPER_UPDATES:
			returnBuf = getMapperUpdates(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
			break;
		case GET_ALL_UPDATES:
			returnBuf = getAllUpdates(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
			break;
		case CHECKOUT:
			returnBuf = checkOut(readbuf);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
		default:
			//ERROR: Unsucessful Request
			returnBuf[RSP_COMMAND_ID] = readbuf[RQS_COMMAND_ID];
			returnBuf[RSP_CODE] = RSP_NOK;
			for(int i = 2; i < LONG_RESPONSE_MSG_SIZE; i++) {
				returnBuf[i] = -1;
			}
			printf("[%d] Unsucessful Request\n", readbuf[1]);
			write(tArg->clientfd, returnBuf, REQUEST_MSG_SIZE*4);
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
