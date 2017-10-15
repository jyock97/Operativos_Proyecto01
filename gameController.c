#include "gameController.h"

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
    pthread_mutex_unlock(&fieldLock);
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

void nextWarrior(struct warrior *warr){
    srand(time(NULL));
    int pos = rand()%10;
    struct warrior *tempWarrior = setNewWarrior(shed[pos]);
    warr -> type = pos + 'A';
    warr -> intType = tempWarrior -> intType;
    warr -> lvl = tempWarrior -> lvl;
    warr -> life = tempWarrior -> life;
    warr -> attack = tempWarrior -> attack;
    printf("%c %d %d\n", warr -> type, warr -> life, warr -> attack);
}

void startHand(){
    initShed();
    for (int i = 0; i < HAND_SIZE; i++) {
        hand[i] = calloc(1, sizeof(struct warrior));
        nextWarrior(hand[i]);
        printf("%c %d %d\n", hand[i] -> type, hand[i] -> life, hand[i] -> attack);
    }
    for(int i = 0; i < HAND_SIZE; i++){
        printf("%c %d %d\n", hand[i] -> type, hand[i] -> life, hand[i] -> attack);
        printf("---\n");
    }exit(0);
}

void startTowers(){
    T1 = calloc(1, sizeof(struct warrior));
    T2 = calloc(1, sizeof(struct warrior));
    T3 = calloc(1, sizeof(struct warrior));
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
    struct warrior *w = arg;

    bDestroy = 0;
    xDirection = w -> direction;
    currentX = w -> x;
    currentY = w -> y;

    if(bPlayer2 != w -> bPlayer2)
    bEnemy = 1;

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
        pthread_mutex_lock(&fieldLock);

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
                if(field[nextY][nextX] -> type == 'T'){
                    free(field[nextY][nextX]);
                    field[nextY][nextX] = NULL;
                }
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

void spawnWarrior(char type, int lvl, int life, int attack, int x, int y, int bPlayer2){

    pthread_t *pWarr = malloc(sizeof(pthread_t));
    pthread_mutex_lock(&fieldLock);
    while(field[y][x]){
        pthread_mutex_unlock(&fieldLock);
        sched_yield();
        pthread_mutex_lock(&fieldLock);
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
    pthread_mutex_unlock(&fieldLock);
    pthread_create(pWarr, NULL, warriorController, (void *) field[y][x]);
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
