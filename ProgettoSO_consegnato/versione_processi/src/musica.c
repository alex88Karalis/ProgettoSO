#include "../hdr/musica.h"

pid_t avviaMusica()
{

    pid_t musica_pid = fork();

    if (musica_pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }
    else if (musica_pid == 0)
    {
        musica();
        exit(0);
    }
    else
    {
        return musica_pid;
    }
}

void musica()
{
    // fa partire la musica
    avviaSuoni();
	// load della musica di gioco la musica deve essere formato mp3
	Mix_Music *music = Mix_LoadMUS("suoni/lvl_music.mp3");
	
	// play della musica di gioco in loop
	int volume = 40; // Valore tra 0 (muto) e 128 (massimo)
	Mix_VolumeMusic(volume);
	Mix_PlayMusic(music, -1);

    while (1)
    {
    }
}

void spegniMusica(GameData *gameData)
{
    if(gameData->pids.pidMusica!=-1){
        kill(gameData->pids.pidMusica, SIGKILL);
        waitpid(gameData->pids.pidMusica, NULL, 0);
        gameData->pids.pidMusica = -1;
    }
    
    return;
}
