#include "scheduler.h"
#include "my_thread.h"
#include "my_mutex.h"

my_mutex lock;

void thread1(){
    int id = 0;

    while(1) {
        if(!id){
            my_mutex_lock(&lock);
        }
        printf("hola %i\n", id++);
        sleep(1);
        my_mutex_unlock(&lock);
        //my_thread_end();
    };
}

void thread3(){
    int id = 0;
    while(1) {
        printf("hola2 %i\n", id++);
        sleep(1);
    };
}

void thread2(int a){
    int ax = (int)a;
    int id = 0;
    my_mutex_lock(&lock);
    printf("hola1 %i %i\n", ax,id++);
    my_thread_create(thread3,NULL,5,1);
    my_mutex_unlock(&lock);
}

//archivo de pruebas para los hilos
int main(){
    int i;
    int a = 23;
    my_mutex_init(&lock);
    my_thread_create(thread1,NULL,5,2);
    my_thread_create(thread2,(void*)a,5,3);
    my_thread_chsched("Real");
    run_threads();
    return 0;
}
