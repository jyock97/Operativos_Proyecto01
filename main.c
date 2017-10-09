#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "netController.h"
#include "gameController.h"
#include "scheduler.h"
#include "my_thread.h"
#include "my_mutex.h"

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
    my_thread_end();
}

int main(){
    //makeConnection();

    my_thread_create(gameController,1,1);
    //my_thread_create(netController,1,1);
    my_thread_create(input,1,3);
    my_thread_chsched("Real");
    run_threads();

    return 0;
}
