#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <zconf.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "../include/protocol.h"

FILE *logfp;
int buffer[28]; // request size = 1D array with 28 entries

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

  	// create TCP socket
    int sockfd = socket(AF_INET , SOCK_STREAM , 0);

    // specify address to connect to
    struct sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_port = 4061;
		address.sin_addr.s_addr = INADDR_ANY;

    // connect
    if (connect(sockfd, (struct sockaddr *) &address, sizeof(address)) == 0) {
    // do the thing
      pid_t master = fork(); // create master process
      if (getpid(master) == -1 ) { // if we're in the first child process begin spawning other processes
        for(int i = 0; i < argv[2]; i++) {
          fork()

        }

        }
      }
    }

    // create log file
    createLogFile();

    // phase1 - File Path Partitioning
    traverseFS(mappers, folderName);

    // Phase2 - Mapper Clients's Deterministic Request Handling
    write(sockfd, buffer, strlen(buffer));
  //  if read(sockfd, buffer, strlen(buffer));
    // Phase3 - Master Client's Dynamic Request Handling (Extra Credit)


    fclose(logfp);
    return 0;

}
