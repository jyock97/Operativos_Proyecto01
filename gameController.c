#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gameController.h"
#include "netController.h"
#include "scheduler.h"
#include "my_thread.h"
#include "my_mutex.h"

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


int cardSelection = 1;
int xMenu = 0;
int yMenu = 0;
int selectedCard;
int selectedX;
int selectedY;
int bPlayer2 = 1;
int bFinishGame = 0;
struct warrior *field[ROWS][COLUMS]; //se debe cambiar esto para que se guarde la referencia el luchador o la torre

struct warrior T1; //{type, lvl, life, atack, direction, x, y, size, pWarrior}
struct warrior T2;
struct warrior T3;

struct warrior hand[HAND_SIZE];


my_mutex fieldLock;

void endGame(){
    bFinishGame = 1;
}
void setBPlayer2(int b){
    bPlayer2 = b;
}

void clearField(){
    for (size_t i = 0; i < ROWS; i++) {
        for (size_t j = 0; j < COLUMS; j++) {
            field[i][j] = NULL;
        }
    }
}

void printField(){
    my_mutex_lock(&fieldLock);
    for (size_t i = 0; i < ROWS; i++) {
        for (size_t j = 0; j < COLUMS; j++) {
            if(field[i][j]){
                if(field[i][j] -> type == 'T')
                    printf("TT");
                else
                    printf("%c%c", field[i][j] -> type, field[i][j] -> orientation);
            }else{
                printf("  ");
            }
        }
        if(i == yMenu)
            printf("<");
        printf("\n\r");
        for (size_t j = 0; j < COLUMS; j++) {
            if(!field[i][j]){
                printf("__");
            }else{
                printf("%c%c", (field[i][j] -> life / 10) + '0', (field[i][j] -> life % 10) + '0');
            }
        }
        if(i == yMenu)
            printf("<");
        printf("\n\r");
    }
    my_mutex_unlock(&fieldLock);
    if(cardSelection){      //Imprimir las cartas de la mano
        for (size_t i = 0; i < HAND_SIZE; i++) {
            printf("|%c%d", hand[i].type, hand[i].lvl);
        }
        printf("| Tipo, Nivel\r\n");
        for (size_t i = 0; i < HAND_SIZE; i++) {
            printf("|%c%c", (hand[i].life / 10) + '0', (hand[i].life % 10) + '0');
        }
        printf("| Vida\r\n");
        for (size_t i = 0; i < HAND_SIZE; i++) {
            printf("|%c%c", (hand[i].atack / 10) + '0', (hand[i].atack % 10) + '0');
        }
        printf("| Ataque\r\n");
    }
    for (size_t i = 0; i < COLUMS; i++) {
        if(cardSelection)
        printf(" ");
        if(i == xMenu)
        printf("^^");
        else
        printf("  ");
    }
    printf("\r\n");
}

void nextWarrior(struct warrior *warr){
    warr -> type = 'A';
    warr -> life = 5;
    warr -> atack = 5;
    warr -> direction = -1;
}

void startHand(){
    for (size_t i = 0; i < HAND_SIZE; i++) {
        nextWarrior(&hand[i]);
        // printf("%d, %d, %d\n", hand[i].life, hand[i].atack, hand[i].direction);
        // exit(0);
    }
}



void startTowers(){
    struct warrior *T1 = calloc(1, sizeof(struct warrior));
    struct warrior *T2 = calloc(1, sizeof(struct warrior));
    struct warrior *T3 = calloc(1, sizeof(struct warrior));
    T1 -> bPlayer2 = bPlayer2;
    T2 -> bPlayer2 = bPlayer2;
    T3 -> bPlayer2 = bPlayer2;
    if(bPlayer2){
        T1 -> type = 'T'; T1 -> life = 25;
        field[1][11] = T1;
        T2 -> type = 'T'; T2 -> life = 50;
        field[3][14] = T2;
        field[4][14] = T2;
        T3 -> type = 'T'; T3 -> life = 25;
        field[6][11] = T3;
    }else{
        T1 -> type = 'T'; T1 -> life = 25;
        field[1][4] = T1;
        T2 -> type = 'T'; T2 -> life = 50;
        field[3][1] = T2;
        field[4][1] = T2;
        T3 -> type = 'T'; T3 -> life = 25;
        field[6][4] = T3;
    }
}

void *gameController(){
    my_mutex_init(&fieldLock);
    clearField();
    startHand();
    startTowers();
    while (!bFinishGame) {
        system("clear");
        printField();
        usleep(250000); //milisegundos
    }
    my_thread_end();
}
void *arg;
void *warriorController(){

    int bDestroy;
    int xDirection;
    int yDirection;
    int currentX;
    int currentY;
    int nextX;
    int nextY;
    int currentTopTowerX;
    int currentMidTowerX;
    struct warrior *w = arg;

    bDestroy = 0;
    xDirection = w -> direction;
    currentX = w -> x;
    currentY = w -> y;
    while(!bDestroy){
        sleep(1);
        if(w -> life <= 0){
            bDestroy = 1;
            my_mutex_lock(&fieldLock);
            field[currentY][currentX] = NULL;
            my_mutex_unlock(&fieldLock);
            free(w);
            break;
        }
        //calcular la siguiente posicion
        if(bPlayer2){
            currentTopTowerX = TOP_TOWER_X_P2;
            currentMidTowerX = MID_TOWER_X_P2;
        }else{
            currentTopTowerX = TOP_TOWER_X_P1;
            currentMidTowerX = MID_TOWER_X_P1;
        }
        if(currentX <= currentTopTowerX){ //si estoy antes de las primeras torres
            if(currentY == TOP_TOWER_Y || currentY == BOT_TOWER_Y ||
            currentY == MID_TOWER_Y || currentY == (MID_TOWER_Y + 1)){

                int temp = (BOT_TOWER_Y - currentY) - (currentY - TOP_TOWER_Y);
                if(temp > 0){
                    nextY = currentY - 1;
                }else{
                    nextY = currentY + 1;
                }
                nextX = currentX;

            }else{
                nextX = currentX + xDirection;
                nextY = currentY;
            }
        }
        else if(currentY != TOP_TOWER_Y && currentY != BOT_TOWER_Y){

            //exit(0);
            int temp = (BOT_TOWER_Y - currentY) - (currentY - TOP_TOWER_Y);
            if(temp > 0){
                if(TOP_TOWER_Y > currentY){
                    nextY = currentY + 1;
                }else{
                    nextY = currentY - 1;
                }
            }else{
                if(BOT_TOWER_Y > currentY){
                    nextY = currentY + 1;
                }else{
                    nextY = currentY - 1;
                }
            }
            nextX = currentX;
        }else if(currentX != currentMidTowerX){
            nextX = currentX + xDirection;
            nextY = currentY;
        }else{
            nextX = currentX;
            if(MID_TOWER_Y > currentY){
                nextY = currentY + 1;
            }else{
                nextY = currentY - 1;
            }
        }
        my_mutex_lock(&fieldLock);
        if(field[nextY][nextX]){
            if(field[nextY][nextX] -> bPlayer2 != bPlayer2){
                //atack
            }
        }else{
            field[currentY][currentX] = NULL;
            field[nextY][nextX] = w;
            currentX = nextX;
            currentY = nextY;
        }
        my_mutex_unlock(&fieldLock);
    }
    my_thread_end();
}

void upMenu(){
    yMenu += (ROWS - 1);
    yMenu %= ROWS;
}
void downMenu(){
    yMenu ++;
    yMenu %= ROWS;
}
void leftMenu(){
    if(cardSelection){
        xMenu += (HAND_SIZE - 1);
        xMenu %= HAND_SIZE;
    }else{
        xMenu += (COLUMS - 1);
        xMenu %= COLUMS;
    }
}
void rigthMenu(){
    if(cardSelection){
        xMenu++;
        xMenu %= HAND_SIZE;
    }else{
        xMenu ++;
        xMenu %= COLUMS;
    }
}
void selectMenu(){

    if(cardSelection){
        selectedCard = xMenu;
    }else{
        selectedX = xMenu;
        selectedY = yMenu;
        if(selectedX == 11 && selectedY == 1 || selectedX == 11 && selectedY == 6 ||
        selectedX == 4 && selectedY == 1 || selectedX == 4 && selectedY == 6 ||
        selectedX == 1 && selectedY == 3 || selectedX == 1 && selectedY == 4 ||
        selectedX == 14 && selectedY == 3 || selectedX == 14 && selectedY == 4){
            if(bPlayer2){
                selectedX--;
                selectedY--;
            }else{
                selectedX++;
                selectedY++;
            }
        }
        my_mutex_lock(&fieldLock);
        while(field[selectedY][selectedX]){
            my_mutex_unlock(&fieldLock);
            my_thread_yield();
            my_mutex_lock(&fieldLock);
        }
        field[selectedY][selectedX] = calloc(1, sizeof(struct warrior));
        field[selectedY][selectedX] -> type = hand[selectedCard].type;
        field[selectedY][selectedX] -> lvl = hand[selectedCard].lvl;
        field[selectedY][selectedX] -> life = hand[selectedCard].life;
        field[selectedY][selectedX] -> atack = hand[selectedCard].atack;
        field[selectedY][selectedX] -> x = selectedX;
        field[selectedY][selectedX] -> y = selectedY;
        field[selectedY][selectedX] -> bPlayer2 = bPlayer2;
        if(bPlayer2){
            field[selectedY][selectedX] -> orientation = '<';
            field[selectedY][selectedX] -> direction = 1;
        }else{
            field[selectedY][selectedX] -> orientation = '>';
            field[selectedY][selectedX] -> direction = 1;
        }
        my_mutex_unlock(&fieldLock);
        arg = field[selectedY][selectedX];
        my_thread_create(warriorController,1,1);
    }
    cardSelection++;
    cardSelection %= 2;
    if(cardSelection){
        xMenu = 0;
    }
}
