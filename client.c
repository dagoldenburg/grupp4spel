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
#include<netinet/in.h>
#include<arpa/inet.h>

#define BUFFSIZE 4096
#define SOCK_PATH "/tmp/socket"
#define SERV_PORT 3232

int main(int argc ,char *argv[])
{

  int s, i, t,portno;
  struct sockaddr_in serv_addr;
  char str[100];
  if (argc < 2) {
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
    serv_addr.sin_port = htons(SERV_PORT);
  if (connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    perror("connect");
    exit(1);
  }

  printf("Connected.\n");

  
	while(printf("> "), fgets(str, 100, stdin), !feof(stdin)) {
        if (send(s, str, strlen(str), 0) == -1) {
            perror("send");
            exit(1);
        }

        if ((t=recv(s, str, 100, 0)) > 0) {
            str[t] = '\0';
            // if(strstr(str,"q")==0)
            // {
            //   exit(0);
            // }
            printf("echo> %s", str);

        } else {
            if (t < 0) perror("recv");
            else printf("Server closed connection\n");
            exit(1);
        }
    }

    close(s);


  

  return 0;
}



