#ifndef SOUNDPLAYER_HE
#define SOUNDPLAYER_HE

#include "./utilities.h"

pid_t avvia_soundPlayer(int pipe_fd[2]);
void soundPlayer(int *pipe_fd);
void termina_soundPlayer(GameData* gameData);
#endif