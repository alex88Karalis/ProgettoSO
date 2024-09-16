#ifndef INIZIALIZZA_H
#define INIZIALIZZA_H
#include "./utilities.h"
#include "./rana.h"
#include "./hud.h"
#include "./nemico.h"
#include "./tempo.h"

/***/
void inizializzaGameInfo(GameData* gamedata);

void inizializzaControlloCoccodrilli(GameData* gameData);

/***/
void inizializzaContatori(GameData* gameData);

/***/
void inizializzaSchermo(GameData* gameData);

/***/
void inizializzaOldPos(OldPos *old_pos);

/***/
void inizializzaFlagMatrice(ScreenCell (*screenMatrix)[WIDTH]);

/***/
void inizializzaMarciapiede(ScreenCell (*screenMatrix)[WIDTH]);

/***/
void inizializzaStrada(ScreenCell (*screenMatrix)[WIDTH]);

/***/
void inizializzaArgine(ScreenCell (*screenMatrix)[WIDTH]);

/***/
void inizializzaFiume(ScreenCell (*screenMatrix)[WIDTH]);

/***/
void inizializzaTane(GameData* gameData);

/***/
void inizializzaHUD(GameData* gameData);

/***/
void copiaMatrice(ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]);

/***/
void inizializzaOggettiTane(Tana* tane);

/***/
void inizializzaSprites(Sprite* sprites);

/** @brief inizializza la struttura dati del gioco
 * @param gameData : la struttura dati del gioco
*/
void inizializza(GameData* gameData);

/***/
Sprite inizializzaSprite(int rows, int cols, char sprite[rows][cols], int color, TipoObj tipo_oggetto);

/***/
void inizializzaBox(GameData* gameData);

/***/
void inizializzaTempo(GameData* gameData);

void inizializzaFlussi(GameData* gameData);

void inizializzaPid(GameData* gameData);
#endif
