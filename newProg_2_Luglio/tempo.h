#ifndef TEMPOX_H
#define TEMPOX_H
#include "utilities.h"
#include <sys/time.h>

/* --------     PROCESSI      ----------  */

pid_t avviaTempo(int* pipe_fd);
void tempoProcess(int* pipe_fd);

/* --------     THREAD      ----------  */

pthread_t avviaTempoThread(Params *thread_arg);
void *tempoThread(void* param);



#endif