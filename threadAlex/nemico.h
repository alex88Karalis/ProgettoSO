#ifndef NEMICO_H
#define NEMICO_H
#include "utilities.h"

// --------- THREAD -------
pthread_t avviaNemicoThread(Params *thread_args, int id);
void* nemicoThread(void *param);



#endif
