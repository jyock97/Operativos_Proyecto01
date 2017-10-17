#ifndef _mythread_h
#define _mythread_h

#include <ucontext.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define NUMTHREADS 100
#define STACKSIZE 4096
#define INTERVAL 100

sigset_t set;
ucontext_t signal_context;
void *signal_stack;

ucontext_t threads[NUMTHREADS];                                 //estructura que guarda los hilos
int tickets[NUMTHREADS];                                        //estructura que guarda tiquetes
int priority[NUMTHREADS], priority_aux[NUMTHREADS], position;   //estructuras para prioridad de tiempo real
int deadThreads[NUMTHREADS];                                    //hilos terminados
int curcontext;                                                 //hilo actual
ucontext_t *current_thread;                                     //contexto actual
ucontext_t exitContext;                                         //contexto que termina un hilo

int init;
int active_threads, active_threads_aux, total_tickets;
int active_sched;

static void executeExitContext();

static void setExitContext();

void setup();

void my_thread_end();

void my_thread_yield();

void my_thread_create(void *function,void *args, int tickets_sched, int priority_sched);

#endif
