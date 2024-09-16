#ifndef TEMPOX_H
#define TEMPOX_H
#include "utilities.h"
//#include <sys/time.h> // in più??



/* --------     THREAD      ----------  */

pthread_t avviaTempoThread(Params *thread_arg);
void *tempoThread(void* param);



#endif