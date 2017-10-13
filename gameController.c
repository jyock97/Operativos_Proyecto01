#include "gameController.h"
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
#define MSG_LEN 9

int cardSelection = 1;
int xMenu = 0;
int yMenu = 0;
int selectedCard;
int selectedX;
int selectedY;
int bPlayer2 = 0;
int bFinishGame = 0;
char *msg;
struct warrior *field[ROWS][COLUMS]; //se debe cambiar esto para que se guarde la referencia el luchador o la torre

struct warrior T1; //{type, lvl, life, atack, direction, x, y, size, pWarrior}
struct warrior T2;
struct warrior T3;

struct warrior hand[HAND_SIZE];

pthread_mutex_t fieldLock;

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
    pthread_mutex_lock(&fieldLock);
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
    pthread_mutex_unlock(&fieldLock);
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
    warr = setNewWarrior(shed[0]);
    warr -> type = 'A';
    warr -> direction = -1;
    warr -> lvl = (warr -> atack+warr -> life)/20;
}

void startHand(){
    initShed();
    for (int i = 0; i < HAND_SIZE; i++) {
        nextWarrior(&hand[i]);
        //printf("%d, %d, %d\n", hand[i].life, hand[i].atack, hand[i].direction);
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
    pthread_mutex_init(&fieldLock, NULL);
    clearField();
    startHand();
    startTowers();
    msg = calloc(MSG_LEN, sizeof(char));
    while (!bFinishGame) {
        system("clear");
        printField();
        usleep(250000); //milisegundos
    }
    pthread_mutex_destroy(&fieldLock);
}


void *warriorController(void *arg){

    int bDestroy;
    int bEnemy;
    int xDirection;
    int yDirection;
    int currentX;
    int currentY;
    int nextX;
    int nextY;
    int currentTopTowerX;
    int enemyMidTowerX;
    struct warrior *w = arg;

    bDestroy = 0;
    xDirection = w -> direction;
    currentX = w -> x;
    currentY = w -> y;

    if(w -> bPlayer2){
        enemyMidTowerX = MID_TOWER_X_P2;
    }else{
        enemyMidTowerX = MID_TOWER_X_P1;
    }

    while(!bDestroy){
        sleep(1);
        if(w -> life <= 0){
            bDestroy = 1;
            addWarrior(w->intType, w);
            pthread_mutex_lock(&fieldLock);
            field[currentY][currentX] = NULL;
            pthread_mutex_unlock(&fieldLock);
            //free(w);
            break;
        }
        //calcular la siguiente posicion
        if(bPlayer2 != w -> bPlayer2)
            bEnemy = 1;

        if(!bEnemy){
            if(bPlayer2? currentX >= TOP_TOWER_X_P2 : currentX <= TOP_TOWER_X_P1){ //si estoy antes de las primeras torres
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
            }else{
                nextX = currentX + xDirection;
                nextY = currentY;
            }
        }else{
            if((bPlayer2)? currentX < MID_TOWER_X_P2 : currentX > MID_TOWER_X_P1){
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
        }
        pthread_mutex_lock(&fieldLock);

        if(nextX < 0 || nextX > COLUMS - 1){
            field[currentY][currentX] = NULL;
            msg[0] = 'C';
            msg[1] = w -> type;
            msg[2] = w -> lvl + '0';
            msg[3] = (w -> life / 10) + '0';
            msg[4] = (w -> life % 10) + '0';
            msg[5] = (w -> atack / 10) + '0';
            msg[6] = (w -> atack % 10) + '0';
            msg[7] = currentY + '0';
            msg[8] = 0;
            sedMessage(msg); //FUNCION|typo|lvl|life|atack|y
        }else if(field[nextY][nextX]){
            if(field[nextY][nextX] -> bPlayer2 != bPlayer2){
                field[nextY][nextX] -> life -= w -> atack;
            }
        }else{
            field[currentY][currentX] = NULL;
            field[nextY][nextX] = w;
            currentX = nextX;
            currentY = nextY;
        }

        pthread_mutex_unlock(&fieldLock);
    }
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
        pthread_t *pWarr = malloc(sizeof(pthread_t));
        pthread_mutex_lock(&fieldLock);
        while(field[selectedY][selectedX]){
            pthread_mutex_unlock(&fieldLock);
            sched_yield();
            pthread_mutex_lock(&fieldLock);
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
            field[selectedY][selectedX] -> direction = -1;
        }else{
            field[selectedY][selectedX] -> orientation = '>';
            field[selectedY][selectedX] -> direction = 1;
        }
        pthread_mutex_unlock(&fieldLock);
        pthread_create(pWarr, NULL, warriorController, (void *) field[selectedY][selectedX]);
        nextWarrior(&hand[selectedCard]);

    }
    cardSelection++;
    cardSelection %= 2;
    if(cardSelection){
        xMenu = 0;
    }
}
