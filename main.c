#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


int sock, newSock;  //file descriptor del socket
int port;             //numero del puerto
int charReadWriteSize;
char buffer[256];

struct sockaddr_in serv_addr, cli_addr; //direccion del servidor y el cliente
struct hostent *server;

void error(char *msg){
  perror(msg);
  exit(1);
}

void initServer(){

  int sock, newSock;  //file descriptor del socket
  int port;             //numero del puerto
  int charReadWriteSize;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr; //direccion del servidor y el cliente

  sock = socket(AF_INET, SOCK_STREAM,0);
  if (sock < 0)   //error creando el socket
    error("ERROR opening socket");


   port = 1234;  //////////////////////////////////////////////cambiar despues

  bzero((char*) &serv_addr, sizeof(serv_addr));   //inicializar en 0

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;   //direccion ip de la maquina
  serv_addr.sin_port = htons(port);

  if (!bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)))
    error("ERROR on binding");


  listen(sock,2);     //listo para recibir conexiones

  newSock = accept(sock, (struct sockaddr *) &cli_addr, sizeof(cli_addr));
  if(newSock < 0)
    error("ERROR on accept");


  bzero(buffer, sizeof(buffer));
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
  server = gethostbyname("Cruciatus");
  if(!server)
    error("ERROR no host");

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server -> h_addr, (char *) serv_addr.sin_addr.s_addr, server -> h_length);
  serv_addr.sin_port = htons(port);

  if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");

  printf("Please enter the message: ");
  bzero(buffer,256);
  fgets(buffer,255,stdin);
  charReadWriteSize = write(sockfd,buffer,strlen(buffer));

  if (n < 0)
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
