#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>


int sock, newSock, useSock;  //file descriptor del socket
int clientLen;
int port = 5000;             //numero del puerto
int charReadWriteSize;
char *ipAddr;
char buffer[256];

struct sockaddr_in serv_addr, cli_addr; //direccion del servidor y el cliente
struct hostent *server;

int retval;
fd_set fdList;
struct timeval tv;

void error(char *msg){
  perror(msg);
  exit(1);
}

void initServer(){

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)   //error creando el socket
    error("ERROR opening socket");

  memset(buffer, '0', sizeof(buffer));

   port = 5000;  //////////////////////////////////////////////cambiar despues

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);   //direccion ip de la maquina
  serv_addr.sin_port = htons(port);

  bind(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

  listen(sock,2);     //listo para recibir conexiones

  clientLen = sizeof(cli_addr);
  newSock = accept(sock, (struct sockaddr*) &cli_addr, &clientLen);
  if(newSock < 0)
    error("ERROR on accept");

  useSock = newSock;

  /*fflush(stdin);
  charReadWriteSize = read(newSock, buffer, sizeof(buffer));*/
}

void initClient(){

  sock = socket(AF_INET, SOCK_STREAM,0);
  if (sock < 0)   //error creando el socket
    error("ERROR opening socket");

  printf("Ingrese el ip del servidor: ");
  scanf("%s\n", ipAddr);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(ipAddr);
  serv_addr.sin_port = htons(port);

  if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  useSock = sock;
  /*printf("Please enter the message: ");
  //bzero(buffer,256);
  fgets(buffer,255,stdin);
  charReadWriteSize = write(sock,buffer,strlen(buffer));

  if (charReadWriteSize < 0)
    error("ERROR writing to socket");*/
}

int main(){

  char c;

  c = 0;
  while(c != 'S' && c != 'C'){

    system("clear");
    printf("Jugar como servidor[S] o conectarse como cliente[C]\n");
    printf("[S/C]: ");
    scanf("%c", &c);
    c = toupper(c);

  }

  switch (c) {
    case 'S':
      initServer();
      break;

    case 'C':
      initClient();
      break;
  }

  FD_ZERO(&fdList);
  FD_SET(0, &fdList);
  FD_SET(useSock, &fdList);
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  while (1) {
    retval = select(FD_SETSIZE, &fdList, NULL, NULL, &tv);

    if(retval == 1){
      if(FD_ISSET (0, &fdList)){

        fgets(buffer, sizeof(buffer), stdin);
        write(useSock, buffer, sizeof(buffer));
      }
      if(FD_ISSET (useSock, &fdList)){
        if(read(useSock, buffer, sizeof(buffer)) > 0)
          printf("%s\n", buffer);
      }
    }
  }

  return 0;
}
