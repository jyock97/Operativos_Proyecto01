#ifndef _SOCKET_H
#define _SOCKET_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

struct sockaddr_in server, client;
int port;
//----- FUNCIONES DEL SERVER -----//
int createSocket();						//Se hace un nuevo server
void bindSocket(int socket, int port);	//Se le asigna el puerto al server
//----- FUNCIONES DEL SERVER -----//

#endif