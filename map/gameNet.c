#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>


#elif __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include<SDL2/SDL_ttf.h>

#endif
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "main.h"

#include "playField.h"
#include "gameStruct.h"
#include "gameNet.h"

#define BUFFSIZE 4096
#define SERV_PORT 3232

void *recvfunc(void *sock)
{
    int s = *(int*) sock;
    int t;
    while(1){
        if ((t=recv(s, recvbuffer, 100, 0)) > 0) {
            recvbuffer[t] = '\0';
          //  sscanf(buffer"%d %d",)
            printf("> %s", recvbuffer);
        } else {
            if (t <= 0) perror("recv");
            else printf("Server closed connection\n");
            exit(1);
        }
    }

}


int TCP_socket_connection()
{
    int s, i, t;
    int counter = 0;
    struct sockaddr_in serv_addr;
    char *recvBuffer = malloc(100 * sizeof(char));

    char name[25];
    printf("Enter your name(max 25 letters): ");

    fgets(name,100,stdin);

    for(i=0;i<25;i++)
    if(name[i]=='\n')
        name[i]='\0';
    strcat(name," connected\n");
HERE:

    if ((s = socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
                perror("ERROR opening socket");
    }
  // memset(&serv_addr,0,sizeof(serv_addr));
   serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi("3232"));
if (connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    perror("connect");
    exit(1);
  }
  if (send(s, name, strlen(name), 0) == -1) {
            perror("send");
            exit(1);
        }
    if ((t=recv(s, recvBuffer, 100, 0)) > 0) {
            recvBuffer[t] = '\0';
             }
     if(strstr(recvBuffer,"Server full\n")){
        if(counter==0){
            counter++;
            close(s);
            goto HERE;
        }
        else if(counter==1){
            printf("Server full, disconnecting.\n");
            close(s);
            return 0;
        }
    }
    //pthread_create(&recvThread,NULL,recvfunc,(void *)&s);

    //close(s);
  return s;
}

