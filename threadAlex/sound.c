
#include "sound.h"

void avviaSuoni(){
    // Inizializza SDL
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Errore nell'inizializzazione di SDL: %s\n", SDL_GetError());
        return;
    }

    // Inizializza SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        fprintf(stderr, "Errore nell'inizializzazione di SDL_mixer: %s\n", Mix_GetError());
        SDL_Quit();
        return;
    }

    // alloca 16 canali per l'audio
    Mix_AllocateChannels(16);
    return;
}

Mix_Chunk* caricaSuono(char* path){
    // Carica il file audio
    Mix_Chunk *sound = Mix_LoadWAV(path);
    if (!sound) {
        fprintf(stderr, "Errore nel caricamento del file audio: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 0;
    }
    return sound;
}

void riproduciSuono(char* path){
    Mix_Chunk* sound = caricaSuono(path);
    // Riproduce il suono
    if (Mix_PlayChannel(-1, sound, 0) == -1) {
        fprintf(stderr, "Errore nella riproduzione del suono: %s\n", Mix_GetError());
    }
    return;
    // questa parte va messa da qualche parte ma non so ancora dove
    SDL_Delay(2000000);
    // Pulizia e chiusura SDL
	Mix_FreeChunk(sound);
    Mix_CloseAudio();
    SDL_Quit();
}