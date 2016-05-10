/*
** echoc.c -- the echo client for echos.c; demonstrates unix sockets
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#define BUFFSIZE 4096
#define SOCK_PATH "/tmp/socket"
#define SERV_PORT 3232
void *recvfunc(void *sock);

int main(int argc ,char *argv[])
{

  int s, i, t,portno;
  int counter = 0;
  struct sockaddr_in serv_addr;
  char str[100];
  char connectivityTest[100];
  strcpy(connectivityTest,"test\n");
 HERE: if (argc < 2) {
        perror("Usage _ tcp client <IP address of the server>");
       exit(1);
    }

    if ((s = socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
                perror("ERROR opening socket");
    }


  // memset(&serv_addr,0,sizeof(serv_addr));
   serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
if (connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    perror("connect");
    exit(1);
  }
  if (send(s, connectivityTest, strlen(connectivityTest), 0) == -1) {
            perror("send");
            exit(1);
        }
    if ((t=recv(s, str, 100, 0)) > 0) {
            str[t] = '\0';
             }
     if(strstr(str,"Server full\n")){
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
    pthread_t recvThread;
    pthread_create(&recvThread,NULL,recvfunc,(void *)&s);

	while(printf("> "), fgets(str, 100, stdin), !feof(stdin)) {
        if (send(s, str, strlen(str), 0) == -1) {
            perror("send");
            exit(1);
        }
//        if ((t=recv(s, str, 100, 0)) > 0) {
//            str[t] = '\0';
//            printf("echo> %s", str);
//
//        } else {
//            if (t <= 0) perror("recv");
//            else printf("Server closed connection\n");
//            exit(1);
//        }
    }
    close(s);
  return 0;
}

void *recvfunc(void *sock)
{
    int s = *(int*) sock;
    int t;
    char buffer[100];
        if ((t=recv(s, buffer, 100, 0)) > 0) {
            buffer[t] = '\0';

            printf("echo> %s", buffer);

        } else {
            if (t <= 0) perror("recv");
            else printf("Server closed connection\n");
            exit(1);
        }

}

