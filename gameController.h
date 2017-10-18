#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
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
    int cTower;
};


/*
 * Funcion la cual asigna True si es un cliente y False si es el servidor,
 * con el fin de saber si la computadora juega como jugador 2 o no.
 *
 * Entrada:
 * b -> booleano True o False.
 */
void setBPlayer2(int b);

/*
 * Funcion la cual desplaza el menu hacia arriba.
 */
void upMenu();

/*
 * Funcion la cual desplaza el menu hacia abajo.
 */
void downMenu();

/*
 * Funcion la cual desplaza el menu hacia la izquierda.
 */
void leftMenu();

/*
 * Funcion la cual desplaza el menu hacia la derecha.
 */
void rigthMenu();

/*
 * Funcion la cual alterna entre la seleccion de la carta y la posicion
 * en el campo. Ademas de llamar a spawnWarrior para posicionarlo en el tablero.
 */
void selectMenu();

/*
 * Funcion la cual es utilizada por el hilo del juego el cual controla la
 * muestra del campo.
 */
void *gameController();
void spawnWarrior(char type, int lvl, int life, int attack, int x, int y, int bPlayer2);
void endGame();

/*
 * Funcion la cual imprime el mensaje de ganar o perder.
 *
 * Entrada:
 * bWinner -> booleano el cual significa si el jugador gano o no.
 */
void printFinish(int bWinner);

#endif
