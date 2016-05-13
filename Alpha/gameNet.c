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
int maxPlayers;
pthread_mutex_t sendLock;

void safeSend(char sendString[30],GameState *gamestate){
    SDL_Delay(15);
    pthread_mutex_lock(&sendLock);
    if (send(gamestate->socket, sendString, strlen(sendString), 0) == -1)
    {
        perror("send");
        exit(1);
    }
    pthread_mutex_unlock(&sendLock);
}

void *recvfunc(void *gamestate)
{
    int mySlot = 0;
    pthread_mutex_t lock;
    pthread_mutex_init(&lock,NULL);
    GameState *g=( GameState *) gamestate;
    Entity tempEntity;

    int t;
    int x,y,id, hp; // varför variabel behöver en egen array som man kan lägga över från bufffern och göra atoi på
    char xArr[5],yArr[5],idArr[4],slot[3], hpArr[4];
    highestId=0;
    while(1){
        pthread_mutex_lock(&lock);
        if ((t=recv(g->socket, recvbuffer, 100, 0)) > 0) {
            recvbuffer[t] = '\0';
          //  sscanf(buffer"%d %d",)
            //
            printf("> %s", recvbuffer);
        } else {
            if (t <= 0) perror("recv");
            else printf("Server closed connection\n");
            exit(1);
        }
        if(strstr(recvbuffer,"newplyr")!=NULL){
            printf("hej");
            slot[0]=recvbuffer[14];
            slot[1]='\n';
            slot[2]='\0';
            g->mySlot=atoi(slot);
            printf("mySlot%d\n",g->mySlot);
        }
        if(strstr(recvbuffer,"spawnai")!=NULL){
            printf("spawnai received\n");
            idArr[0] = recvbuffer[12];// Lägger in x koordinaterna
            idArr[1] = recvbuffer[13];
            idArr[2] = recvbuffer[14];
            idArr[3] = '\0';

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
            id = atoi(idArr);
            //printf("X:%d\n",x);
            //printf("Y:%d\n",y);
            //printf("highestId: %d aiId:%d\n",highestId,aiId);
            if(id>highestId){
                highestId=id;
            }
            g->aiEntityToken[id] = 1;
            //printf("highestId: %d aiId:%d entityToken: %d\n",highestId,aiId,g->aiEntityToken[aiId]);
            g->AiEntity[id]=createEntity(&tempEntity, x, y);
            g->AiEntity[id].id = id;
            g->AiEntity[id].mPosX=getAIPositionX(&g->AiEntity[id]);
            g->AiEntity[id].mPosY=getAIPositionY(&g->AiEntity[id]);
        }
 /*     if(strstr(recvbuffer,"moveai")!=NULL){

        }*/
        if(strstr(recvbuffer,"movplyr")!=NULL){
            slot[0] = recvbuffer[14];
            slot[1] = '\n';
            slot[2] = '\0';

            xArr[3] = recvbuffer[21];
            xArr[2] = recvbuffer[20];
            xArr[1] = recvbuffer[19];
            xArr[0] = recvbuffer[18];

            yArr[3] = recvbuffer[28];
            yArr[2] = recvbuffer[27];
            yArr[1] = recvbuffer[26];
            yArr[0] = recvbuffer[25];
            recvbuffer[29] = '\n';
            recvbuffer[30] = '\0';

            id=atoi(slot);
            x=atoi(xArr);
            y=atoi(yArr);
            printf("IM MOVING PLAYER: %d\n",id);

            g->playerEntity[id].object.rect.x = x;
            g->playerEntity[id].object.rect.y = y;

        }
        if(strstr(recvbuffer,"givedmg")!=NULL){
            printf("ai HealthRecieved\n");
            idArr[0] = recvbuffer[12];// Lägger in x koordinaterna
            idArr[1] = recvbuffer[13];
            idArr[2] = recvbuffer[14];
            idArr[3] = '\0';

            hpArr[0] = recvbuffer[19];// Lägger in hp koordinaterna
            hpArr[1] = recvbuffer[20];
            hpArr[2] = recvbuffer[21];
            hpArr[3] = recvbuffer[22];
            hpArr[4] = '\0';

            recvbuffer[0]='\0';
            hp = atoi(hpArr);
            id = atoi(idArr);

            printf("Damgage data recieved");
            g->AiEntity[id].hpData.currentHp = hp;
        }
        if(strstr(recvbuffer,"aidead!")!=NULL){
            printf("jag ska inte vara här \n");
            idArr[0] = recvbuffer[12];// Lägger in x koordinaterna
            idArr[1] = recvbuffer[13];
            idArr[2] = recvbuffer[14];
            idArr[3] = '\0';
            id = atoi(idArr);
            g->aiEntityToken[id] = 0;
            if(id==highestId){ //hittar nya highestId
                for(int i = highestId;i>=0;i--){
                    if(g->aiEntityToken[i]==1){
                        highestId=i;
                        break;
                      //  printf("new highest id: %d highestId");
                    }
                 }
             }
         }
         if(strstr(recvbuffer,"plrinfr")!=NULL){
            for(int i = 0;i<maxPlayers;i++){
                printf("i: Här är players innan: %d\n",g->playerEntityToken[i]);
            }
            int j = 0;
            for(int i = 0;i<maxPlayers;i++){
                slot[0]=recvbuffer[9+j];
                j+=2;
                id = atoi(slot);
                switch(i){
                    case 0:
                            if(id==7)
                                g->playerEntityToken[i]=0;
                            else{
                                g->playerEntityToken[i]=1;
                                g->playerEntity[i] = createEntity(&tempEntity,0,0);
                            }break;
                    case 1: if(id==7)
                                g->playerEntityToken[i]=0;
                            else{
                                g->playerEntityToken[i]=1;
                                g->playerEntity[i] = createEntity(&tempEntity,32,32);
                            }break;
                    case 2: if(id==7)
                                g->playerEntityToken[i]=0;
                            else{
                                g->playerEntityToken[i]=1;
                                g->playerEntity[i] = createEntity(&tempEntity,64,64);
                            }break;
                    case 3: if(id==7)
                                g->playerEntityToken[i]=0;
                            else{
                                g->playerEntityToken[i]=1;
                                g->playerEntity[i] = createEntity(&tempEntity,96,96);
                            }break;
                    case 4: if(id==7)
                                g->playerEntityToken[i]=0;
                            else{
                                g->playerEntityToken[i]=1;
                                g->playerEntity[i] = createEntity(&tempEntity,128,128);
                            }break;
                    case 5: if(id==7)
                                g->playerEntityToken[i]=0;
                            else{
                                g->playerEntityToken[i]=1;
                                g->playerEntity[i] = createEntity(&tempEntity,160,160);
                            }break;
                    }
            }
            for(int i = 0;i<maxPlayers;i++){
                printf("i: Här är players efter: %d\n",g->playerEntityToken[i]);
            }}
        pthread_mutex_unlock(&lock);
    }
}

int TCP_socket_connection()
{
    int s, i, t;
    int counter = 0;
    struct sockaddr_in serv_addr;
    char *recvBuffer = malloc(100 * sizeof(char));
    maxPlayers = 6;
    char name[25]="douglas";
    pthread_mutex_init(&sendLock,NULL);
    //printf("Enter your name(max 25 letters): ");

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
    serv_addr.sin_port = htons(atoi("3233"));
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
