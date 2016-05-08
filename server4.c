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

#define maxplayer 3
__thread int threadLocal;
__thread int joinQuerry;
int flag;
int server;
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
struct clientData
{
    pthread_t clientThread[maxplayer];
    int clientControl[maxplayer];
    int client[maxplayer];
    struct Data Data[maxplayer];
    int currentClient;
};
pthread_mutex_t iMutex;
pthread_t sendThread;
int n,done;
int sendFlag=0;
void* send_recv(void* clientData)
{
    int i;
    flag = 0;
    struct clientData *c=(struct clientData *) clientData;
    threadLocal=c->currentClient;
    pthread_mutex_unlock(&iMutex);

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
                    printf("closing:%d\n",c->client[threadLocal]);
                    if(close(c->client[threadLocal])==-1){
                        printf("no close on:%d\n",threadLocal);
                    }
                    c->clientControl[threadLocal]=0;
                    nextExit = threadLocal;
                    printf("next i tråd: %d\n",next);
                    printf("not connected\n");
                    flag = 1;
                    pthread_mutex_unlock(&iMutex);
                    pthread_exit(NULL);
                }
                done = 1;
                }else{

                    c->Data[threadLocal].buffer[n] = '\0';
                    sendFlag = 1;
                }
        }
        return 0;
 }
void* send_to_all_clients(void* clientData)
{
    struct clientData *c=(struct clientData *) clientData;
    while(1)
    {
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
    char spawnAiString[30], intStrBuffer[7]={0};
    int i;
    static int nrOfAi=0; // identifikationsnumret på AIn man spawnar, static gör så att vid varje funktions call är värdet det samma.
    if(nrOfAi>999999) // börjar om från 0 ifall de har spawnat 1 million AIs. de tidigaste bör vara döda
        nrOfAi=0;
    strcpy(spawnAiString,"spawnai #00000s x:0000 y:0000"); // spawnai kommer tolkas som att clienten ska skapa en ai, med ett id nummer och kordinater
    snprintf(intStrBuffer, 7, "%d",nrOfAi); // gör inten till en string som man kan lägga in i spawnai meddelandet
    switch(nrOfAi){ // korrigerar så att man skickar rätt siffra på AIn, så man kan urskilja dem.
        case 0 ... 9: spawnAiString[14] = intStrBuffer[0];
                      break;
        case 10 ... 99: spawnAiString[14] = intStrBuffer[1];
                        spawnAiString[13] = intStrBuffer[0];
                        break;
        case 100 ... 999: spawnAiString[14] = intStrBuffer[2];
                          spawnAiString[13] = intStrBuffer[1];
                          spawnAiString[12] = intStrBuffer[0];
                          break;
        case 1000 ... 9999: spawnAiString[14] = intStrBuffer[3];
                            spawnAiString[13] = intStrBuffer[2];
                            spawnAiString[12] = intStrBuffer[1];
                            spawnAiString[11] = intStrBuffer[0];
                            break;
        case 10000 ... 99999: spawnAiString[14] = intStrBuffer[4];
                              spawnAiString[13] = intStrBuffer[3];
                              spawnAiString[12] = intStrBuffer[2];
                              spawnAiString[11] = intStrBuffer[1];
                              spawnAiString[10] = intStrBuffer[0];
                              break;
        case 100000 ... 999999: spawnAiString[14] = intStrBuffer[5];
                                spawnAiString[13] = intStrBuffer[4];
                                spawnAiString[12] = intStrBuffer[3];
                                spawnAiString[11] = intStrBuffer[2];
                                spawnAiString[10] = intStrBuffer[1];
                                spawnAiString[9] = intStrBuffer[0];
                                break;
    }
    nrOfAi++;
    spawnAiString[18] = x[0]; // Lägger in x koordinaterna
    spawnAiString[19] = x[1];
    spawnAiString[20] = x[2];
    spawnAiString[21] = x[3];

    spawnAiString[25] = y[0]; // lägger in y koordinaterna
    spawnAiString[26] = y[1];
    spawnAiString[27] = y[2];
    spawnAiString[28] = y[3];
    spawnAiString[29] = '\n'; //newline
    spawnAiString[30] = '\0'; // null

    printf("%s", spawnAiString);
    for(i=0;i<maxplayer;i++){ // en for loop som skickar ut information om att en ai har spawnat
        if(c->clientControl[i]!=0) // skickar bara till de socket handles som är använda
            if(send(c->client[i],spawnAiString,sizeof(spawnAiString),0)<0)
            {
                perror("spawn ai send failed\n");
            }
    }
}

void* aiSpawner(void *clientData){ //thread

    struct clientData *c=(struct clientData *) clientData;
    int i;
    time_t currentTime, lastTime=0;
    char *posStringX[5]={"0090","0032","1000","0032","0404"};
    char *posStringY[5]={"0100","0032","0032","1000","0800"};

    time(&lastTime);
    while(1){
        time(&currentTime);
        if(currentTime>lastTime+5){
            lastTime = currentTime;
                for(i=0;i<5;i++)
                    spawnAi(c,posStringX[i],posStringY[i]);
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

    pthread_mutex_init(&iMutex,NULL);

    joinQuerry = -1;
    int i;
    next = 0;
    int slotsAvailable = 1;
    int serverFullMsg = 0;
    pthread_create(&sendThread,NULL,&send_to_all_clients,(void *)&clientData);
    pthread_t spawnThread;
    pthread_create(&spawnThread,NULL,&aiSpawner,(void *)&clientData);

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
