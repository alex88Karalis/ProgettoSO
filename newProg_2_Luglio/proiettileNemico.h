#ifndef PROIETTILENEMICO_H
#define PROIETTILENEMICO_H
#include "utilities.h"

pid_t avviaProiettileNemico(int* pipe_fd, PipeData *shooter,int id);
void proiettileNemico(int* pipe_fd, PipeData *shooter,int id);
void uccidiProiettileNemico( pid_t *array_pid_proiettili, int id_proiettile);

// -------- THREAD -----------------------------

pthread_t avviaProiettileNemicoThread(Params* thread_arg, int id);
void* proiettileNemicoThread(void* param);


#endif
