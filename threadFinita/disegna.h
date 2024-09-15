#ifndef DISEGNA_H
#define DISEGNA_H

#include "rana.h"
#include "logo.h"
#include "inizializza.h"
#include "proiettile.h"
#include "hud.h"
#include "schermo.h"

//#include "nemico.h"             // in più?
//#include "proiettileNemico.h"   // in più?
//#include "tempo.h"              // in più?
#include "coccodrillo.h"


pthread_t avviaDrawThread(Params *thread_args);

void *drawThread (void *param);
void stampaWin(GameData *gameData);
void stampaGameOver(GameData *gameData);
void printRana(PipeData* datiRana, Sprite* sprite,GameData* gameData);
bool thereIsSpaceForCoccodrilloOnFila(GameData *gameData, int fila);
void spawnCoccodrilli(GameData *gameData, int sec, int contatore_dispari,Params* p);

#endif
