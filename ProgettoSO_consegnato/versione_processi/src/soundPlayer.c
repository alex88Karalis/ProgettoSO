#include "../hdr/soundPlayer.h"

pid_t avvia_soundPlayer(int pipe_fd[2])
{
    pid_t sound_player_pid = fork();
    if (sound_player_pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }
    else if (sound_player_pid == 0)
    {
        // Processo sound player
        soundPlayer(pipe_fd);
        exit(0);
    }
    return sound_player_pid;
}

void soundPlayer(int *pipe_fd)
{
    PipeData data;
    avviaSuoni();

    Mix_Chunk *switch_sound = caricaSuono("suoni/change_option.wav");

    while (1)
    {
        read(pipe_fd[0], &(data), sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe

        switch (data.type)
        {
        case 'F':
            // riproduce suono plof caduta in acqua
            Mix_PlayChannel(-1, switch_sound, 0);
            break;
        case 'M':
            // riproduce suono morte
            break;
        default:
            break;
        }
    }

    return;
}

void termina_soundPlayer(GameData *gameData)
{
    if (gameData->pids.soundPlayer != -1)
    {
        kill(gameData->pids.soundPlayer, SIGKILL);
        waitpid(gameData->pids.soundPlayer, NULL, 0);
        gameData->pids.soundPlayer=-1;
    }

    return;
}