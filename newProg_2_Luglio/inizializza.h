#ifndef INIZIALIZZA_H
#define INIZIALIZZA_H
#include "utilities.h"
#include "rana.h"
#include "hud.h"
#include "nemico.h"
#include "tempo.h"

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

/** @brief Imposta i valori iniziali della Rana nella matrice Schermo
 * @param ranaPos :  puntatore alle coordinate assolute della Rana nella matrice Schermo 
*/
void inizializzaPosRana(RanaAbsPos* ranaPos);

/***/
void inizializzaOggettiTane(Tana* tane);

/***/
void inizializzaSprites(Sprite* sprites);

/** @brief inizializza la struttura dati del gioco
 * @param gameData : la struttura dati del gioco
 * @param pipe_fd : il file descriptor della pipe
*/
void inizializza(GameData* gameData, int* pipe_fd);

/** @brief avvia i processi base del gioco
 * @param pipe_fd : la pipe
 * @param pids : puntatore a struttura dati che contiene i pid dei processi
*/
void avviaProcessiBase(int* pipe_fd,Pids* pids);


/** @brief avvia il thread Rana
 * @param pids : puntatore a struttura dati che contiene i pid dei processi
 * @param param_general: passa al thread i puntatori principali (semafori,buffer,ecc..)
*/
pthread_t avviaThreadBase(Pids* pids, Params *param_general);


/***/
Sprite inizializzaSprite(int rows, int cols, char **sprite, int color, TipoObj tipoObj);

/***/
void inizializzaBox(GameData* gameData);

/***/
void inizializzaTempo(GameData* gameData);

/** @brief Inizializza valori per i TCB
*/
void inizializzaTCB(AllTCB *array_tcb);

#endif
