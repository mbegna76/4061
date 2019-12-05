#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "../include/protocol.h"
#include "phase1.c"

FILE *logfp;
int buffer[28]; // request size = 1D array with 28 entries

char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

typedef struct dictionary {
	char letter;
  int count;
} dict;

void createLogFile(void) {
    pid_t p = fork();
    if (p==0)
        execl("/bin/rm", "rm", "-rf", "log", NULL);

    wait(NULL);
    mkdir("log", ACCESSPERMS);
    logfp = fopen("log/log_client.txt", "w");
}

int main(int argc, char *argv[]) {

  int mappers;
  char folderName[100] = {'\0'};
  char *server_ip;
  int server_port;
  struct sockaddr_in servaddr;
  int connfd, len;

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
       printf("Runing mapper %d\n", i+1);
       FILE * fp;
       char *fileNameBuffer = (char*)malloc(pathSize * sizeof(char));
       sprintf(fileNameBuffer, "./MapperInput/Mapper_%d.txt", mapperIndex);
       fp = fopen (fileNameBuffer,"r");
       char fileBuff[255];
       char lineBuff[255];
       int request[28];
       for(int m = 0; m < 26; m++) {
         request[m] = 0;
       }

       // create TCP socket
       int sockfd = socket(AF_INET , SOCK_STREAM , 0);
       // specify address to connect to
       struct sockaddr_in address;
       address.sin_family = AF_INET;
       address.sin_port = htons(server_port);
       address.sin_addr.s_addr = inet_addr(server_ip);

       if (connect(sockfd, (struct sockaddr *) &address, sizeof(address)) == 0) {

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
           fclose (tp);
         }
         //DO THE CONNECTIONS AND SENDING IN HERE -> Structlist contains the count, but we can chnge how we send the values
         //REQUEST NOW CONTAINS THE DATA TO SEND
         printf("A: %d\n", request[2]);

         write(sockfd, request, strlen(request));
         close(sockfd);

       } else {
         		perror("Connection failed!");
         }


       free(fileNameBuffer);
       fclose(fp);
       int returnStatus;
       waitpid(mappedProcs[i], &returnStatus, 0);
       exit(1);
     }  else {
          i++;
          mapperIndex++;
          continue;
        }
    }





    fclose(logfp);
    return 0;
}
