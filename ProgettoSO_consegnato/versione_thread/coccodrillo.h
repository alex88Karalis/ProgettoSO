#ifndef COCCODRILLO_H
#define COCCODRILLO_H

#include "utilities.h"


pthread_t avviaCoccodrilloThread(Params_coccodrilli *thread_args, int id);

void* coccodrilloThread(void *param);



#endif