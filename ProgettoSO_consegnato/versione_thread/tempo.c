#include "tempo.h"
#include <sys/time.h> // in più??

/* --------     THREAD      ----------  */

// TODO  fare thread Tempo che scriva su buffer il valore della x come valore del tempo
/**
 *  Avvia il thread che gestisce il tempo
 * @param thread_arg struttura per i thread_arg da passare al nuovo thread
 * @return  ritorna il thread_ID del nuovo thread
 */
pthread_t avviaTempoThread(Params *thread_arg)
{
	pthread_t tempo_pid; //= fork(); // pid che contiente il pid della rana
	int err = pthread_create(&tempo_pid, NULL, tempoThread, (void *)thread_arg);

	if (err != 0)
	{
		perror("Fork TEMPO failed");
		exit(1);
	}
	return tempo_pid;
}

/**
 * 	Il thread che gestisce il tempo
 */
void *tempoThread(void *param)
{
	Params *p = (Params *)param;
	sem_t *semaforoTCB = &(p->semafori->tcb_mutex);

	ThreadControlBlock my_tcb = {pthread_self(), false, false};
	copiaTCB(&(p->gameData->allTCB->tcb_tempo), my_tcb, semaforoTCB);
	ThreadControlBlock *tcb_tempo = &(p->gameData->allTCB->tcb_tempo);

	struct timeval start, end;
	long millisec;
	int sec;
	gettimeofday(&start, NULL);

	gettimeofday(&end, NULL);
	millisec = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
	sec = (end.tv_sec - start.tv_sec);
	// inizializzazione pipeData per comunicazione
	PipeData pipeData;
	// coordinate relative iniziali nulle
	pipeData.x = (int)millisec;
	pipeData.y = 0;
	pipeData.type = 'T';
	pipeData.id = 0;

	while (1)
	{
		if (isThreadTarget(tcb_tempo, semaforoTCB))
		{
			break;
		}
		usleep(80000);
		gettimeofday(&end, NULL);
		// millisec = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
		// pipeData.x=(int)millisec;

		// sec = (end.tv_sec - start.tv_sec);
		sec = (int)difftime(end.tv_sec, start.tv_sec);
		if (sec != pipeData.x)
		{
			pipeData.x = (int)sec;

			scriviSuBuffer(p, pipeData, tcb_tempo, false);
		}
	}
	// ultima scrittura di TEMPO
	scriviSuBuffer(p, pipeData, tcb_tempo, true);
	pthread_exit(NULL);
}
