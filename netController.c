#include "netController.h"
#include "gameController.h"
#include "mythreads/my_mutex.h"
#include "mythreads/my_thread.h"

int sock, newSock, useSock;  //file descriptor del socket
int clientLen;
int port = 5000;             //numero del puerto
int charReadWriteSize;
char ipAddr[16];
char clientIP[16];
char buffer[10];

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
    setBPlayer2(0);
}

void initClient(){

    sock = socket(AF_INET, SOCK_STREAM,0);
    if (sock < 0)   //error creando el socket
        error("ERROR opening socket");

    printf("Ingrese el ip del servidor: ");
    fgets(buffer, sizeof(buffer), stdin);
    strcpy(ipAddr, buffer);
    //printf("%s\n", ipAddr);

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(ipAddr);
    servAddr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
        error("ERROR connecting");

    printf("Se realiza conexion\n");
    useSock = sock;
    setBPlayer2(1);
}

void makeConnection(){

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
}

void *netController(){
    char function;
    char type;
    int lvl;
    int life;
    int attack;
    int x, y;
    int bPlayer2;
    while(1){
        if(read(useSock, buffer, sizeof(buffer)) > 0){
            //FUNCION|typo|lvl|life|attack|y|bPlayer2
            function = buffer[0];
            switch (function) {
                case 'C':
                    type = buffer[1];
                    lvl = buffer[2] - '0';
                    life = buffer[3] -'0';
                    life *= 10;
                    life += buffer[4] - '0';
                    attack = buffer[5] -'0';
                    attack *= 10;
                    attack += buffer[6] - '0';
                    y = buffer[7] - '0';
                    bPlayer2 = buffer[8] - '0';
                    if(bPlayer2)
                    x = COLUMS-1;
                    else
                    x = 0;
                    spawnWarrior(type, lvl, life, attack, x, y, bPlayer2);
                    //char type, int lvl, int life, int attack, int x, int y, int bPlayer2
                    break;

                case 'L':
                    endGame();
                    printFinish(1);
                    break;
            }
        }
    }
}

void sedMessage(char *msg) {
    write(useSock, msg, strlen(msg));
}
