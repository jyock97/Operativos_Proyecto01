#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include <pthread.h>

struct warrior{
    char type;
    char orientation;
    int lvl;
    int life;
    int atack;
    int direction;
    int x, y;
    int bPlayer2;
    //int size;
    pthread_t pWarrior;
    //int scheduler;
};

void setBPlayer2(int b);
void *gameController();

void upMenu();
void downMenu();
void leftMenu();
void rigthMenu();
void selectMenu();
void endGame();

#endif