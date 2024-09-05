#ifndef COCCODRILLO_H
#define COCCODRILLO_H

#include "utilities.h"

/* CODICE */
pid_t avviaCoccodrillo(int* pipe_fd, PipeData* coccodrillo_init,int id);

void coccodrillo(int* pipe_fd, PipeData* coccodrillo_init,int id);

void uccidiCoccodrillo(pid_t *pids_coccodrilli, int id_coccodrillo);

pthread_t avviaCoccodrilloThread(Params_coccodrilli *thread_args, int id);

//pthread_t avviaCoccodrilloThread(Params_coccodrilli *thread_args, PipeData* coccodrillo_init, int id);

void* coccodrilloThread(void *param);



#endif