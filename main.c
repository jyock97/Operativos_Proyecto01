#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "netController.h"
#include "gameController.h"
#include "genetico.h"

int main(){

    char buffer[2];

    pthread_t pGameController;
    pthread_t pNetController;


    makeConnection();

    pthread_create(&pGameController, NULL, gameController, NULL);
    pthread_create(&pNetController, NULL, netController, NULL);

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
    //pthread_exit(&pGameController);
    //pthread_exit(&pNetController);

    return 0;
}
