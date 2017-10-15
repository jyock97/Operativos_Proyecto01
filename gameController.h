#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>
#include "netController.h"
#include "genetico.h"

#define ROWS    8
#define COLUMS  16
#define HAND_SIZE 8
#define TOP_TOWER_Y 1
#define BOT_TOWER_Y 6
#define TOP_TOWER_X_P1 4
#define TOP_TOWER_X_P2 11
#define MID_TOWER_X_P1 1
#define MID_TOWER_X_P2 14
#define MID_TOWER_Y 3
#define MSG_LEN 10

struct warrior{
    char type;
    int intType;
    char orientation;
    int lvl;
    int life;
    int attack;
    int direction;
    int x, y;
    int bPlayer2;
};

void setBPlayer2(int b);
void *gameController();
void spawnWarrior(char type, int lvl, int life, int attack, int x, int y, int bPlayer2);
void upMenu();
void downMenu();
void leftMenu();
void rigthMenu();
void selectMenu();
void endGame();

#endif
