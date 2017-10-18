#ifndef NET_CONTROLLER_H
#define NET_CONTROLLER_H
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

/*
 * Funcion la cual es llamada para iniciar la conexion del juego.
 */
void makeConnection();

/*
 * Funcion la cual es utilizada por un hilo para controlar los mensajes
 * recividos y llamar a las funciones correspondientes.
 */
void *netController();

/*
 * Funcion la cual se encarga de enviar un mensaje al cliente o servidor
 * al cual este conectado.
 */
void sedMessage(char *msg);

#endif
