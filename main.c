#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "netController.h"
#include "gameController.h"
#include "genetico.h"
#include "mythreads/my_mutex.h"
#include "mythreads/my_thread.h"
#include "mythreads/scheduler.h"
void input(){
    char buffer[2];
    system ("/bin/stty raw"); //no esperar al enter para leer
    read(0, buffer, 2);
    while (buffer[0] != 'q') {
        //buffer[0] = \n <=> buffer[0] = 13

        switch (buffer[0]) {
            case 'w':
                upMenu();
                break;
            case 's':
                downMenu();
                break;
            case 'a':
                leftMenu();
                break;
            case 'd':
                rigthMenu();
                break;
            case 13:
                selectMenu();
        }
        read(0, buffer, 2);
    }
    system ("/bin/stty cooked");
    endGame();
    exit(0);
}

int main(int argc, char *argv[]){
    char *iniFile;

    if(argc < 2){
        printf("Se necesita un archivo de inicio\n");
        exit(0);
    }
    iniFile = argv[1];
    initShed(iniFile);

    srand(time(NULL));

    makeConnection();
    my_thread_create(gameController,NULL,5,5);
    my_thread_create(netController,NULL,5,5);
    my_thread_create(input,NULL,5,5);
    my_thread_chsched("Real");
    run_threads();
    return 0;
}
