#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>


#elif __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
#include "gameObject.h"
#define BUFFSIZE 4096
#define SERV_PORT 3232
pthread_t recvThread;

int highestId;

void *recvfunc(void *gamestate)
{

    pthread_mutex_t lock;
    pthread_mutex_init(&lock,NULL);
    GameState *g=(struct GameState *) gamestate;
    Entity tempEntity;
    int t;
    int x,y,aiId; // varför variabel behöver en egen array som man kan lägga över från bufffern och göra atoi på
    char xArr[5],yArr[5],aiIdArr[4];
    highestId=0;
    while(1){

        if ((t=recv(g->socket, recvbuffer, 100, 0)) > 0) {
            recvbuffer[t] = '\0';
          //  sscanf(buffer"%d %d",)
            //printf("> %s", recvbuffer);
        } else {
            if (t <= 0) perror("recv");
            else printf("Server closed connection\n");
            exit(1);
        }
        if(strstr(recvbuffer,"spawnai")!=NULL){
            printf("spawnai received\n");
            aiIdArr[0] = recvbuffer[12];// Lägger in x koordinaterna
            aiIdArr[1] = recvbuffer[13];
            aiIdArr[2] = recvbuffer[14];
            aiIdArr[3] = '\0';

            xArr[0] = recvbuffer[18];// Lägger in x koordinaterna
            xArr[1] = recvbuffer[19];
            xArr[2] = recvbuffer[20];
            xArr[3] = recvbuffer[21];
            xArr[4] = '\0';

            yArr[0] = recvbuffer[25];// lägger in y koordinaterna
            yArr[1] = recvbuffer[26];
            yArr[2] = recvbuffer[27];
            yArr[3] = recvbuffer[28];
            yArr[4] = '\0';

            recvbuffer[0]='\0';
            x = atoi(xArr);
            y = atoi(yArr);
            aiId = atoi(aiIdArr);
            printf("X:%d\n",x);
            printf("Y:%d\n",y);
            printf("highestId: %d aiId:%d\n",highestId,aiId);
            if(aiId>highestId){
                highestId=aiId;
            }
            g->aiEntityToken[aiId] = 1;
            printf("highestId: %d aiId:%d entityToken: %d\n",highestId,aiId,g->aiEntityToken[aiId]);
            g->AiEntity[aiId]=createEntity(&tempEntity, x, y);
            g->AiEntity[aiId].mPosX=getAIPositionX(&g->AiEntity[aiId]);
            g->AiEntity[aiId].mPosY=getAIPositionY(&g->AiEntity[aiId]);
        }
 /*     if(strstr(recvbuffer,"moveai")!=NULL){

        }
        if(strstr(recvbuffer,"mvplyr")!=NULL){

        }
        if(strstr(recvbuffer,"givdmg")!=NULL){

        }*/
        if(strstr(recvbuffer,"aidead")!=NULL){
            printf("jag ska inte vara här \n");
            aiIdArr[0] = recvbuffer[12];// Lägger in x koordinaterna
            aiIdArr[1] = recvbuffer[13];
            aiIdArr[2] = recvbuffer[14];
            aiIdArr[3] = '\0';
            aiId = atoi(aiIdArr);
            g->aiEntityToken[recvbuffer[aiId]] = 0;
            if(aiId==highestId){ //hittar nya highestId
                highestId=0;
                for(int i = 0;i<100;i++){
                    if(g->aiEntityToken[i]==1 && i>= highestId){
                        highestId=i;
                        printf("new highest id: %d highestId");
                    }
                }
            }
        }
    }
}

int TCP_socket_connection()
{
    int s, i, t;
    int counter = 0;
    struct sockaddr_in serv_addr;
    char *recvBuffer = malloc(100 * sizeof(char));

    char name[25]="douglas";
    printf("Enter your name(max 25 letters): ");

    //fgets(name,100,stdin);
    //name="Douglas";
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
//    pthread_create(&recvThread,NULL,recvfunc,(void *)&s);
//    while(1)
//    {
//        if (send(s, sendBuffer, strlen(sendBuffer), 0) == -1) {
//            perror("send");
//            exit(1);
//        }
//    }
//    close(s);
  return s;
}
