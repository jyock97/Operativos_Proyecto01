#include "gameController.h"
#include "mythreads/my_mutex.h"
#include "mythreads/my_thread.h"
#include "mythreads/scheduler.h"

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

struct warrior *T1; //{type, lvl, life, attack, direction, x, y, size, pWarrior}
struct warrior *T2;
struct warrior *T3;

struct warrior *hand[HAND_SIZE];

int towers = 0;

my_mutex fieldLock;

void *towerController(void *arg);

void endGame(){
    bFinishGame = 1;
}

void loose(){

    msg[0] = 'L';
    msg[1] = 0;
    sedMessage(msg);
    endGame();
    printFinish(0);
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
                if(field[i][j] -> life>99){
                    printf("99");
                }else{
                    if(field[i][j] -> life<10){
                        printf("0%d",(field[i][j] -> life));
                    }else{
                        printf("%d", (field[i][j] -> life));
                    }
                }
            }
        }
        if(i == yMenu)
            printf("<");
        printf("\n\r");
    }
    my_mutex_unlock(&fieldLock);
    if(cardSelection){      //Imprimir las cartas de la mano
        for (size_t i = 0; i < HAND_SIZE; i++) {
            printf("|%c%d", hand[i] -> type, hand[i] -> lvl);
        }
        printf("| Tipo, Nivel\r\n");
        for (size_t i = 0; i < HAND_SIZE; i++) {
            if(hand[i] -> life>99){
                printf("|99");
            }else{
                if(hand[i] -> life<10){
                    printf("|0%d",hand[i] -> life);
                }else{
                    printf("|%d", (hand[i] -> life));
                }
            }
        }
        printf("| Vida\r\n");
        for (size_t i = 0; i < HAND_SIZE; i++) {
            if(hand[i] -> attack>99){
                printf("|99");
            }else{
                if(hand[i] -> attack<10){
                    printf("|0%d",(hand[i] -> attack));
                }else{
                    printf("|%d", (hand[i] -> attack));
                }
            }
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

void printFinish(int bWinner){
    system("clear");
    printf("/--------------------------/\n\r");
    printf("/--------------------------/\n\r");
    printf("/--------------------------/\n\r");
    printf("/--------------------------/\n\r");

    if(bWinner){
        printf("/----------Winner----------/\n\r");
    }else{
        printf("/----------Looser----------/\n\r");
    }

    printf("/--------------------------/\n\r");
    printf("/--------------------------/\n\r");
    printf("/--------------------------/\n\r");
    printf("/--------------------------/\n\r");
}

void nextWarrior(struct warrior *warr){

    int pos = rand()%10;
    struct warrior *tempWarrior = setNewWarrior(shed[pos]);
    warr -> type = pos + 'A';
    warr -> intType = tempWarrior -> intType;
    warr -> lvl = tempWarrior -> lvl;
    warr -> life = tempWarrior -> life;
    warr -> attack = tempWarrior -> attack;
}

void startHand(){

    for (int i = 0; i < HAND_SIZE; i++) {
        hand[i] = calloc(1, sizeof(struct warrior));
        nextWarrior(hand[i]);
    }
}

void startTowers(){

    T1 = calloc(1, sizeof(struct warrior));
    T2 = calloc(1, sizeof(struct warrior));
    T3 = calloc(1, sizeof(struct warrior));
    T1 -> bPlayer2 = bPlayer2;
    T2 -> bPlayer2 = bPlayer2;
    T3 -> bPlayer2 = bPlayer2;

    T1 -> type = 'T'; T1 -> life = 99; T1 -> cTower = 0;
    T2 -> type = 'T'; T2 -> life = 99; T2 -> cTower = 1;
    T3 -> type = 'T'; T3 -> life = 99; T3 -> cTower = 0;

    if(bPlayer2){

        T1 -> x = 11; T1 -> y = 1;
        T2 -> x = 14; T2 -> y = 3;
        T3 -> x = 11; T3 -> y = 6;
    }else{

        T1 -> x = 4; T1 -> y = 1;
        T2 -> x = 1; T2 -> y = 3;
        T3 -> x = 4; T3 -> y = 6;
    }
    my_thread_create(towerController, (void *) T1,5,5);
    my_thread_create(towerController, (void *) T2,5,5);
    my_thread_create(towerController, (void *) T3,5,5);
}


void *gameController(){
    my_mutex_init(&fieldLock);
    clearField();
    startHand();
    startTowers();
    msg = calloc(MSG_LEN, sizeof(char));
    while (!bFinishGame) {
        system("clear");
        printField();
        usleep(250000); //milisegundos
    }
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
    struct warrior *w = arg;

    bDestroy = 0;
    xDirection = w -> direction;
    currentX = w -> x;
    currentY = w -> y;

    if(bPlayer2 != w -> bPlayer2)
    bEnemy = 1;

    while(!bDestroy && !bFinishGame){
        sleep(1);
        if(w -> life <= 0){
            bDestroy = 1;
            addWarrior(w->intType, w);
            my_mutex_lock(&fieldLock);
            field[currentY][currentX] = NULL;
            my_mutex_unlock(&fieldLock);
            //free(w);
            break;
        }
        //calcular la siguiente posicion

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
        my_mutex_lock(&fieldLock);

        if(nextX < 0 || nextX > COLUMS - 1){
            field[currentY][currentX] = NULL;
            msg[0] = 'C';
            msg[1] = w -> type;
            msg[2] = w -> lvl + '0';
            msg[3] = (w -> life / 10) + '0';
            msg[4] = (w -> life % 10) + '0';
            msg[5] = (w -> attack / 10) + '0';
            msg[6] = (w -> attack % 10) + '0';
            msg[7] = currentY + '0';
            msg[8] = bPlayer2 + '0';
            msg[9] = 0;
            sedMessage(msg); //FUNCION|typo|lvl|life|attack|y|bPlayer2
            bDestroy = 1;
            field[currentY][currentX] = NULL;

        }else if(field[nextY][nextX]){
            if(field[nextY][nextX] -> bPlayer2 != w -> bPlayer2){
                field[nextY][nextX] -> life -= w -> attack;
                if(field[nextY][nextX] < 0)
                    field[nextY][nextX] = 0;
            }
        }else{
            field[currentY][currentX] = NULL;
            field[nextY][nextX] = w;
            currentX = nextX;
            currentY = nextY;
        }
        my_mutex_unlock(&fieldLock);
    }
}

void spawnWarrior(char type, int lvl, int life, int attack, int x, int y, int bPlayer2){

    pthread_t *pWarr = malloc(sizeof(pthread_t));
    my_mutex_lock(&fieldLock);
    while(field[y][x]){
        my_mutex_unlock(&fieldLock);
        my_thread_yield();
        my_mutex_lock(&fieldLock);
    }

    field[y][x] = calloc(1, sizeof(struct warrior));
    field[y][x] -> type = type;
    field[y][x] -> lvl = lvl;
    field[y][x] -> life = life;
    field[y][x] -> attack = attack;
    field[y][x] -> x = x;
    field[y][x] -> y = y;
    field[y][x] -> bPlayer2 = bPlayer2;
    if(bPlayer2){
        field[y][x] -> orientation = '<';
        field[y][x] -> direction = -1;
    }else{
        field[y][x] -> orientation = '>';
        field[y][x] -> direction = 1;
    }
    my_mutex_unlock(&fieldLock);
    my_thread_create(warriorController,(void *) field[y][x],5,1);
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
        char type = hand[selectedCard] -> type;
        int lvl = hand[selectedCard] -> lvl;
        int life = hand[selectedCard] -> life;
        int attack = hand[selectedCard] -> attack;
        int x = selectedX;
        int y = selectedY;

        nextWarrior(hand[selectedCard]);
        spawnWarrior(type, lvl, life, attack, x, y, bPlayer2);

    }
    cardSelection++;
    cardSelection %= 2;
    if(cardSelection){
        xMenu = 0;
    }
}

void *towerController(void *arg){

    struct warrior *t = arg;
    int x, y;
    int bDestroy;

    x = t -> x;
    y = t -> y;
    my_mutex_lock(&fieldLock);
    field[y][x] = t;

    if(t -> cTower){
        field[y+1][x] = t;
    }
    my_mutex_unlock(&fieldLock);

    while(!bDestroy && !bFinishGame){
        sleep(1);
        if(t -> life <= 0){
            if(t -> cTower){
                loose();
            }else{
                my_mutex_lock(&fieldLock);
                field[y][x] = NULL;
                my_mutex_unlock(&fieldLock);
                free(t);
                bDestroy = 1;
            }
        }
    }
}
