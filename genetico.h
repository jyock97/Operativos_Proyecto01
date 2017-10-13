#ifndef GENETICO_H
#define GENETICO_H
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "gameController.h"

struct warrior *shed[10][2];
int position[10];

void addWarrior(int index, struct warrior *new);

struct warrior *setNewWarrior(struct warrior **poblacion);

void initShed();

#endif
