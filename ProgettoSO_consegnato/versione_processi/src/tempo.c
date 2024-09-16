#include "../hdr/tempo.h"

// funzione che si occupa di avviare il processo rana e restituirne il pid
pid_t avviaTempo(int *pipe_fd)
{
	pid_t tempo_pid = fork(); // pid che contiente il pid della rana

	if (tempo_pid < 0)
	{
		perror("Fork failed");
		exit(1);
	}
	else if (tempo_pid == 0)
	{
		// Processo "muovi"
		close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
		tempoProcess(pipe_fd);
		exit(0);
	}
	return tempo_pid;
}

// per riavviare il processo tempo
void resetTempo(GameData* gameData){
	kill(gameData->pids.pidTempo, SIGKILL);
	waitpid(gameData->pids.pidTempo, NULL,0);
	gameData->pids.pidTempo = avviaTempo(gameData->pipe);
	return;
}

// il main del processo tempo
void tempoProcess(int *pipe_fd)
{

	struct timeval start, end;
	long millisec;
	gettimeofday(&start, NULL);

	gettimeofday(&end, NULL);
	millisec = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

	// inizializzazione pipeData per comunicazione
	PipeData pipeData;
	// coordinate relative iniziali nulle
	pipeData.x = (int)(millisec / 1000);
	pipeData.y = 0;
	pipeData.type = 'T';
	pipeData.id = 0;
	int prev_second = 0;

	// Invia le coordinate iniziali attraverso la pipe
	write(pipe_fd[1], &pipeData, sizeof(PipeData));

	// loop principale del processo tempo
	while (1)
	{
		usleep(8000);
		gettimeofday(&end, NULL);
		millisec = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
		pipeData.x = (int)(millisec / 1000);
		if (prev_second != pipeData.x)
		{
			write(pipe_fd[1], &pipeData, sizeof(PipeData));
		}
		prev_second = pipeData.x;
	}
	return;
}