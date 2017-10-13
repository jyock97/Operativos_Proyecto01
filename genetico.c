#include "genetico.h"
#include "gameController.h"

void addWarrior(int index, struct warrior *new){
    int temp = 1-position[index];
    position[index] = temp;
    shed[index][temp+1] = new;
}

struct warrior *setNewWarrior(struct warrior **poblacion){
    srand(time(NULL));
    int size = 2, gen = 5, index;
    int mutation = 80,random;
    struct warrior **parents = malloc(64*sizeof(struct warrior*));
    struct warrior **sons = malloc(64*sizeof(struct warrior*));
    while(gen--){
        parents = poblacion;
        index = 0;
        for (int i = 0; i < size; i++) {
            struct warrior *temp = malloc(sizeof(struct warrior));
            temp->atack = parents[i]->atack;
            temp->life = parents[i]->life;
            sons[index] = temp;
            index++;
            temp = malloc(sizeof(struct warrior));
            temp->atack = parents[i+1]->atack;
            temp->life = parents[i+1]->life;
            sons[index] = temp;
            index++;
            temp = malloc(sizeof(struct warrior));
            temp->atack = parents[i]->atack;
            temp->life = parents[i+1]->life;
            sons[index] = temp;
            index++;
            temp = malloc(sizeof(struct warrior));
            temp->atack = parents[i+1]->atack;
            temp->life = parents[i]->life;
            sons[index] = temp;
            index++;
            i++;
        }
        memset(poblacion,0,64);
        for (int i = 0; i < index; i++) {
            poblacion[i] = sons[i];
            random = rand()%101;
            if(random<mutation){
                poblacion[i]->atack +=2;
                poblacion[i]->life +=3;
            }
        }
        size*=2;
    }
    int lvl = poblacion[0]->atack+poblacion[0]->life, aux = 0;
    int pos = 0;
    for(int i = 1; i<size; i++){
        aux = poblacion[i]->atack+poblacion[i]->life;
        if(aux>lvl){
            lvl = aux;
            pos = i;
        }
    }
    poblacion[pos]->lvl = (poblacion[pos]->atack+poblacion[pos]->life)/10;
    return poblacion[pos];
}

void initShed(){
    struct warrior *primer = malloc(sizeof(struct warrior));
    struct warrior *segundo = malloc(sizeof(struct warrior));
    primer->life = 5;
    primer->atack = 10;
    segundo->life = 8;
    segundo->atack = 6;
    position[0] = 0;         //esto siempre inicia como 0
    shed[0][0] = primer;
    shed[0][1] = segundo;
}
