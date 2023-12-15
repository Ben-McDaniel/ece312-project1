// gcc tcpserver.c -o server -pthread
// ./server

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <sys/types.h> 
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <pthread.h>

  #define PORT 4567
  #define BUF_SIZE 256
  #define CLADDR_LEN 100
  int firstMessageReceived = 0;
  char OtherName[256];
  char YouUserName[256];
  int isFirstSentMessage = 0;

    void error(char *msg)
    {
        perror(msg);
        exit(1);
    }

    void * receiveMessage(void * socket) {
        int sockfd, ret;
        char buffer[BUF_SIZE];
        sockfd = (int) socket;

        memset(buffer, 0, BUF_SIZE);
        while(1) {
            memset(buffer, 0, BUF_SIZE);
            while ((ret = read(sockfd, buffer, BUF_SIZE)) > 0) {
                if(firstMessageReceived == 1 ) {
                    //I like minions
                    printf("\n%s: %s", OtherName, buffer);
                    memset(buffer, 0, BUF_SIZE);
                } else {
                    strcpy(OtherName, buffer);
                    // printf("\n%s\n", OtherName);
                    OtherName[strcspn(OtherName, "\n")] = 0;
                    firstMessageReceived = 1;
                    memset(buffer, 0, BUF_SIZE);
                }
                memset(buffer, 0, BUF_SIZE);
            }
        }
        if (ret < 0) 
            printf("Error receiving data!");
        else
            printf("Closing connection");
        close(sockfd);
    }

    void *sendMessage(void *socket) {
        int sockfd, ret;
        char buffer[BUF_SIZE];
        sockfd = (int) socket;
        char exitMessage[] = "exit\n";
        char closeConn[] = "Server Closing Connection";

        write(sockfd, YouUserName, strlen(YouUserName));

        while (1) {
            if(isFirstSentMessage == 0) {
                printf("What is it you wish to be called? ");
                bzero(buffer,256);
                fgets(buffer,255,stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                strcpy(YouUserName, buffer);
                printf("Hello, %s\n", YouUserName);
                write(sockfd,buffer,strlen(buffer));
                isFirstSentMessage = 1;
                bzero(buffer,256);
                memset(buffer, 0, BUF_SIZE);
            } else {
                printf("%s: ",YouUserName);
                memset(buffer, 0, BUF_SIZE);
                fgets(buffer, 255, stdin);
                if(!strcmp(buffer,exitMessage)){
                    write(sockfd, closeConn, strlen(closeConn));
                    printf("\nClosing Your connection\n");

                    //add delay to allow time for message to be written
                    int j = 0;
                    for(int i = 0; i < 10000; i++){
                        // for(int k = 0; k < 1000; k++) {
                        //     j = j - 1;
                        // }
                        j = j + i;
                    }
                    exit(0);
                }
                write(sockfd, buffer, strlen(buffer));
                buffer[strcspn(buffer, "\n")] = 0;
            }
        }
    }

    int main(int argc, char *argv[]) {
        int sockfd, newsockfd, portno, clilen;
        char buffer[BUF_SIZE];
        struct sockaddr_in serv_addr, cli_addr;
        pid_t childpid;
        char clientAddr[CLADDR_LEN];
        pthread_t rThread;
        pthread_t bannnanathread; 
        int ret, n, bannnana;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
           error("ERROR opening socket");
        printf("Socket created...\n");

        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
        serv_addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
            error("ERROR on binding");
        printf("Binding done...\n");

        printf("Waiting for a connection...\n");
        listen(sockfd,5);
        clilen = sizeof(cli_addr);
        while (newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) {
            inet_ntop(AF_INET, &(cli_addr.sin_addr), clientAddr, CLADDR_LEN);
            printf("Connection accepted from %s...\n", clientAddr);

            //creating a new thread for receiving messages from the client
            ret = pthread_create(&rThread, NULL, receiveMessage, (void *) newsockfd);
            bannnana = pthread_create(&bannnanathread, NULL, sendMessage, (void *) newsockfd);
            if(ret < 0) {
                printf("User has disconnected from: %d", clientAddr);
            }
        }
        printf("THEY DISCONNECTED\n");
        if (newsockfd < 0) 
            error("ERROR on accept");
//        close(newsockfd);
        printf("Closing due to lost connection");
        close(sockfd);

        pthread_exit(NULL);
        return 0; 
    }

