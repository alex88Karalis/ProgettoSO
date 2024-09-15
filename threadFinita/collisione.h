#ifndef COLLISIONE_H
#define COLLISIONE_H
#include "utilities.h"
#include "rana.h"
#include "schermo.h"


Collisione detectCollisione(GameData* gameData);
void handleCollisione(Params* thread_args, GameData* gameData,Collisione collisione);
void setCollision(Collisione *collisione, TipoCollisione tipo, TipoObj attivo, int id_attivo, TipoObj passivo, int id_passivo, int hit_x, int hit_y);
#endif // COLLISIONE_H