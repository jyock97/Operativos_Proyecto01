#include "genetico.h"
#include "gameController.h"
int position[10];
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
    w -> type = type + 'A';
    w -> intType = type;
    w -> lvl = rand()%10;
    w -> life = rand()%100;
    w -> attack = rand()%50;
    return w;
}

void initShed(char *iniFile){

    char ss[256], *temp;
    int bWarrior;
    int life, attack;
    int i;

    w = calloc(1, sizeof(struct warrior)); //se utiliza en el algoritmo genetico

    FILE *f = fopen(iniFile, "r");
    bWarrior = 0;
    while(!bWarrior && fgets(ss, sizeof(ss), f) != NULL){
        if(memcmp(ss, "[warriors]", strlen("[warriors]")) == 0)
            bWarrior = 1;
    }

    i = 0;
    while(fgets(ss, sizeof(ss), f) != NULL) {
        if(ss[0] == '[')
            break;
        if(ss[0] == '#')
            continue;
        temp = strchr(ss, '=');
        if(temp == NULL)
            life = 0;
        else
            life = atoi(temp+1);

        fgets(ss, sizeof(ss), f);
        while(ss[0] == '#'){
            fgets(ss, sizeof(ss), f);
        }
        temp = strchr(ss, '=');
        if(temp == NULL)
            attack = 0;
        else
            attack = atoi(temp+1);

        position[i] = 0;
        shed[i][0] = calloc(1, sizeof(struct warrior));
        shed[i][0] -> life = life;
        shed[i][0] -> attack = attack;
        shed[i][1] = calloc(1, sizeof(struct warrior));
        shed[i][1] -> life = life;
        shed[i][1] -> attack = attack;
        i++;
    }
    fclose(f);
    for (int i = 0; i < 10; i++) {
    }

}
