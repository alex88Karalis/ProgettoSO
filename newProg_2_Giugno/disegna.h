#ifndef DISEGNA_H
#define DISEGNA_H

#include "rana.h"
#include "logo.h"
#include "menuIniziale.h"
#include "inizializza.h"
#include "proiettile.h"
#include "hud.h"
#include "schermo.h"
#include "nemico.h"
#include "proiettileNemico.h"

/** @brief Il Thread che disegna e legge da buffer 
 * @param param: parametri del thread (Param *)
*/
void *drawThread (void *param);

/** @brief Avvia il thread Disegna e restituisce il suo thread_id.
 *  @param thread_args: Parametri da passare al thread: semafori, buffer, e GameData.
*/
pthread_t avviaDrawThread(Params *thread_args);




/** @brief avvia il processo disegna
 * @param pipe_fd : la pipe (array di int di dimensione 2)
void avviaDrawProcess(int pipe_fd[2]);
*/

/** @brief processo disegna, 
 * si occupa di leggere la pipe aggiornare il gioco e stampare l'aggiornamento
 * @param pipe_fd : file descriptor della pipe
void drawProcess(int* pipe_fd);
*/

/** @brief stampa la schermata di win*/
void stampaWin();

/** @brief stampa la schermata di lose*/
void stampaGameOver();

/** @brief stampa lo sprite della rana
 * @param datiRana : i dati della rana tipo PipeData
 * @param sprite : la sprite della rana tipo Sprite
 * @param gameData : per la gestione dello schermo
*/
void printRana(PipeData* datiRana, Sprite* sprite,GameData* gameData);

/***/
void *buff_Reader(void *param); // thread di prova di lettura dal buffer

void* buff_Writer(void *param );


#endif
