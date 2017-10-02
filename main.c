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
char ipAddr[16];
char clientIP[16];
char buffer[256];

struct sockaddr_in servAddr, cliAddr; //direccion del servidor y el cliente
struct hostent *server;

int retval;
struct timeval timeOut;
fd_set fdList;


void error(char *msg){
  perror(msg);
  exit(1);
}

void initServer(){

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)   //error creando el socket
    error("ERROR opening socket");
  printf("Socket creando\n");

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = INADDR_ANY;   //direccion ip de la maquina
  servAddr.sin_port = htons(port);

  retval = bind(sock, (struct sockaddr*) &servAddr, sizeof(servAddr));
  if(retval < 0)
    error("Error binding");
  printf("Binding hecho\n");

  listen(sock,2);     //listo para recibir conexiones

  clientLen = sizeof(cliAddr);
  newSock = accept(sock, (struct sockaddr*) &cliAddr, &clientLen);
  if(newSock < 0)
    error("ERROR on accept");

  inet_ntop(AF_INET, &(cliAddr.sin_addr), clientIP, sizeof(clientIP));
  printf("Se realiza la conexion con el cliente: %s\n", clientIP);
  useSock = newSock;

  /*charReadWriteSize = read(useSock, buffer, sizeof(buffer));
  if(charReadWriteSize > 0)
    printf("%s\n", buffer);
  }*/
}

void initClient(){

  sock = socket(AF_INET, SOCK_STREAM,0);
  if (sock < 0)   //error creando el socket
    error("ERROR opening socket");

  printf("Ingrese el ip del servidor: ");
  fgets(buffer, sizeof(buffer), stdin);
  strcpy(ipAddr, buffer);
  printf("%s\n", ipAddr);

  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = inet_addr(ipAddr);
  servAddr.sin_port = htons(port);

  if (connect(sock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
    error("ERROR connecting");

  printf("Se realiza conexion\n");
  useSock = sock;
  printf("Please enter the message: ");

  /*fgets(buffer,255,stdin);
  charReadWriteSize = write(sock,buffer,strlen(buffer));

  if (charReadWriteSize < 0)
    error("ERROR writing to socket");
  */
}

int main(){

  char c;

  c = 0;
  while(c != 'S' && c != 'C'){

    system("clear");
    printf("Jugar como servidor[S] o conectarse como cliente[C]\n");
    printf("[S/C]: ");
    fgets(buffer, sizeof(buffer), stdin);
    c = buffer[0];
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

  timeOut.tv_sec = 2;
  timeOut.tv_usec = 0;
  FD_ZERO(&fdList);
  while (1) {

    FD_SET(0, &fdList);
    FD_SET(useSock, &fdList);
    retval = select(FD_SETSIZE, &fdList, NULL, NULL, &timeOut);
    //printf("%d\n", retval);
    if(retval == 1){
      if(FD_ISSET (0, &fdList)){
        fgets(buffer, sizeof(buffer), stdin);
        write(useSock, buffer, sizeof(buffer));
      }
      if(FD_ISSET (useSock, &fdList)){
        if(read(useSock, buffer, sizeof(buffer)) > 0){
          printf("MSG: %s\n", buffer);
        }
      }
    }
    printf("1");
  }

  return 0;
}
