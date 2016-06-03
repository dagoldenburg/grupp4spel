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
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "gameStruct.h"
#include "gameNet.h"
#include "gameObject.h"


int highestId;
//pthread_mutex_t sendLock;
SDL_mutex *sendLock;

void chatFunc(void *gamestate){ // Funktion för tråden som har hand om chatten
    int i=0,placeInString=1;
    GameState *g=( GameState *) gamestate;
    SDL_Event event,event2;
    while(1){
    //printf("CHAT WORKING \n");
        while( SDL_PollEvent( &event ) ){
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_RETURN){ // om man trycker p¨å enter kan man börja skriva
                    if(g->Text.enterPressedFlag==0){
                        printf("Enter pressed, flag: %d\n",g->Text.enterPressedFlag);
                        SDL_StartTextInput();
                        g->Text.enterPressedFlag=1;
                    }
                    else if(g->Text.enterPressedFlag==1){ // trycker man på enter igen uppdateras chathistoriken och meddelandet skickas
                        strcpy(g->Text.chatHistory[3],g->Text.chatHistory[2]);
                        strcpy(g->Text.chatHistory[2],g->Text.chatHistory[1]);
                        strcpy(g->Text.chatHistory[1],g->Text.chatHistory[0]);
                        strcpy(g->Text.chatHistory[0],g->Text.chatMessage);
                        printf("history %s\n",g->Text.chatHistory[0]);
                        safeSend(g->Text.chatMessage,g);
                        placeInString=1;
                        for(i=0;i<29;++i){ // suddar ut allt
                            printf("i:%d",i);
                            g->Text.chatMessage[i]=' ';
                        }
                        g->Text.chatMessage[29]='\0';
                        g->Text.chatMessage[0]='*';
                        SDL_StopTextInput();
                        g->Text.enterPressedFlag=0;
                    }
                }
            }
            else if(event.type == SDL_TEXTINPUT && g->Text.enterPressedFlag){ // Lägger in en text
                //TODO: enable backspace
                if(placeInString<29)
                    g->Text.chatMessage[placeInString] = *event.text.text;
                placeInString++;
            }
        }
    }
}

void safeSend(char sendString[30],GameState *gamestate){ // skickar meddelandet med bevarad integritet med hjälp av mutex
    int i;
    SDL_LockMutex(sendLock);

    printf("Sending: %s\n",sendString);
    int len=strlen(sendString);
    if(SDLNet_TCP_Send(gamestate->socket,sendString,len)==-1)
    {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    }
    for(i=0;i<30;i++)
        sendString[i]=' ';
    SDL_UnlockMutex(sendLock);
   // pthread_mutex_unlock(&sendLock);
}


void stringManipulate(int intToString,int posInList,char* string){//Manipulerar en sträng så man får rätt värden på rätt plats
    char intStrBufferMove[5]={'\0','\0','\0','\0','\0'};
    snprintf(intStrBufferMove, 5, "%d",intToString);
    switch(intToString){
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

void recvfunc(void *gamestate) // Tråd för att ta emot information från servern
{
    int mySlot = 0,i;
    GameState *g=( GameState *) gamestate;
    Entity tempEntity;

    int t;
    int x,y,id,hp,dmg; // varför variabel behöver en egen array som man kan lägga över från bufffern och göra atoi på
    char xArr[5],yArr[5],idArr[4],slot[3],hpArr[4],dmgArr[4], name[12], facing[2];
    highestId=0;
    while(1){
        if((t=SDLNet_TCP_Recv(g->socket,recvbuffer,30))>0)
        {
            recvbuffer[t]='\0';

        } else {
            if (t <= 0){ printf("Server closed connection\n");
            printf("Exit thread\n");
            SDL_DetachThread(g->recvThread);
            SDL_DetachThread(NULL);
            //FUnkar inte ????
            }
        }
        if(strstr(recvbuffer,"*")!=NULL){ // om man tar emot ett chatmeddelande
            strcpy(g->Text.chatHistory[3],g->Text.chatHistory[2]);
            strcpy(g->Text.chatHistory[2],g->Text.chatHistory[1]);
            strcpy(g->Text.chatHistory[1],g->Text.chatHistory[0]);
            strcpy(g->Text.chatHistory[0],recvbuffer);
        }
        else{
            if(strstr(recvbuffer,"rdyplyr")!=NULL || strstr(recvbuffer,"nrdyplr")!=NULL){ // ready unready hantering
                slot[0] = recvbuffer[12];
                g->rPlayers=atoi(slot);
                printf("recieved rPlayersis: %d\n",g->rPlayers);
            }
            if(strstr(recvbuffer,"newplyr")!=NULL){ // man får sin slot, så man vet vilken gubbe ma ska styra
                if(g->slotNeed == 0){
                    slot[0]=recvbuffer[14];
                    g->mySlot=atoi(slot);
                    printf("mySlot%d\n",g->mySlot);
                    g->slotNeed = 1;
                }
            }
            if(strstr(recvbuffer,"cstlatk")!=NULL){ // tar emot information om att slottet har tagit skada och uppdaterar det
                dmgArr[0] = recvbuffer[13];// Lägger in hp koordinaterna
                dmgArr[1] = recvbuffer[14];
                dmg=atoi(dmgArr);
                g->castle.hpData.currentHp -= dmg;
                printf("castle hp %f\n",g->castle.hpData.currentHp);
            }
            if(strstr(recvbuffer,"spawnai")!=NULL){ // tar del av information om en spawnad ai
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
            }
        if(strstr(recvbuffer,"movplyr")!=NULL){ // tar emot information om flyttat spelare
            slot[0] = recvbuffer[14];
            slot[2] = '\0';

            xArr[3] = recvbuffer[21];
            xArr[2] = recvbuffer[20];
            xArr[1] = recvbuffer[19];
            xArr[0] = recvbuffer[18];
            xArr[4] = '\0';

            yArr[3] = recvbuffer[28];
            yArr[2] = recvbuffer[27];
            yArr[1] = recvbuffer[26];
            yArr[0] = recvbuffer[25];
            yArr[4] = '\0';

            facing[0] = recvbuffer[29];
            facing[1] = '\0';

            recvbuffer[30] = '\0';

            id=atoi(slot);
            x=atoi(xArr);
            y=atoi(yArr);

            //printf("IM MOVING PLAYER: %d\n",id);
            g->playerEntity[id].facing = atoi(facing);

            g->playerEntity[id].object.rect.x = x;
            g->playerEntity[id].object.rect.y = y;


        }
        /*if(strstr(recvbuffer,"plyrnam")!=NULL){
            slot[0] = recvbuffer[14];
            slot[1] = '\n';
            slot[2] = '\0';

            for(int i = 0; i<12; i++){
                name[i] = recvbuffer[18 + i];
            }

            recvbuffer[30] = '\0';

            id=atoi(slot);

            strcpy(g->playerEntity[id].name, name);
            printf("%s", g->playerEntity[id].name);
        }*/
        if(strstr(recvbuffer,"strtgam")!=NULL){ // tar emot information om att spelet har startat
                g->playMode=1;
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
                g->AiEntity[id].hpData.currentHp = hp;
            }
            if(strstr(recvbuffer,"spwnatt")!=NULL){ //tar emot information om att en spelare har attackerat och gör en attack animation
                 printf("Player attack recieved\n");
                idArr[0] = recvbuffer[14];// Lägger in id
                idArr[1] = '\0';

                //recvbuffer[0]='\0';
                id = atoi(idArr);

                spawnAttack(&g->playerEntity[id]);
                printf("x:%d y:%d h:%d\n", g->playerEntity[id].attack.rect.x, g->playerEntity[id].attack.rect.y, g->playerEntity[id].attack.rect.h);
            }

            if(strstr(recvbuffer,"resetAT")!=NULL){ // resetar attacken som spawnades förut
                printf("ResetAttack recieved");
                idArr[0] = recvbuffer[14];// Lägger in id
                idArr[1] = '\0';

                id = atoi(idArr);
                resetAttack(&g->playerEntity[id].attack);
            }
            if(strstr(recvbuffer,"chartyp")!=NULL){ // så man vet vilken karaktär de är warrior/mage
                idArr[0] = recvbuffer[12];// Lägger in x koordinaterna
                idArr[1] = recvbuffer[13];
                idArr[2] = recvbuffer[14];
                idArr[3] = '\0';


                hpArr[0] = recvbuffer[20];// Lägger in hp koordinaterna
                hpArr[1] = recvbuffer[21];
                hpArr[2] = recvbuffer[22];
                hpArr[3] = recvbuffer[23];
                hpArr[4] = '\0';

                //recvbuffer[0]='\0';
                id = atoi(idArr);


                g->playerEntity[id].characterType = atoi(hpArr);

                printf("Character data recieved TYP: %d\n", g->playerEntity[id].characterType);
            }
             if(strstr(recvbuffer,"plrinfr")!=NULL){ // tar emot en lista om vilka slots som är upptagna
             printf("recv: %s\n",recvbuffer);
                int j = 0;
                for(int i = 0;i<g->maxPlayers;i++){
                    slot[0]=recvbuffer[9+j];
                    j+=2;
                    id = atoi(slot);
                    switch(i){
                        case 0:
                                if(id==7)
                                    g->playerEntityToken[i]=0;
                                else if(id!=7 && g->playerEntityToken[i]==0){
                                    g->playerEntityToken[i]=1;
                                    g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                                }break;
                        case 1: if(id==7)
                                    g->playerEntityToken[i]=0;
                                else if(id!=7 && g->playerEntityToken[i]==0){
                                    g->playerEntityToken[i]=1;
                                    g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                                }break;
                        case 2: if(id==7)
                                    g->playerEntityToken[i]=0;
                                else if(id!=7 && g->playerEntityToken[i]==0){
                                    g->playerEntityToken[i]=1;
                                    g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                                }break;
                        case 3: if(id==7)
                                    g->playerEntityToken[i]=0;
                                else if(id!=7 && g->playerEntityToken[i]==0){
                                    g->playerEntityToken[i]=1;
                                    g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                                }break;
                        case 4: if(id==7)
                                    g->playerEntityToken[i]=0;
                                else if(id!=7 && g->playerEntityToken[i]==0){
                                    g->playerEntityToken[i]=1;
                                    g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                                }break;
                        case 5: if(id==7)
                                    g->playerEntityToken[i]=0;
                                else if(id!=7 && g->playerEntityToken[i]==0){
                                    g->playerEntityToken[i]=1;
                                    g->playerEntity[i] = createEntity(&tempEntity,1024,508,32,32,100.0);
                                }break;
                        }
                }
            }
        if(strstr(recvbuffer,"moveai!")!=NULL){ // om man flyttar en ai
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
            recvbuffer[30] = '\0';

            id=atoi(idArr);
            x=atoi(xArr);
            y=atoi(yArr);

            if(y < g->AiEntity[id].object.rect.y ){
                g->AiEntity[id].facing = 0;
            }
            else if(y > g->AiEntity[id].object.rect.y ){
                g->AiEntity[id].facing = 2;
            }
            else if(x > g->AiEntity[id].object.rect.x ){
                g->AiEntity[id].facing = 1;
            }
            else if(x < g->AiEntity[id].object.rect.x ) {
                g->AiEntity[id].facing = 3;
            }else{
                g->AiEntity[id].facing = 4;
            }
            g->AiEntity[id].object.rect.x = x;
            g->AiEntity[id].object.rect.y = y;
        }
        }
    for(i=0;i<30;i++)
        recvbuffer[i]=' ';
    }
}

TCPsocket TCP_socket_connection() // hanterar tcp anslutningar
{
    IPaddress client;
    TCPsocket socket;
    int counter =0,i,t;
//    pthread_mutex_init(&sendLock,NULL);
    sendLock=SDL_CreateMutex();
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
         if(SDLNet_ResolveHost(&client,"130.237.84.86",32320)<0) //130.237.84.86
         {
             printf("SDLnet_resolveHost error");
             exit(EXIT_FAILURE);
         }
         if(!(socket=SDLNet_TCP_Open(&client)))
        {
                    printf("SDL_net_TCP_open error");
                    return -1;
        }
        int len;
        len =strlen(name);
        if (SDLNet_TCP_Send(socket,name, len) < len)
        {
            fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        }
        if ((t = SDLNet_TCP_Recv(socket,recvBuffer, 100)) > 0)
        {
            printf("message from server :%s",recvBuffer);
            recvBuffer[t] = '\0';
        }
        if(strstr(recvBuffer,"srvbusy")!=NULL){
            return -3;
        }
        if(strstr(recvBuffer,"Server full\n")){
            if(counter==0){
                counter++;
                goto HERE;
            }
            else if(counter==1){
                printf("Server full, disconnecting.\n");
                return -2;
            }
        }
    return socket;
 }
