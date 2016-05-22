#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>


#elif __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>

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


int highestId;
int maxPlayers;
pthread_mutex_t sendLock;

void safeSend(char sendString[31],GameState *gamestate){
    SDL_Delay(15);
    pthread_mutex_lock(&sendLock);
//    if (send(gamestate->socket, sendString, strlen(sendString), 0) == -1)
//    {
//        perror("send");
//        exit(1);
//    }
     int len=strlen(sendString);
    if(SDLNet_TCP_Send(gamestate->socket,sendString,len)==-1)
    {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    pthread_mutex_unlock(&sendLock);
}


void stringManipulate(int intToString,int posInList,char* string){
    // TODO gör så man skickar in en sträng istället för att ha två globala kanske
    char intStrBufferMove[5]={0};
    snprintf(intStrBufferMove, 5, "%d",intToString);
    switch(intToString){ // korrigerar så att man skickar rätt siffra på AIn, så man kan urskilja dem.
        case 0 ... 9: string[posInList] = intStrBufferMove[0];
                      break;
        case 10 ... 99: string[posInList] = intStrBufferMove[1];
                        string[posInList-1] = intStrBufferMove[0];
                        break;
        case 100 ... 999: string[posInList] = intStrBufferMove[2];
                          string[posInList-1] = intStrBufferMove[1];
                          string[posInList-2] = intStrBufferMove[0];
                          break;
        case 1000 ... 9999: string[posInList] = intStrBufferMove[3];
                            string[posInList-1] = intStrBufferMove[2];
                            string[posInList-2] = intStrBufferMove[1];
                            string[posInList-3] = intStrBufferMove[0];
                            break;
    }
    string[29] = '\n'; //newline
    string[30] = '\0'; // null

    return;
}

void *recvfunc(void *gamestate)
{
    int mySlot = 0;
    GameState *g=( GameState *) gamestate;
    Entity tempEntity;

    int t;
    int x,y,id,hp; // varför variabel behöver en egen array som man kan lägga över från bufffern och göra atoi på
    char xArr[5],yArr[5],idArr[4],slot[3],hpArr[4];
    highestId=0;
    while(1){
//        if ((t=recv(g->socket, recvbuffer, 30, 0)) > 0) {
//            recvbuffer[t] = '\0';
//            sscanf(buffer"%d %d",)
//
//
//        }
        if((t=SDLNet_TCP_Recv(g->socket,recvbuffer,30))>0)
        {
            recvbuffer[t]='\0';
            printf(">%s",recvbuffer);

        }else {
            if (t <= 0) perror("recv");
            else printf("Server closed connection\n");
            exit(1);
        }
        if(strstr(recvbuffer,"newplyr")!=NULL){
            slot[0]=recvbuffer[14];
            slot[1]='\n';
            slot[2]='\0';
            g->mySlot=atoi(slot);
            printf("mySlot%d\n",g->mySlot);
        }
        if(strstr(recvbuffer,"cstlatk")!=NULL){
            hpArr[0] = recvbuffer[13];// Lägger in hp koordinaterna
            hpArr[1] = recvbuffer[14];
            hp=atoi(hpArr);
            printf("> %s",recvbuffer);
            g->castle.hpData.currentHp -= hp;
            printf("castle hp %f\n",g->castle.hpData.currentHp);
        }
        if(strstr(recvbuffer,"spawnai")!=NULL){
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
            g->AiEntity[id].id=id;
            g->aiEntityToken[id] = 1;
            //printf("highestId: %d aiId:%d entityToken: %d\n",highestId,aiId,g->aiEntityToken[aiId]);
            g->AiEntity[id]=createEntity(&tempEntity, x, y,32,32,20.0);
            g->AiEntity[id].mPosX=getAIPositionX(&g->AiEntity[id]);
            g->AiEntity[id].mPosY=getAIPositionY(&g->AiEntity[id]);
        }
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
            //printf("IM MOVING PLAYER: %d\n",id);

            g->playerEntity[id].object.rect.x = x;
            g->playerEntity[id].object.rect.y = y;

        }
        if(strstr(recvbuffer,"givedmg")!=NULL){
            idArr[0] = recvbuffer[12];// Lägger in x koordinaterna
            idArr[1] = recvbuffer[13];
            idArr[2] = recvbuffer[14];
            idArr[3] = '\0';

            hpArr[0] = recvbuffer[19];// Lägger in hp koordinaterna
            hpArr[1] = recvbuffer[20];
            hpArr[2] = recvbuffer[21];
            hpArr[3] = recvbuffer[22];
            hpArr[4] = '\0';

            //recvbuffer[0]='\0';
            hp = atoi(hpArr);
            id = atoi(idArr);

            printf("Damgage data recieved");
            g->AiEntity[id].hpData.currentHp = hp;
        }
         if(strstr(recvbuffer,"plrinfr")!=NULL){
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
                                g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                            }break;
                    case 1: if(id==7)
                                g->playerEntityToken[i]=0;
                            else{
                                g->playerEntityToken[i]=1;
                                g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                            }break;
                    case 2: if(id==7)
                                g->playerEntityToken[i]=0;
                            else{
                                g->playerEntityToken[i]=1;
                                g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                            }break;
                    case 3: if(id==7)
                                g->playerEntityToken[i]=0;
                            else{
                                g->playerEntityToken[i]=1;
                                g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                            }break;
                    case 4: if(id==7)
                                g->playerEntityToken[i]=0;
                            else{
                                g->playerEntityToken[i]=1;
                                g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                            }break;
                    case 5: if(id==7)
                                g->playerEntityToken[i]=0;
                            else{
                                g->playerEntityToken[i]=1;
                                g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                            }break;
                    }
            }
        }
        if(strstr(recvbuffer,"moveai!")!=NULL){
            idArr[0] = recvbuffer[12];// Lägger in x koordinaterna
            idArr[1] = recvbuffer[13];
            idArr[2] = recvbuffer[14];
            idArr[3] = '\0';
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

            id=atoi(idArr);
            x=atoi(xArr);
            y=atoi(yArr);

            g->AiEntity[id].object.rect.x = x;
            g->AiEntity[id].object.rect.y = y;
        }

        if(threadJoinQuerry!=0){
            printf("RECV THREAD KILLED\n");
            pthread_exit(NULL);
        }
    }
}

TCPsocket TCP_socket_connection()
{
    IPaddress client;
    TCPsocket socket;
    maxPlayers = 6;
    int counter =0,i,t;
    pthread_mutex_init(&sendLock,NULL);
//    sendLock=SDL_CreateMutex();
    char *recvBuffer = malloc(100 * sizeof(char));

    char name[25]="unknown";
//    printf("Enter your name(max 25 letters): ");
//    fgets(name,100,stdin);
//        //name="Douglas";
//    for(i=0;i<25;i++)
//    {
//        if(name[i]=='\n')
//        {
//            name[i]='\0';
//        }
//
//    }

    strcat(name," connected\n");
    HERE:

        if(SDLNet_Init()<0)
        {
             printf("SDL_net init error");
             exit(EXIT_FAILURE);
        }
         if(SDLNet_ResolveHost(&client,"130.237.84.86",3231)<0)
         {
             printf("SDLnet_resolveHost error");
             exit(EXIT_FAILURE);
         }
         if(!(socket=SDLNet_TCP_Open(&client)))
        {
                    printf("SDL_net_TCP_open error");
                    exit(EXIT_FAILURE);
        }
        int len;
        len =strlen(name);
        if (SDLNet_TCP_Send(socket,name, len) < len)
        {
            fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
            exit(EXIT_FAILURE);
        }
        if ((t = SDLNet_TCP_Recv(socket,recvBuffer, 100)) > 0)
        {
            recvBuffer[t] = '\0';
            printf("message from server :%s",recvBuffer);
        }
        if(strstr(recvBuffer,"Server full\n")){
            if(counter==0){
                counter++;
                //SDLNet_TCP_Close(socket);
                goto HERE;
            }
            else if(counter==1){
                printf("Server full, disconnecting.\n");
                SDLNet_TCP_Close(socket);
                return 0;
            }
        }else

    return socket;
 }
