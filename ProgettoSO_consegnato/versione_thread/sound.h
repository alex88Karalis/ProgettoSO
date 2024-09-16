#ifndef SOUND_H
#define SOUND_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

void avviaSuoni();
Mix_Chunk* caricaSuono(char* path);
void riproduciSuono(char* path);

#endif