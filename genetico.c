#include "genetico.h"
#include "gameController.h"
int position[10];
struct warrior *w;
struct warrior *parents[64];
struct warrior *sons[64];

//Funcion que agrega el ultimo warrior muerto
void addWarrior(int index, struct warrior *new){
    int temp = 1-position[index];
    position[index] = temp;
    shed[index][temp] = new;
}

//Funcion que realiza el algoritmo genetico
struct warrior *setNewWarrior(struct warrior **poblacion, int type){

    int size = 2, gen = 5, index;
    int mutation = 10,random;
    for(int i = 0;i<64;i++){//inicia las estructuras
        parents[i] = calloc(1,sizeof(struct warrior));
        sons[i] = calloc(1,sizeof(struct warrior));
    }
    for (int i = 0; i < size; i++) {//setea los primeros padres
        parents[i] -> life = poblacion[i]-> life;
        parents[i] -> attack = poblacion[i]-> attack;
    }
    while(gen--){//para despues de x generaciones
        index = 0;
        for (int i = 0; i < size; i++) {//realiza los cruces con los padres
            sons[index]-> life = parents[i]->life;
            sons[index]-> attack = parents[i]->attack;
            index++;

            sons[index]-> life = parents[i+1]->life;
            sons[index]-> attack = parents[i+1]->attack;
            index++;

            sons[index]-> life = parents[i+1]->life;
            sons[index]-> attack = parents[i]->attack;
            index++;

            sons[index]-> life = parents[i]->life;
            sons[index]-> attack = parents[i+1]->attack;
            index++;
            i++;
        }
        for (int i = 0; i < index; i++) {
            parents[i] = sons[i];
            random = rand()%101;
            if(random<mutation){//revisa el factor de mutacion
                parents[i]->attack +=1;
                parents[i]->life +=1;
            }
        }
        size*=2;
    }
    int lvl = parents[0]->attack+parents[0]->life, aux = 0;
    int pos = 0;
    for(int i = 1; i<size; i++){//busca el mejor elemento
        aux = parents[i]->attack+parents[i]->life;
        if(aux>lvl){
            lvl = aux;
            pos = i;
        }
    }
    parents[pos]->lvl = (parents[pos]->attack+parents[pos]->life)/20;
    if(parents[pos]->lvl>9)
        parents[pos]->lvl = 9;
    parents[pos] -> type = type + 'A';//setea demas datos
    parents[pos] -> intType = type;
    return parents[pos];
}

//Funcion que inicia matriz de elementos para usar en el algoritmo genetico
//lee desde el game.ini
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
