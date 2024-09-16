#ifndef NEMICO_H
#define NEMICO_H
#include "./utilities.h"
pid_t avviaNemico(int* pipe_fd,int id);
void nemico(int *pipe_fd,int id);
void uccidiNemico(pid_t *array_pid_nemici, int id_nemico);
#endif
