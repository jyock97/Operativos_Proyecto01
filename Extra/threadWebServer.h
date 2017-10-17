#ifndef _THREADWEBSERVER_H
#define _THREADWEBSERVER_H

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "my_mutex.h"
#define MAX_CONNECTIONS 100
#define true 1
#define false 0

int *processPool;
int prin_socket;
int current_threads;
int new_socket;
char *current_path;

my_mutex mutex;
my_mutex number_mutex;
//pthread_cond_t condition;
//pthread_cond_t accept_condition;
//pthread_mutex_t mutex;
//pthread_mutex_t number_mutex;

//----- FUNCIONES PARA HILOS -----//
void startThreads(); 	//Se crean los hilos y se asignan para las conexiones

void *threadHandler(); 			//
void createThreads(int max);	//Se crean los hilos
void listeningConnections();	//Se pone a escuchar conexiones el server
//----- FUNCIONES PARA HILOS -----//

#endif
