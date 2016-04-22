/*
** echos.c -- the echo server for echoc.c; demonstrates unix sockets
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
#define maxplayer 2
__thread int threadLocal;
char str[100];

struct clientData
{
    pthread_t clientThread[maxplayer];
    int clientControl[maxplayer];
    int client[maxplayer];
    int currentClient;
};
pthread_mutex_t iMutex;

#define SERV_PORT 3000

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

 void* send_recv(void* clientData)
 {  
    printf("i funktionen\n");
    struct clientData* c=(struct clientData*) clientData;
    // threadlock
    threadLocal=c->currentClient;
    c->clientControl[c->currentClient]=1;
    c->currentClient=firstFreeSlot(c);
    pthread_mutex_unlock(&iMutex);

    int n, done;
        // thread release
    printf("Connected.\n");

    
        done = 0;
        do {
            n = recv(c->client[c->currentClient], str, 100, 0);
            if (n <= 0) {
                if (n < 0) perror("recv");
                done = 1;
            }

            if (!done) 
                if (send(c->client[c->currentClient], str, n, 0) < 0) {
                    perror("send");
                    done = 1;
                }
        } while (!done);
        c->clientControl[threadLocal]=0;
        //pthtread lock

        //i=firstFreeSlot();
        //thread release

 }
 int firstFreeSlot(struct clientData* clientData)
 {
    int i;
    for(i=0;i<maxplayer;i++)
    {
        if((clientData->clientControl[i])==0)
        {
            return i;
        }
    }
 }

int main(char argc ,char *argv[])
{
    int server,t, len,portno;
    struct clientData *clientData = malloc(sizeof(struct clientData)); 
    struct sockaddr_in server_address, client_address;
    //char str[100];

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }


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
    clientData->currentClient=0;
    int a;
    for(a=0;a<maxplayer;a++)
    {
        clientData->clientControl[a]=0;
        
    }

    pthread_mutex_init(&iMutex,NULL);

    for(;;) {
        printf("Waiting for a connection...\n");
        t = sizeof(client_address);
   
        /* code */
    
   

    
        if ((clientData->client[clientData->currentClient] = accept(server, (struct sockaddr *)&client_address, &t)) == -1) {
            perror("accept");
            exit(1);
        }
        else{

            pthread_mutex_lock(&iMutex);
            pthread_create(&clientData->clientThread[clientData->currentClient],NULL,&send_recv,(void *)clientData);
            
            pthread_mutex_lock(&iMutex);
            pthread_join(clientData->clientThread[threadLocal],NULL);
            clientData->clientControl[threadLocal]=0;
            clientData->currentClient=firstFreeSlot(clientData); 
            pthread_mutex_unlock(&iMutex);
            }
             
              //pthtread lock
                
                //thread release
                 
            }

    close(server);

  

  return 0;
}


