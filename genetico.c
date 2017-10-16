#include "genetico.h"
#include "gameController.h"

struct warrior *w;

void addWarrior(int index, struct warrior *new){
    int temp = 1-position[index];
    position[index] = temp;
    shed[index][temp] = new;
}

struct warrior *setNewWarrior(struct warrior **poblacion){

    // int size = 2, gen = 5, index;
    // int mutation = 10,random;
    // struct warrior **parents = malloc(64*sizeof(struct warrior*));
    // struct warrior **sons = malloc(64*sizeof(struct warrior*));
    // while(gen--){
    //     parents = poblacion;
    //     index = 0;
    //     for (int i = 0; i < size; i++) {
    //         struct warrior *temp = malloc(sizeof(struct warrior));
    //         temp->attack = parents[i]->attack;
    //         temp->life = parents[i]->life;
    //         sons[index] = temp;
    //         index++;
    //         temp = malloc(sizeof(struct warrior));
    //         temp->attack = parents[i+1]->attack;
    //         temp->life = parents[i+1]->life;
    //         sons[index] = temp;
    //         index++;
    //         temp = malloc(sizeof(struct warrior));
    //         temp->attack = parents[i]->attack;
    //         temp->life = parents[i+1]->life;
    //         sons[index] = temp;
    //         index++;
    //         temp = malloc(sizeof(struct warrior));
    //         temp->attack = parents[i+1]->attack;
    //         temp->life = parents[i]->life;
    //         sons[index] = temp;
    //         index++;
    //         i++;
    //     }
    //     memset(poblacion,0,64);
    //     for (int i = 0; i < index; i++) {
    //         poblacion[i] = sons[i];
    //         random = rand()%101;
    //         if(random<mutation){
    //             poblacion[i]->attack +=1;
    //             poblacion[i]->life +=1;
    //         }
    //     }
    //     size*=2;
    // }
    // int lvl = poblacion[0]->attack+poblacion[0]->life, aux = 0;
    // int pos = 0;
    // for(int i = 1; i<size; i++){
    //     aux = poblacion[i]->attack+poblacion[i]->life;
    //     if(aux>lvl){
    //         lvl = aux;
    //         pos = i;
    //     }
    // }
    // poblacion[pos]->lvl = (poblacion[pos]->attack+poblacion[pos]->life)/20;
    // if(poblacion[pos]->lvl>9)
    //     poblacion[pos]->lvl = 9;
    // return poblacion[pos];
    int type = rand()%10;
    w -> life = rand()%100;
    w -> attack = rand()%50;
    w -> type = type + 'A';
    w -> intType = type;
    w -> lvl = rand()%10;
    return w;
}

void initShed(){
    w = calloc(1, sizeof(struct warrior));
    for(int i = 0;i<10;i++){
        position[i] = 0;         //esto siempre inicia como 0
        shed[i][0] = malloc(sizeof(struct warrior));
        shed[i][0] -> life = 20;
        shed[i][0] -> attack = 5;

        shed[i][1] = malloc(sizeof(struct warrior));
        shed[i][1] -> life = 30;
        shed[i][1] -> attack = 3;
    }
}
