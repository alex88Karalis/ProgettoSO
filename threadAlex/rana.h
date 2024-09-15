#ifndef RANA_HE
#define RANA_HE

#include "utilities.h"

/*  ----------  THREAD ------------ */

/** 
 * @brief Lancia il thread Rana e restituisce il thread_id
 * @param p: struttura con i semafori e buffer
*/
pthread_t avviaRanaThread(Params *p);

/** 
 * @brief Thread principale della Rana
 * 
*/
void *moveRanaThread(void *param);

/** 
 * @brief Resetta il thread Rana
 * @return 0 se successo, -1 se errore
*/
int resetRanaThread(Params* thread_args);

/**  
    @brief Imposta il thread Rana a target. decrementa vite e manche. imposta ranaIsDead a TRUE 
*/
void uccidiRana(Params* thread_args);

#endif

