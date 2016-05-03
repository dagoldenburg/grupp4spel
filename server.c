/*
** echos.c -- the echo server for echoc->c; demonstrates unix sockets
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
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

#define maxplayer 2
__thread int threadLocal;
__thread int joinQuerry;
int flag;
int server;
//__thread char clientIp[INET_ADDRSTRLEN];
//char clientIpAddrRecv[INET_ADDRSTRLEN];
int previous;
char str[100];
int next,nextExit;
struct sockaddr_in server_address, client_address;
int t;

struct clientData
{
    pthread_t clientThread[maxplayer];
    int clientControl[maxplayer];
    int client[maxplayer];
    int currentClient;
};

pthread_mutex_t iMutex;


static void daemonize(void)
{
    pid_t pid, sid;

    /* already a daemon */
    if ( getppid() == 1 ) return;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then we can exit the parent process. */
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* At this point we are executing as the child process */

    /* Change the file mode mask */
   // umask(0);

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
       int pidlog=getpid();
    FILE *fp;
    fp = fopen("/tmp/pidlog.txt", "w" ); // Open file for writing
    fprintf(fp, "%d", pidlog);
    fclose(fp);

}
    int n,done;

 /*void* serverFullThread(void* serverFullMsg){
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
    serverFullMsg = 0;
    pthread_exit(NULL);
 }*/

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
        do {
            n = recv(c->client[threadLocal], str, 100, 0);
            if (n <= 0) {
                if (n < 0) {
                    printf("recv error\n");
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
                }
            if (!done)
                if (send(c->client[threadLocal], str, n, 0) < 0) {
                    perror("send");
                    done = 1;
                }
        } while (!done);
        return;
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

    }

    pthread_mutex_init(&iMutex,NULL);
    joinQuerry = -1;
    int i;
    next = 0;
    int slotsAvailable = 1;
    int serverFullMsg = 0;
    pthread_t serverFullHandle;
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
                            clientData.currentClient = i;
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

