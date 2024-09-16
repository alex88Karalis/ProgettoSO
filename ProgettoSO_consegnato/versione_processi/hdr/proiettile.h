#ifndef PROIETTILE_HE
#define PROIETTILE_HE

#include "./utilities.h"

pid_t avviaProiettile(int* pipe_fd, RanaAbsPos *initPos,int id);
void moveProiettile(int* pipe_fd, RanaAbsPos *initPos,int id);
void uccidiProiettile( pid_t *array_pid_proiettili, int id_proiettile);

//void proiettile(int* pipe_fd, PipeData* obj);
//pid_t creaProiettile(int* pipe_fd, PipeData* obj);

#endif
