#ifndef TEMPOX_H
#define TEMPOX_H
#include "./utilities.h"

pid_t avviaTempo(int* pipe_fd);
void resetTempo(GameData* gameData);
void tempoProcess(int* pipe_fd);

#endif