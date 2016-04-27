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

#define maxplayer 3
__thread int threadLocal;
__thread int joinQuerry;
//__thread char clientIp[INET_ADDRSTRLEN];
//char clientIpAddrRecv[INET_ADDRSTRLEN];

char str[100];
char str2[100];
int next;

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
 void* send_recv(void* clientData)
 {
    int i;
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
                    n = recv(c->client[threadLocal], str2, 100, 0);
                    c->currentClient = threadLocal;
                    printf("\n\n recv error");
                    printf("closing:%d\n",threadLocal);
                    if(close(c->client[threadLocal])==-1){

                        printf("Connected,client = %d.\n",c->client[threadLocal]);
                        printf("no close on:%d\n",threadLocal);
                    }
                }
                if (n == 0){
                    pthread_mutex_lock(&iMutex);
                    joinQuerry=threadLocal;
                    c->currentClient = threadLocal;
                    printf("closing:%d\n",c->client[threadLocal]);
                    if(close(c->client[threadLocal])==-1){

                        printf("no close on:%d\n",threadLocal);
                    }
                    c->clientControl[threadLocal]=0;
                    next = threadLocal;
                    printf("next i tråd: %d\n",next);
                    printf("not connected\n");
                    pthread_mutex_unlock(&iMutex);
                    pthread_exit(NULL);
                    return;
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
    int server,t, len,portno,optval,errorhej = 0;
    socklen_t optlen = sizeof(optval);
    struct clientData clientData;
    clientData.currentClient = 0;

    struct sockaddr_in server_address, client_address;
    //char str[100];

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
  /*  optval = 1;
    optlen = sizeof(optval);
    if(setsockopt(server,SOL_SOCKET,SO_KEEPALIVE,&optval,optlen)<0){
        perror("setsockopt()");
        close(server);
    }*/
    printf("Hej %d\n",errorhej++);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr=INADDR_ANY;
    server_address.sin_port=htons(atoi(argv[1]));
    len =sizeof(server_address);
  //strcpy(local.sun_path, SOCK_PATH);
  //unlink(local.sun_path);
  //len = strlen(local.sun_path) + sizeof(local.sun_family);
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
    for(;;) {
        printf("Waiting for a connection...\n");
        t = sizeof(client_address);

        /* code */

            for(i=0;i<maxplayer;i++)
            {
                if((clientData.clientControl[i])==0){
                    break;
                }
                else if(i==maxplayer-1 || clientData.clientControl[maxplayer-1]==1){
                    //server full
                    i=0;
                }
            }
        if((clientData.client[next] = accept(server, (struct sockaddr *)&client_address, &t)) == -1) {
            perror("accept");
            exit(1);
        }

        else{
            pthread_mutex_lock(&iMutex);
            printf("\n\n\nAccepting %d\n",next);


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
            printf("after %d token for client %d\n",clientData.clientControl[clientData.currentClient],clientData.currentClient);

            pthread_create(&clientData.clientThread[clientData.currentClient],NULL,&send_recv,(void *)&clientData);
            }
            /*if(joinQuerry<=0){
                pthread_join(clientData.clientThread[joinQuerry],NULL);
                joinQuerry=-1;
            }*/
        }
            printf("close\n" );
    close(server);
  return 0;
}




