#ifndef PROIETTILE_HE
#define PROIETTILE_HE

#include "utilities.h"

/** @brief Funzione del thread proiettile
 * 
*/
void *moveProiettileThread(void *param);

/** @brief Avvia il thread proiettile con i parametri 
 * @param thread_arg: Puntatore alla struttura con i thread_arguments
 * @param id: ID dell'oggetto proiettile
 * @return (pthread_t) Ritorna il thread_id del proiettile
*/
pthread_t avviaProiettileThread(Params *thread_arg, int id);

/** @brief Designa il thread come target, attende che termini e chiama la join sul thread aggiornando il rispettivo TCB del thread
 * @param thread_args : Thread argument
 * @param gameData : riferimento alla struttura generale dati
 * @param thread_id : identificativo del thread bersaglio
*/
//void uccidiProiettileThread(Params* thread_args, GameData* _gameData, pthread_t thread_id);

#endif
