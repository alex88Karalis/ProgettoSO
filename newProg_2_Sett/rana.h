#ifndef RANA_HE
#define RANA_HE

#include "utilities.h"

/*  ----------  PROCESSI ------------ */

pid_t avviaRana(int* pipe_fd);
void moveProcess(int* pipe_fd);
void resetRana(GameData* gameData);


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


#endif

