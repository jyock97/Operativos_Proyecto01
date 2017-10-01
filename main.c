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


int sock, newSock;  //file descriptor del socket
int clientLen;
int port;             //numero del puerto
int charReadWriteSize;
char *ipAddr;
char buffer[256];

struct sockaddr_in serv_addr, cli_addr; //direccion del servidor y el cliente
struct hostent *server;

void error(char *msg){
  perror(msg);
  exit(1);
}

void initServer(){

  sock = socket(AF_INET, SOCK_STREAM,0);
  if (sock < 0)   //error creando el socket
    error("ERROR opening socket");

  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(buffer, '0', sizeof(buffer));

   port = 1234;  //////////////////////////////////////////////cambiar despues

  //bzero((char*) &serv_addr, sizeof(serv_addr));   //inicializar en 0

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);   //direccion ip de la maquina
  serv_addr.sin_port = htons(port);

  if (!bind(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)))
    error("ERROR on binding");


  listen(sock,2);     //listo para recibir conexiones

  clientLen = sizeof(cli_addr);
  newSock = accept(sock, (struct sockaddr*) &cli_addr, &clientLen);
  if(newSock < 0)
    error("ERROR on accept");


  //bzero(buffer, sizeof(buffer));
  charReadWriteSize = read(newSock, buffer, sizeof(buffer));
  if(charReadWriteSize < 0)
    error("ERROR reading from socket");

  printf("%s\n", buffer);
}

void initClient(){

  sock = socket(AF_INET, SOCK_STREAM,0);
  if (sock < 0)   //error creando el socket
    error("ERROR opening socket");

  port = 1234;
  ipAddr = "127.0.0.1";
  server = gethostbyname("Cruciatus");
  if(!server)
    error("ERROR no host");

  //bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(ipAddr);
  serv_addr.sin_port = htons(port);

  if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  printf("Please enter the message: ");
  //bzero(buffer,256);
  fgets(buffer,255,stdin);
  charReadWriteSize = write(sock,buffer,strlen(buffer));

  if (charReadWriteSize < 0)
    error("ERROR writing to socket");
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

  return 0;
}
