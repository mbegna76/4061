#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/types.h>
  #include <sys/wait.h>
#include "../include/protocol.h"
#include "phase1.c"

FILE *logfp;
int request[REQUEST_MSG_SIZE]; // request size = 1D array with 28 entries

char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

void createLogFile(void) {
    pid_t p = fork();
    if (p==0)
        execl("/bin/rm", "rm", "-rf", "log", NULL);

    wait(NULL);
    mkdir("log", ACCESSPERMS);
    logfp = fopen("log/log_client.txt", "w");
}


int* serverConnectionAndRespone(int server_port, char* server_ip, int request[]) {
  // create TCP socket
  int sockfd = socket(AF_INET , SOCK_STREAM , 0);
  // specify address to connect to
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = htons(server_port);
  address.sin_addr.s_addr = inet_addr(server_ip);
  static int readbuf[28];


  if (connect(sockfd, (struct sockaddr *) &address, sizeof(address)) == 0) {

    write(sockfd, request, REQUEST_MSG_SIZE*4);
    read(sockfd, readbuf, REQUEST_MSG_SIZE*4);

    close(sockfd);
  }
  else {
     perror("Connection failed!");
  }
  return readbuf;

}

int main(int argc, char *argv[]) {

  int mappers;
  char folderName[100] = {'\0'};
  char *server_ip;
  int server_port;

  if (argc == 5) { // 4 arguments
      strcpy(folderName, argv[1]);
      mappers = atoi(argv[2]);
      server_ip = argv[3];
      server_port = atoi(argv[4]);
      if (mappers > MAX_MAPPER_PER_MASTER) {
          printf("Maximum number of mappers is %d.\n", MAX_MAPPER_PER_MASTER);
          printf("./client <Folder Name> <# of mappers> <server IP> <server Port>\n");
          exit(1);
      }
  }
  else {
      printf("Invalid or less number of arguments provided\n");
      printf("./client <Folder Name> <# of mappers> <server IP> <server Port>\n");
      exit(1);
  }

  // create log file
  createLogFile();

  // phase1 - File Path Partitioning
  traverseFS(mappers, folderName);

  // Phase2 - Mapper Clients's Deterministic Request Handling
  pid_t mappedProcs[mappers]; // create mapper processes

  int i = 0;
  int mapperIndex = i+1;
  int pathSize;
  while (i < mappers) {
     mappedProcs[i] = fork();
     if (mappedProcs[i] > 0) {
       if (i < 10) {
         pathSize = 26;
       } else {
         pathSize = 27;
       }

       FILE * fp;
       char *fileNameBuffer = (char*)malloc(pathSize * sizeof(char));
       sprintf(fileNameBuffer, "./MapperInput/Mapper_%d.txt", mapperIndex);
       fp = fopen (fileNameBuffer,"r");
       char fileBuff[255];
       char lineBuff[255];
       int* response;
       int numMessagesSent = 0;
       char *logOutput;

       // open connection
       logOutput = (char*)malloc(sizeof(char)*30);
       sprintf(logOutput, "[%d] open connection\n",  mapperIndex);
       fputs(logOutput, logfp);
       free(logOutput);


       // CHECK-IN
       //Reset request values
       for(int m = 0; m < REQUEST_MSG_SIZE; m++) {
         request[m] = 0;
       }
       request[RQS_COMMAND_ID] = CHECKIN;
       request[RQS_MAPPER_PID] = mapperIndex;
       response = serverConnectionAndRespone(server_port, server_ip, request);
       logOutput = (char*)malloc(sizeof(char)*30);
       sprintf(logOutput, "[%d] CHECKIN: %d %d\n", mapperIndex, response[RSP_CODE], response[RSP_DATA]);
       fputs(logOutput, logfp);
       free(logOutput);


       //Reset request values
       for(int m = 0; m < REQUEST_MSG_SIZE; m++) {
         request[m] = 0;
       }

       // Loops through the .txt files in the mapper file
       while (fscanf(fp, "%s", fileBuff) != -1){
         FILE * tp;
         tp = fopen(fileBuff, "r");
         // Loops through the lines in a .txt file
         while (fscanf(tp, "%s", lineBuff) != -1){
           for (int j = 0; j < 26; j++) {
             if (toupper(alphabet[j]) == toupper(lineBuff[0])) {
               request[j+2] =  request[j+2] + 1;
               break;
             }
           }
         }

         //SEND UPDATE TO SERVER
         request[RQS_COMMAND_ID] = UPDATE_AZLIST;
         request[RQS_MAPPER_PID] = mapperIndex;
         response = serverConnectionAndRespone(server_port, server_ip, request);
         numMessagesSent++;
         //Reset request values
         for (int i = 0; i < REQUEST_MSG_SIZE; i++) {
           request[i] = 0;
         }

         fclose (tp);
       }

       logOutput = (char*)malloc(sizeof(char)*30);
       sprintf(logOutput, "[%d] UPDATE_AZLIST: %d\n", mapperIndex, numMessagesSent);
       fputs(logOutput, logfp);
       free(logOutput);

       //GET getAZList
       request[RQS_COMMAND_ID] = GET_AZLIST;
       request[RQS_MAPPER_PID] = mapperIndex;
       response = serverConnectionAndRespone(server_port, server_ip, request);
       logOutput = (char*)malloc(sizeof(char)*20);
       sprintf(logOutput, "[%d] GET_AZLIST: %d ", mapperIndex,  response[RSP_CODE]);
       fputs(logOutput, logfp);
       free(logOutput);

       for(int i = 2; i < REQUEST_MSG_SIZE; i++) {

         logOutput = (char*)malloc(sizeof(char)*10);
         sprintf(logOutput, "%d ", response[i]);
         fputs(logOutput, logfp);
         free(logOutput);
       }
       logOutput = (char*)malloc(sizeof(char)*2);
       sprintf(logOutput, " \n");
       fputs(logOutput, logfp);
       free(logOutput);

       //GET Mapper Updates
       //Reset request values
       for(int m = 0; m < REQUEST_MSG_SIZE; m++) {
         request[m] = 0;
       }
       request[RQS_COMMAND_ID] = GET_MAPPER_UPDATES;
       request[RQS_MAPPER_PID] = mapperIndex;
       response = serverConnectionAndRespone(server_port, server_ip, request);
       logOutput = (char*)malloc(sizeof(char)*50);
       sprintf(logOutput, "[%d] GET_MAPPER_UPDATES: %d %d\n", mapperIndex, response[RSP_CODE], response[RSP_DATA]);
       fputs(logOutput, logfp);
       free(logOutput);

       //GET ALL Updates
       //Reset request values
       for(int m = 0; m < REQUEST_MSG_SIZE; m++) {
         request[m] = 0;
       }
       request[RQS_COMMAND_ID] = GET_ALL_UPDATES;
       request[RQS_MAPPER_PID] = mapperIndex;
       response = serverConnectionAndRespone(server_port, server_ip, request);
       logOutput = (char*)malloc(sizeof(char)*50);
       sprintf(logOutput, "[%d] GET_ALL_UPDATES: %d %d\n", mapperIndex, response[RSP_CODE], response[RSP_DATA]);
       fputs(logOutput, logfp);
       free(logOutput);


       // CHECK-OUT
       //Reset request values
       for(int m = 0; m < REQUEST_MSG_SIZE; m++) {
         request[m] = 0;
       }
       request[RQS_COMMAND_ID] = CHECKOUT;
       request[RQS_MAPPER_PID] = mapperIndex;
       response = serverConnectionAndRespone(server_port, server_ip, request);
       logOutput = (char*)malloc(sizeof(char)*30);
       sprintf(logOutput, "[%d] CHECKOUT: %d %d\n", mapperIndex, response[RSP_CODE], response[RSP_DATA]);
       fputs(logOutput, logfp);
       free(logOutput);

       // close connection
       logOutput = (char*)malloc(sizeof(char)*30);
       sprintf(logOutput, "[%d] close connection\n",  mapperIndex);
       fputs(logOutput, logfp);
       free(logOutput);

       free(fileNameBuffer);
       fclose(fp);
       exit(0);
     }

     else {
          i++;
          mapperIndex++;
          continue;
        }
    }


    fclose(logfp);
    return 0;
}
