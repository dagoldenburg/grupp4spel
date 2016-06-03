/*
** echos.c -- the echo server for echoc->c; demonstrates unix sockets
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <pthread.h>
#include <netinet/in.h>
#include <time.h>

#define MAX_PLAYER 6
__thread int threadLocal;
int flag;
int server;
int highestId;
int next,nextExit;
struct sockaddr_in server_address;

struct Data
{
    char buffer[30];
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
    pthread_t clientThread[MAX_PLAYER];
    int clientControl[MAX_PLAYER];
    int client[MAX_PLAYER];
    struct Data Data[MAX_PLAYER];
    int currentClient;
    int lobbyMode;
    int errorHandle;
    char errorString[100];
    char lobbyString[12];
    int readyArray[MAX_PLAYER];
    int readyPlayers;
    int masterServerHandle;
    struct sockaddr_in client_address[MAX_PLAYER];
    struct sockaddr_in error_address;

};

int arrayOfAisToken[300]={0}; // För att se om det finns lediga AI slots, 300 för 5*6*600 ai*antalgångerperminut*speltid
struct aiInfo arrayOfAis[300];

void controlAi(struct clientData *c);

void updatePlayerInfo(struct clientData *c);
void stringManipulate(int intToString,int posInList, char *string);

pthread_mutex_t iMutex;
pthread_t sendThread;
int done;
int sendFlag=0;

pthread_mutex_t sendLock;

void daemonize(void)
{
    pid_t pid, sid;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();

    FILE *f = fopen("daemonpid.txt","w");
    if (f == NULL)
    {
    printf("Error opening file!\n");
    exit(1);
     }
    fprintf(f, "%d", getpid());
    fclose(f);

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    /* At this point we are executing as the child process */


    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    /* Change the current working directory.  This prevents the current
       directory from being locked; hence not being able to remove it. */
    if ((chdir("/")) < 0) {
        exit(EXIT_FAILURE);
    }

    /* Redirect standard files to /dev/null */
    freopen( "/dev/null", "r", stdin);
    freopen( "/dev/null", "w", stdout);
    freopen( "/dev/null", "w", stderr);
}

void safeSend(char sendString[30],int sockethandle){ //Skickar en sträng, mutexar för att flera trådar kan skicka saker till cienter och det behövs säkerhet
    //pthread_mutex_lock(&sendLock);
    pthread_mutex_lock(&sendLock);
    printf("Sending: %s\n",sendString);
    int len=strlen(sendString);
    if(send(sockethandle,sendString,len,0)==-1)
    {
        perror("safesend failed\n");
    }
   /* for(i=0;i<30;i++)
        sendString[i]='\0';*/
    pthread_mutex_unlock(&sendLock);
   // pthread_mutex_unlock(&sendLock);
}

int notReady(struct clientData *c,int threadLocal){ // Om en spelar trycker på "not ready" i lobbyn, skickas det här till andra klienter för att framhålla information
    int i;
    char number[2];
    char nReady[30]="nrdyplr,.NR:0.................";

    if(c->readyArray[threadLocal]==1){
        c->readyArray[threadLocal]=0;
        c->readyPlayers--;
        printf("Ready PLayers: %d\n",c->readyPlayers);
        stringManipulate(c->readyPlayers,12,nReady);


    for(i = 0;i<MAX_PLAYER;i++)
        if(c->clientControl[i]==1)
            safeSend(nReady,c->client[i]);
    }
    return;
}

int ready(struct clientData *c,int threadLocal){// Om en spelare trycker på redo anropas den här funktionen
    int i;
    char ready[30]="rdyplyr,.NR:0.................";
    if(c->readyArray[threadLocal]==0){
        c->readyArray[threadLocal]=1;
        c->readyPlayers++;
        printf("Ready PLayers: %d TL:%d\n",c->readyPlayers,threadLocal);
        stringManipulate(c->readyPlayers,12,ready);

    for(i = 0;i<MAX_PLAYER;i++) // skicka til alla för att uppdatera alla om att en ny spelare finns
        if(c->clientControl[i]==1)
            safeSend(ready,c->client[i]);
    }
    return;
}

void* send_recv(void* clientData)
{
    int i,deadAi,n;
    flag = 0;
    struct clientData *c=(struct clientData *) clientData;
    threadLocal=c->currentClient;
    pthread_mutex_unlock(&iMutex);
    pthread_mutex_t newPlayerMutex;
    pthread_mutex_init(&newPlayerMutex,NULL);
    char deadAiId[4];
    char slot[2];
    char welcomeString[30];
    char hej[4] = "hej";
    if (send(c->client[threadLocal], hej, sizeof(hej), 0) == -1) {
        perror("send");
    }
        // thread release
    printf("Connected,threadlocal = %d.\n",threadLocal);
    printf("Connected,currentclient = %d.\n",c->currentClient);
    printf("Connected,clientControlToken = %d.\n",c->clientControl[threadLocal]);

        done = 0;
        printf("c->client[threadLocal]: %d\n",c->client[threadLocal]);
        while(!done) {
            n = recv(c->client[threadLocal],c->Data[threadLocal].buffer, 30, 0);
            c->Data[threadLocal].buffer[n] = '\0';
            printf("connect to threadlocal%d",threadLocal);
            if (n <= 0) { // Om man märker att klienten är frånkopplad eller man får ett fel så frigörs resurserna och uppkopplingen stängs ner på serversidan med
                    pthread_mutex_lock(&iMutex);
                    c->currentClient = threadLocal;
                    notReady(c,threadLocal); // Tar bort ready ifall någon disconnectar
                    c->clientControl[threadLocal]=0;
                    updatePlayerInfo(c); // skickar upp den nya listan av spelare, för att informera folk att någon  har disconnectat
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
                    done = 1;
                }else{
                    if(strstr(c->Data[threadLocal].buffer,"disconnecting")!=NULL){ // om någon disconnectar utan krash så skickas det här meddelandet, och servern stänger ner uppkopplingen
                    pthread_mutex_lock(&iMutex);
                    c->currentClient = threadLocal;
                    notReady(c,threadLocal);// Tar bort ready ifall någon disconnectar
                    c->clientControl[threadLocal]=0;
                    updatePlayerInfo(c); // skickar upp den nya listan av spelare, för att informera folk att någon  har disconnectat
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
                    done = 1;
                    }
                    else if(strstr(c->Data[threadLocal].buffer, "newplyr")!=NULL){ // Om en ny spelare connectar skickas deras slot på servern tillbaks till dom, så klienten vet vilken karaktär den ska följa
                            strcpy(welcomeString,"newplyr, ID:00");
                            snprintf(slot, 2, "%d",threadLocal); // gör inten till en string som man kan lägga in i spawnai meddelandet
                            strcat(welcomeString,slot);
                            strcat(welcomeString,"..............."); //Fyller ut strängen
                            printf("%s\n",welcomeString);
                            safeSend(welcomeString,c->client[threadLocal]);
                            //pthread_mutex_unlock(&newPlayerMutex);
                    }
                    else if(strstr(c->Data[threadLocal].buffer, "plyrinf")!=NULL)//när någon connectar frågar de om en lista över spelar
                        updatePlayerInfo(c); //Den här funktionen tillhandanhåller den listan
                    else if(strstr(c->Data[threadLocal].buffer, "plrrdy!")!=NULL){//Om en spelar säger att den är redo
                        ready(c,threadLocal); // Meddelas alla andra spelare om det
                        if(c->readyPlayers>=1){ // om tillräckligt många spelare har tyckt på ready startas spelet
                        printf("Run mode!\n");
                            c->lobbyMode = 0;
                            char startgame[30]="strtgam......................."; //skickas ut för att klienterna ska starta spelet
                            for(i = 0;i<MAX_PLAYER;i++){
                                if(c->clientControl[i]==1)
                                    safeSend(startgame,c->client[i]);
                            }
                            sleep(3);
                        }
                    }
                    else if(strstr(c->Data[threadLocal].buffer, "plrnrdy")!=NULL){ // om någon säger att dom inte är redo får alla klienter reda på det här
                        notReady(c,threadLocal);
                    }
                    else{
                        sendFlag=1;
                        while(sendFlag);
                    }
                }
        }
        return 0;
}

void sendAttack(struct clientData *c){ //Skickar att en ai attack har skett mot slottet
    char attackString[30], atk[4];
    int i;
    printf("castle atk\n");
    strcpy(attackString,"cstlatk, dmg:05...............");
    for(i=0;i<MAX_PLAYER;i++){
        if(c->clientControl[i]==1)
            safeSend(attackString,c->client[i]);
    }
    return;
}

void updatePlayerInfo(struct clientData *c){ // Skickar ut listan över alla ansluta klienter
    char playerInfoString[30];
    int one,two,three,four,five,six,i;
    for(i=0;i<MAX_PLAYER;i++){
        switch(i){
            case 0: if(c->clientControl[i]==1){ one = i; }else{ one = 7; }break;
            case 1: if(c->clientControl[i]==1){ two = i; }else{ two = 7; }break;
            case 2: if(c->clientControl[i]==1){ three = i; }else{ three = 7; }break;
            case 3: if(c->clientControl[i]==1){ four = i; }else{ four = 7; }break;
            case 4: if(c->clientControl[i]==1){ five = i; }else{ five = 7; }break;
            case 5: if(c->clientControl[i]==1){ six = i; }else{ six = 7; }break;
        }
    }
    snprintf(playerInfoString,30,"plrinfr: %d %d %d %d %d %d..........",one,two,three,four,five,six);
    playerInfoString[29]='\n';
    playerInfoString[30]='\0';
    printf("player info stirng: %s\n",playerInfoString);
    for(i = 0;i<MAX_PLAYER;i++) // skicka til alla för att uppdatera alla om att en ny spelare finns
        if(c->clientControl[i]==1)
            safeSend(playerInfoString,c->client[i]);
    return;
    }

void* send_to_all_clients(void* clientData)
{
    struct clientData *c=(struct clientData *) clientData;
    while(1)
    {
        if(sendFlag!=0)
        {
            int i,j,a;
            for(i=0;i<MAX_PLAYER;i++)
            {
                if(c->Data[i].buffer[0]!='\0')
                {
                    printf("buffer from client%d: %s\n",i,c->Data[i].buffer);
                    pthread_mutex_lock(&iMutex);
                    for(j=0;j<MAX_PLAYER;j++)
                    {
                        // todo client control check
                        if(c->clientControl[j]==1 && (i!=j || strstr(c->Data[i].buffer,"connected")!=NULL)) // skickar ut något från en klient till alla andra klienter, men inte till den skickande klienten
                            safeSend(c->Data[i].buffer,c->client[j]);
                    }
                    sendFlag=0;
                    for(a=0;a<30;a++){ // tömmer arrayen, möjligtvis onödig
                        c->Data[i].buffer[a]='\0';
                    }
                    pthread_mutex_unlock(&iMutex);
                }
            }
        }
    }
}

void spawnAi(struct clientData *c,char x[], char y[],int *nrOfAi){ //spawnar ai motståndare
    int i;
    char spawnAiString[30];
    strcpy(spawnAiString,"spawnai, ID:000 x:0000 y:0000.");
    for(i=0;i<300;i++){
        if(arrayOfAisToken[i]==0){
            *nrOfAi = i;
            break;
        }
        else if(i==299){
            i=0;
        }
    }
            //Loopar tills någon plats blir ledig att spawna en ai på
            // TODO: Om en Ai dör bör klienten skicka ett death meddelande som gör platsen ledig
    stringManipulate(*nrOfAi,14,spawnAiString);
    spawnAiString[18] = x[0]; // Lägger in x koordinaterna
    spawnAiString[19] = x[1];
    spawnAiString[20] = x[2];
    spawnAiString[21] = x[3];

    spawnAiString[25] = y[0]; // lägger in y koordinaterna
    spawnAiString[26] = y[1];
    spawnAiString[27] = y[2];
    spawnAiString[28] = y[3];
    arrayOfAisToken[*nrOfAi]=1;
    arrayOfAis[*nrOfAi].id=*nrOfAi;
    arrayOfAis[*nrOfAi].xPos=atoi(x);
    arrayOfAis[*nrOfAi].yPos=atoi(y);
    for(i=0;i<MAX_PLAYER;i++){ // en for loop som skickar ut information om att en ai har spawnat
        if(c->clientControl[i]!=0) // skickar bara till de socket handles som är använda
            safeSend(spawnAiString,c->client[i]);
    }
}


void* aiHandler(void *clientData){ //kontrollerar allt som har med ai att göra
    struct clientData *c=(struct clientData *) clientData;
    int i;
    c->lobbyMode = 1;
    highestId = 0;
    int nrOfAi=0;
    // TODO se till att man inte spawnar på en upptagen plats
    time_t currentTime, lastTime=0;
    char *posStringX[5]={"0100","1856","0704","1152","0928"}; // koordinaterna för vart ai ska spawna
    char *posStringY[5]={"0540","0540","0064","0064","1016"};
    time(&lastTime);
    while(1){
        if(c->clientControl[0]==0 && c->clientControl[1]==0 && c->clientControl[2]==0 && c->clientControl[3]==0 && c->clientControl[4]==0 && c->clientControl[5]==0){
            c->lobbyMode=1; // om alla har lämnat sätt servern i lobbymode igen
            for(i=0;i<300;i++){
                arrayOfAisToken[i]=0;
            }
            nrOfAi=0;
        }
        controlAi(c);
        time(&currentTime);
        if(currentTime>lastTime+10 && c->lobbyMode==0){ // var 10onde sekund spawnar en våg av fiender
            lastTime = currentTime;
                for(i=0;i<5;i++){
                    spawnAi(c,posStringX[i],posStringY[i], &nrOfAi);
                }
        }
        else if(c->lobbyMode==1) // uppdaterar last time om spelet inte körs, så att ain spawnar 10 sekunder efter spelets start
            time(&lastTime);
    }
}


void stringManipulate(int intToString,int posInList,char* string){ // Manipulerar stringen så att den informationen man vill skicka kommer med
    char intStrBufferMove[5]={'\0','\0','\0','\0','\0'};
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

void controlAi(struct clientData *c){ // styr Ais
    int i,j,n=0,m,x,sendToken=0;
    static int aiThrtl1=0,aiThrtl2=0,aiThrtl3=0,aiThrtl4=0,aiThrtl5=0;
    char moveAiString[30];
    time_t currentTime;
    time(&currentTime);
    for(i=0;i<30;i++){
        usleep(1000); // gör så att man inte skickar alltför ofta
        for(j=0;j<10;j++){
        strcpy(moveAiString,"moveai!, ID:000 x:0000 y:0000.");
            n=j;
            m=i*10+j;
            //printf("m %d n %d\n",m,n);
            if(arrayOfAisToken[m]==1){
                if(n==0 || n==5){
                    if(arrayOfAis[m].xPos<864){ //waypoint
                            arrayOfAis[m].xPos++;
                            aiThrtl1++;
                            if(aiThrtl1==3){ // gör så att man bara skickar var tredje gång positionen uppdateras för att inte skicka ut för många paket
                                aiThrtl1=0;
                                sendToken=1;
                            }
                    }
                    else{
                        if(currentTime>arrayOfAis[m].timeOfAttack+2){ // om den inte rör sig ska den attackera
                            arrayOfAis[m].timeOfAttack = currentTime;
                            sendAttack(c);
                        }
                    }
                }
                else if(n==1 || n==6){
                    if(arrayOfAis[m].xPos>992){ //waypoint
                            arrayOfAis[m].xPos--;
                            aiThrtl2++;
                            if(aiThrtl2==3){// gör så att man bara skickar var tredje gång positionen uppdateras för att inte skicka ut för många paket
                                aiThrtl2=0;
                                sendToken=1;
                            }
                        }
                    else{
                        if(currentTime>arrayOfAis[m].timeOfAttack+2){//om den inte rör sig ska den attackera
                            arrayOfAis[m].timeOfAttack = currentTime;
                            sendAttack(c);
                        }
                    }
                }
                else if(n==2 || n==7){
                    if(arrayOfAis[m].xPos<896){ // waypoint
                        arrayOfAis[m].xPos++;
                        aiThrtl3++;
                        if(aiThrtl3==3){ // gör så att man bara skickar var tredje gång positionen uppdateras för att inte skicka ut för många paket
                            aiThrtl3=0;
                            sendToken=1;
                        }
                      }
                      else if(arrayOfAis[m].yPos<476){ //waypoint
                            arrayOfAis[m].yPos++;
                            aiThrtl3++;
                            if(aiThrtl3==3){// gör så att man bara skickar var tredje gång positionen uppdateras för att inte skicka ut för många paket
                                aiThrtl3=0;
                                sendToken=1;
                            }
                      }
                      else{
                        if(currentTime>arrayOfAis[m].timeOfAttack+2){ // om den inte rör på sig ska den attackera
                            arrayOfAis[m].timeOfAttack = currentTime;
                            sendAttack(c);
                        }
                    }
                }
                else if(n==3 || n==8){
                    if(arrayOfAis[m].xPos>960){ // waypoint
                         arrayOfAis[m].xPos--;
                         aiThrtl4++;
                         if(aiThrtl4==3){// gör så att man bara skickar var tredje gång positionen uppdateras för att inte skicka ut för många paket
                            aiThrtl4=0;
                            sendToken=1;
                        }
                      }
                      else if(arrayOfAis[m].yPos<476){ // waypoint
                            arrayOfAis[m].yPos++;
                            aiThrtl4++;
                            if(aiThrtl4==3){// gör så att man bara skickar var tredje gång positionen uppdateras för att inte skicka ut för många paket
                                aiThrtl4=0;
                                sendToken=1;
                            }
                      }
                      else{
                        if(currentTime>arrayOfAis[m].timeOfAttack+2){ // om den inte rör sig ska den attackera
                            arrayOfAis[m].timeOfAttack = currentTime;
                            sendAttack(c);
                        }
                    }
                 }
                else if(n==4 || n==9){
                    if(arrayOfAis[m].yPos>608){ //waypoint
                           arrayOfAis[m].yPos--;
                           aiThrtl5++;
                           if(aiThrtl5==3){// gör så att man bara skickar var tredje gång positionen uppdateras för att inte skicka ut för många paket
                                aiThrtl5=0;
                                sendToken=1;
                            }
                        }
                    else{
                        if(currentTime>arrayOfAis[m].timeOfAttack+2){ // om den inte rör sig ska den attackera
                            arrayOfAis[m].timeOfAttack = currentTime;
                            sendAttack(c);
                        }
                    }
                }
                if(sendToken){
                stringManipulate(arrayOfAis[m].id,14,moveAiString);
                stringManipulate(arrayOfAis[m].xPos,21,moveAiString);
                stringManipulate(arrayOfAis[m].yPos,28,moveAiString);
                //printf("%s",moveAiString);
                for(x=0;x<MAX_PLAYER;x++){ // en for loop som skickar ut information om att en ai har gjort något
                    if(c->clientControl[x]!=0) // skickar bara till de socket handles som är använda
                        safeSend(moveAiString,c->client[x]);
                }
                sendToken=0;
                }
            }
        }
    }
}
/*
void* masterServer(void *clientData){
    int i,players;
    struct clientData *c=(struct clientData *) clientData;
    char recvBuffer[30];
    char sendBuffer[30];
    while(1){
        if(recv(c->masterServerHandle,recvBuffer,sizeof(recvBuffer),NULL)==-1){
            printf("Error in communication with master server\n");
        }
        if(strstr(infoBuffer,"listInfo")!=NULL){
            strcpy(sendBuffer,"srvrinf: plr:0 rplr:0 lby:0....");
            for(i=0;i<MAX_PLAYER;i++){
                if(c->clientControl[i]==1){
                    players++;
                }
            }
            stringManipulate(players,13,sendBuffer);
            stringManipulate(clientData.readyPlayers,20,sendBuffer);
            stringManipulate(clientData.lobbyMode,26,sendBuffer);

            if(send(c->masterServerHandle,sendBuffer,sizeof(sendBuffer),0)<0){
                printf("Error in communication with master server\n");
            }
        }
    }
}*/

int main(char argc ,char *argv[])
{
    daemonize();
    int len,portno,optval,errorhej = 0;
    socklen_t optlen = sizeof(optval);
    struct clientData clientData;
    clientData.currentClient = 0;
    strcpy(clientData.errorString,"Server full\n");

   /* masterServ_addr.sin_family = AF_INET; //Hann tyvärr inte bli klar med master servern, en dag till och det hade varit klart :(
    masterServ_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    masterServ_addr.sin_port = htons(atoi("3230"));
    if (connect(clientData.masterServerHandle, (struct sockaddr *)&masterServ_addr, sizeof(masterServ_addr)) == -1) {
        printf("Could not connect to the master server\n");
        exit(1);
    }
    pthread_t masterServerThread;
    pthread_create(&masterServerThread,NULL,&send_recv,(void *)&clientData);*/

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == -1) { // Gör en socket
        perror("socket");
        exit(1);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr=INADDR_ANY;
    server_address.sin_port=htons(atoi(argv[1])); //Port
    len =sizeof(server_address);
    if (bind(server, (struct sockaddr *)&server_address, len) == -1) {//binder socketen till addressen och porten
        perror("bind");
        exit(1);
    }

    if (listen(server, 5) == -1) {//Gör servern till en listen socket
        perror("listen");
        exit(1);
    }

    clientData.currentClient=0;
    int a;
    clientData.readyPlayers=0;
    for(a=0;a<MAX_PLAYER;a++) // initialiserar
    {
        clientData.clientControl[a]=0;
        clientData.Data[a].buffer[0]='\0';
        clientData.readyArray[a]=0;

    }
    for(a=0;a<100;a++) // initialiserar
        arrayOfAis[a].timeOfAttack=0;

    pthread_mutex_init(&iMutex,NULL);

    int i,t;
    next = 0;
    int slotsAvailable = 1;
    pthread_create(&sendThread,NULL,&send_to_all_clients,(void *)&clientData); // tråd som skickar vidare det den får in
    pthread_t aiThread;
    pthread_create(&aiThread,NULL,&aiHandler,(void *)&clientData); // tråd som tar hand om ai
    strcpy(clientData.lobbyString,"srvbusy");
    pthread_mutex_init(&sendLock,NULL);

    for(;;) {
        printf("Waiting for a connection...\n");
        int ersize = sizeof(clientData.error_address);

        slotsAvailable=0;
        for(i=0;i<MAX_PLAYER;i++){ // om den hittar en ledig slot så kommer den låta folk connecta på den
            if(clientData.clientControl[i]==0){
                slotsAvailable=1;
                break;
            }
        }
            if(slotsAvailable==0){ // hantera om servern är  full
                printf("Server full\n");
                    if((clientData.errorHandle = accept(server, (struct sockaddr *)&clientData.error_address, &t)) == -1) {
                        perror("accept");
                    }
                    if (send(clientData.errorHandle, clientData.errorString, sizeof(clientData.errorString), 0) < 0) {
                        perror("send");
                    }
                    if(close(clientData.errorHandle)==-1){
                        perror("send");
                    }
                    for(i=0;i<MAX_PLAYER;i++)
                    {
                        if((clientData.clientControl[i])==0){
                            next = i;
                            printf("inside if \n");
                            break;
                        }
                    }
                }
        if(slotsAvailable==1){
            if(flag==1){ // uppdatera så den tar den lägsta lediga platsen
                next = nextExit;
                flag = 0;
            }
            printf("The handle i actually accept: %d\n",next);
            t = sizeof(clientData.client_address[next]);
            if((clientData.client[next] = accept(server, (struct sockaddr *)&clientData.client_address[next], &t)) == -1) { //Acceptera en klinet
                    perror("accept");
                }
                else{
                    if(clientData.lobbyMode==0){ // om servern redan spelar så får klienten som försöker connecta ett felmeddelande om det.
                        if (send(clientData.client[next], clientData.lobbyString, sizeof(clientData.lobbyString), 0) < 0) {
                            perror("send");
                        }
                        if(close(clientData.client[next])==-1){
                            perror("send");
                        }
                        else printf("busy kick\n");
                    }
                    else{ // annars kör den på som vanligt, uppdaterar några variabler och så sen skapar en tråd för klienten.
                        pthread_mutex_lock(&iMutex);
                        printf("1next %d & curclient %de\n",next,clientData.currentClient);
                        for(i=0;i<MAX_PLAYER;i++) // håller reda på vilken klient den behandlar(egetnlien ondöigt)
                        {
                            if((clientData.clientControl[i])==0){
                                clientData.currentClient = next;
                                printf("inside if \n");
                                break;
                            }
                        }
                        printf("2next %d & curclient %d\n",next,clientData.currentClient);
                        clientData.clientControl[clientData.currentClient]=1;
                        for(i=0;i<MAX_PLAYER;i++) // uppdaterar vilka nästa lediga plats är
                            {
                                if((clientData.clientControl[i])==0){
                                    next = i;
                                    printf("inside if \n");
                                    break;
                                }
                            }
                        printf("3next %d & curclient %d\n",next,clientData.currentClient);
                        pthread_create(&clientData.clientThread[clientData.currentClient],NULL,&send_recv,(void *)&clientData); //Slapar en tråd som kommer ta hand om denna klient
                    }
                }
            }
        }
            printf("close\n" );
    close(server);
  return 0;
}
