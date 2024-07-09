#ifndef COLLISIONE_H
#define COLLISIONE_H
#include "utilities.h"
#include "schermo.h"



Collisione detectCollisione(GameData* gameData);
void handleCollisione(Params* thread_args, GameData* gameData,Collisione collisione);
#endif // COLLISIONE_H