/*
** echos.c -- the echo server for echoc->c; demonstrates unix sockets
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <time.h>

#define maxplayer 6
__thread int threadLocal;
__thread int joinQuerry;
int flag;
int server;
int highestId;
//__thread char clientIp[INET_ADDRSTRLEN];
//char clientIpAddrRecv[INET_ADDRSTRLEN];
int previous;
//char str[100];
int next,nextExit;
struct sockaddr_in server_address, client_address;
int t;

struct Data
{
    char buffer[100];
};

struct aiInfo{
    int id;
    int posOfAttack; // Ska användas för AI aggressivitet, när ain inte längre är aggresiv ska den gå tillbaks till sn position;
    time_t timeOfAttack;
    int xPos;
    int yPos;
};
struct clientData
{
    pthread_t clientThread[maxplayer];
    int clientControl[maxplayer];
    int client[maxplayer];
    struct Data Data[maxplayer];
    int currentClient;
};

int arrayOfAisToken[100]={0}; // För att se om det finns lediga AI slots
struct aiInfo arrayOfAis[100];


void controlAi(struct clientData *c);

void updatePlayerInfo(struct clientData *c);
void stringManipulate(int intToString,int posInList, char *string);

pthread_mutex_t iMutex;
pthread_t sendThread;
int n,done;
int sendFlag=0;

void* send_recv(void* clientData)
{
    int i,deadAi;
    flag = 0;
    struct clientData *c=(struct clientData *) clientData;
    threadLocal=c->currentClient;
    pthread_mutex_unlock(&iMutex);
    pthread_mutex_t newPlayerMutex;
    pthread_mutex_init(&newPlayerMutex,NULL);
    char deadAiId[4];
    char slot[2];
    char welcomeString[17];
    char hej[4] = "hej";
    if (send(c->client[threadLocal], hej, sizeof(hej), 0) == -1) {
        perror("send");
        exit(1);
    }
        // thread release
    printf("Connected,threadlocal = %d.\n",threadLocal);
    printf("Connected,currentclient = %d.\n",c->currentClient);
    printf("Connected,clientControlToken = %d.\n",c->clientControl[threadLocal]);

        done = 0;
        printf("c->client[threadLocal]: %d\n",c->client[threadLocal]);
        while(!done) {
            n = recv(c->client[threadLocal],c->Data[threadLocal].buffer, 100, 0);
            printf("connect to threadlocal%d",threadLocal);
            if (n <= 0) {
                if (n < 0) {
                    printf("recv error %d\n",errno);
                }
                if (n == 0){
                    pthread_mutex_lock(&iMutex);
                    c->currentClient = threadLocal;
                    c->clientControl[threadLocal]=0;
                    updatePlayerInfo(c);
                    printf("closing:%d\n",c->client[threadLocal]);
                    if(close(c->client[threadLocal])==-1){
                        printf("no close on:%d\n",threadLocal);
                    }
                    nextExit = threadLocal;
                    printf("next i tråd: %d\n",next);
                    printf("not connected\n");
                    flag = 1;
                    pthread_mutex_unlock(&iMutex);
                    pthread_exit(NULL);
                }
                done = 1;
                }else{
                    if(strstr(c->Data[threadLocal].buffer, "newplyr")!=NULL){
                            //pthread_mutex_lock(&newPlayerMutex);
                            strcpy(welcomeString,"newplyr, ID:00");
                            snprintf(slot, 2, "%d",threadLocal); // gör inten till en string som man kan lägga in i spawnai meddelandet
                            strcat(welcomeString,slot);
                            welcomeString[15] = '\n'; //newline
                            welcomeString[16] = '\0'; // null
                            printf("%s\n",welcomeString);
                            if (send(c->client[threadLocal], welcomeString, sizeof(welcomeString), 0) == -1) {
                                perror("send");
                                exit(1);
                            }
                            //pthread_mutex_unlock(&newPlayerMutex);
                    }
                    if(strstr(c->Data[threadLocal].buffer, "plyrinf")!=NULL)
                        updatePlayerInfo(c);

                    c->Data[threadLocal].buffer[n] = '\0';
                    sendFlag = 1;
                }
        }
        return 0;
 }

void sendAttack(struct clientData *c){
    char attackString[30], atk[4];
    int i;
    printf("castle atk\n");
    strcpy(attackString,"cstlatk, dmg:05");
    for(i=0;i<maxplayer;i++){
        if(send(c->client[threadLocal], attackString, sizeof(attackString), 0) == -1) {
            perror("send");
        }
    }
    return;
}

void updatePlayerInfo(struct clientData *c){
    char playerInfoString[22];
    int one,two,three,four,five,six,i;
    for(i=0;i<maxplayer;i++){
        switch(i){
            case 0: if(c->clientControl[i]==1){ one = i; }else{ one = 7; }break;
            case 1: if(c->clientControl[i]==1){ two = i; }else{ two = 7; }break;
            case 2: if(c->clientControl[i]==1){ three = i; }else{ three = 7; }break;
            case 3: if(c->clientControl[i]==1){ four = i; }else{ four = 7; }break;
            case 4: if(c->clientControl[i]==1){ five = i; }else{ five = 7; }break;
            case 5: if(c->clientControl[i]==1){ six = i; }else{ six = 7; }break;
        }
    }
    snprintf(playerInfoString,21,"plrinfr: %d %d %d %d %d %d",one,two,three,four,five,six);
    playerInfoString[20]='\n';
    playerInfoString[21]='\0';
    for(i = 0;i<maxplayer;i++) // skicka til alla för att uppdatera alla om att en ny spelare finns
        if(c->clientControl[i]==1)
            if (send(c->client[i], playerInfoString, sizeof(playerInfoString), 0) == -1) {
                    perror("send");
                    exit(1);
            }
    return;
    }
void* send_to_all_clients(void* clientData)
{
    struct clientData *c=(struct clientData *) clientData;
    while(1)
    {
        //printf("sendflag: %d\n",sendFlag);
        if(sendFlag!=0)
        {
            int i,j;
            for(i=0;i<maxplayer;i++)
            {
                if(c->Data[i].buffer[0]!='\0')
                {
                    printf("buffer from client%d: %s\n",i,c->Data[i].buffer);
                    pthread_mutex_lock(&iMutex);
                    for(j=0;j<maxplayer;j++)
                    {
                        // todo client control check
                        if(c->clientControl[j]==1 && (i!=j || strstr(c->Data[i].buffer,"connected")!=NULL))
                            if(send(c->client[j],c->Data[i].buffer,sizeof(c->Data[i].buffer),0)<0)
                            {
                                perror("send to all clients");
                            }
                    }
                    sendFlag=0;
                    c->Data[i].buffer[0]='\0';
                    pthread_mutex_unlock(&iMutex);
                }
            }
        }
    }
}

void spawnAi(struct clientData *c,char x[], char y[]){
    int i;
    char spawnAiString[30];
    strcpy(spawnAiString,"spawnai, ID:000 x:0000 y:0000");
    printf("hejsan\n");
    static int nrOfAi=0; // identifikationsnumret på AIn man spawnar, static gör så att vid varje funktions call är värdet det samma.
    for(i=0;i<100;i++){
        if(arrayOfAisToken[i]==0){
            printf("%d\n",i);
            nrOfAi = i;
            break;
        }
        else if(i==99){
            i=0;
        }
            //Loopar tills någon plats blir ledig att spawna en ai på
            // TODO: Om en Ai dör bör klienten skicka ett death meddelande som gör platsen ledig
    }
    stringManipulate(nrOfAi,14,spawnAiString);
    spawnAiString[18] = x[0]; // Lägger in x koordinaterna
    spawnAiString[19] = x[1];
    spawnAiString[20] = x[2];
    spawnAiString[21] = x[3];

    spawnAiString[25] = y[0]; // lägger in y koordinaterna
    spawnAiString[26] = y[1];
    spawnAiString[27] = y[2];
    spawnAiString[28] = y[3];
    arrayOfAisToken[nrOfAi]=1;
    arrayOfAis[nrOfAi].id=nrOfAi;
    arrayOfAis[nrOfAi].xPos=atoi(x);
    arrayOfAis[nrOfAi].yPos=atoi(y);
    //nrOfAi++;
    printf("%s", spawnAiString);
    for(i=0;i<maxplayer;i++){ // en for loop som skickar ut information om att en ai har spawnat
        if(c->clientControl[i]!=0) // skickar bara till de socket handles som är använda
            if(send(c->client[i],spawnAiString,sizeof(spawnAiString),0)<0)
            {
                perror("spawn ai send failed\n");
            }
    }
}


void* aiHandler(void *clientData){ //thread

    struct clientData *c=(struct clientData *) clientData;
    int i;
    highestId = 0;
    // TODO se till att man inte spawnar på en upptagen plats
    time_t currentTime, lastTime=0;
    /*char *posStringX[5]={"0900","0932","0964","0996","1028"};
    char *posStringY[5]={"0540","0540","0540","0540","0540"};*/
    char *posStringX[5]={"0100","1856","0704","1152","0928"};
    char *posStringY[5]={"0540","0540","0064","0064","1016"};
    time(&lastTime);
    while(1){
        controlAi(c);
        time(&currentTime);
        if(currentTime>lastTime+10){
            lastTime = currentTime;
                for(i=0;i<5;i++){
                    spawnAi(c,posStringX[i],posStringY[i]);
                }
        }
    }
}


void stringManipulate(int intToString,int posInList,char* string){
    // TODO gör så man skickar in en sträng istället för att ha två globala kanske
    char intStrBufferMove[5]={0};
    char intStrBufferSpawn[5]={0};
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


void controlAi(struct clientData *c){
    int i,j,n=0,m,x,sendToken=0;
    char moveAiString[30];
    time_t currentTime;
    time(&currentTime);
    for(i=0;i<10;i++){
        usleep(1000);
        for(j=0;j<10;j++){
        strcpy(moveAiString,"moveai!, ID:000 x:0000 y:0000");
            n=j;
            m=i*10+j;
            //printf("m %d n %d\n",m,n);
            if(arrayOfAisToken[m]==1){
                if(n==0 || n==5){
                    if(arrayOfAis[m].xPos<864){ //864 är 32 pixlar från vänstra slottväggen
                            arrayOfAis[m].xPos++;
                            sendToken=1;
                    }
                    else{
                        if(currentTime>arrayOfAis[m].timeOfAttack+2){
                            arrayOfAis[m].timeOfAttack = currentTime;
                            printf("I Should attack\n");
                            sendAttack(c);
                        }
                    }
                }
                else if(n==1 || n==6){
                    if(arrayOfAis[m].xPos>992){ //864 är 32 pixlar från vänstra slottväggen
                            arrayOfAis[m].xPos--;
                            sendToken=1;
                        }
                    else{
                        if(currentTime>arrayOfAis[m].timeOfAttack+2){
                            arrayOfAis[m].timeOfAttack = currentTime;
                            printf("I Should attack\n");
                            sendAttack(c);
                        }
                    }
                }
                else if(n==2 || n==7){
                    if(arrayOfAis[m].xPos<896){
                        arrayOfAis[m].xPos++;
                        sendToken=1;
                      }
                      else if(arrayOfAis[m].yPos<476){ //864 är 32 pixlar från vänstra slottväggen
                            arrayOfAis[m].yPos++;
                            sendToken=1;
                      }
                      else{
                        if(currentTime>arrayOfAis[m].timeOfAttack+2){
                            arrayOfAis[m].timeOfAttack = currentTime;
                            printf("I Should attack\n");
                            sendAttack(c);
                        }
                    }
                }
                else if(n==3 || n==8){
                    if(arrayOfAis[m].xPos>960){
                         arrayOfAis[m].xPos--;
                         sendToken=1;
                      }
                      else if(arrayOfAis[m].yPos<476){ //864 är 32 pixlar från vänstra slottväggen
                            arrayOfAis[m].yPos++;
                            sendToken=1;
                      }
                      else{
                        if(currentTime>arrayOfAis[m].timeOfAttack+2){
                            arrayOfAis[m].timeOfAttack = currentTime;
                            printf("I Should attack\n");
                            sendAttack(c);
                        }
                    }
                 }
                else if(n==4 || n==9){
                    if(arrayOfAis[m].yPos>608){ //864 är 32 pixlar från vänstra slottväggen
                           arrayOfAis[m].yPos--;
                           sendToken=1;
                        }
                    else{
                        if(currentTime>arrayOfAis[m].timeOfAttack+2){
                            arrayOfAis[m].timeOfAttack = currentTime;
                            printf("I Should attack\n");
                            sendAttack(c);
                        }
                    }
                }
                if(sendToken){
                stringManipulate(arrayOfAis[m].id,14,moveAiString);
                stringManipulate(arrayOfAis[m].xPos,21,moveAiString);
                stringManipulate(arrayOfAis[m].yPos,28,moveAiString);
                //printf("%s",moveAiString);
                for(x=0;x<maxplayer;x++){ // en for loop som skickar ut information om att en ai har spawnat
                    if(c->clientControl[x]!=0) // skickar bara till de socket handles som är använda
                        if(send(c->client[x],moveAiString,sizeof(moveAiString),0)<0)
                        {
                            perror("spawn ai send failed\n");
                        }
                }
                sendToken=0;
                }
            }
        }
    }
}

int main(char argc ,char *argv[])
{
    int len,portno,optval,errorhej = 0;
    socklen_t optlen = sizeof(optval);
    struct clientData clientData;
    clientData.currentClient = 0;
    int errorHandle;
    char errorString[100];
    strcpy(errorString,"Server full\n");

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr=INADDR_ANY;
    server_address.sin_port=htons(atoi(argv[1]));
    len =sizeof(server_address);
    if (bind(server, (struct sockaddr *)&server_address, len) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(server, 5) == -1) {
        perror("listen");
        exit(1);
    }
    clientData.currentClient=0;
    int a;
    for(a=0;a<maxplayer;a++)
    {
        clientData.clientControl[a]=0;
        clientData.Data[a].buffer[0]='\0';

    }
    for(a=0;a<100;a++)
        arrayOfAis[a].timeOfAttack=0;

    pthread_mutex_init(&iMutex,NULL);

    joinQuerry = -1;
    int i;
    next = 0;
    int slotsAvailable = 1;
    int serverFullMsg = 0;
    pthread_create(&sendThread,NULL,&send_to_all_clients,(void *)&clientData);
    pthread_t aiThread;
    pthread_create(&aiThread,NULL,&aiHandler,(void *)&clientData);

    for(;;) {
        printf("Waiting for a connection...\n");
        t = sizeof(client_address);

        slotsAvailable=0;
        for(i=0;i<maxplayer;i++){
            if(clientData.clientControl[i]==0){
                slotsAvailable=1;
                break;
            }
        }
            if(slotsAvailable==0){
                printf("Server full\n");
                    if((errorHandle = accept(server, (struct sockaddr *)&client_address, &t)) == -1) {
                        perror("accept");
                        exit(1);
                    }
                    if (send(errorHandle, errorString, sizeof(errorString), 0) < 0) {
                        perror("send");
                        exit(1);
                    }
                    if(close(errorHandle)==-1){
                        perror("send");
                        done = 1;
                    }
                    for(i=0;i<maxplayer;i++)
                    {
                        if((clientData.clientControl[i])==0){
                            next = i;
                            printf("inside if \n");
                            break;
                        }
                    }
                }
        if(slotsAvailable==1){
            if(flag==1){
                next = nextExit;
                flag = 0;
            }
            printf("The handle i actually accept: %d\n",next);
            if((clientData.client[next] = accept(server, (struct sockaddr *)&client_address, &t)) == -1) {
                    perror("accept");
                    exit(1);
                }
                else{
                    pthread_mutex_lock(&iMutex);
                    printf("1next %d & curclient %d\n",next,clientData.currentClient);
                    for(i=0;i<maxplayer;i++)
                    {
                        if((clientData.clientControl[i])==0){
                            clientData.currentClient = next;
                            printf("inside if \n");
                            break;
                        }
                    }
                    printf("2next %d & curclient %d\n",next,clientData.currentClient);
                    clientData.clientControl[clientData.currentClient]=1;
                    for(i=0;i<maxplayer;i++)
                        {
                            if((clientData.clientControl[i])==0){
                                next = i;
                                printf("inside if \n");
                                break;
                            }
                        }
                    printf("3next %d & curclient %d\n",next,clientData.currentClient);
                    pthread_create(&clientData.clientThread[clientData.currentClient],NULL,&send_recv,(void *)&clientData);
                    if(flag == 1){
                        next=nextExit;
                        flag = 0;
                    }
                }
            }
        }
            printf("close\n" );
    close(server);
  return 0;
}
