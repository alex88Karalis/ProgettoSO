#include "tempo.h"

/* --------     PROCESSI      ----------  */


// funzione che si occupa di avviare il processo rana e restituirne il pid
pid_t avviaTempo(int* pipe_fd){
	pid_t tempo_pid = fork(); // pid che contiente il pid della rana
    
  if (tempo_pid < 0) {
  	perror("Fork failed");
    exit(1);
  } else if (tempo_pid == 0) {
  		// Processo "muovi"
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
      tempoProcess(pipe_fd);
      exit(0);
  }
	return tempo_pid;
}


// il main del processo rana
void tempoProcess(int* pipe_fd) {
	
    struct timeval start, end;
	long millisec;
	gettimeofday(&start, NULL);

    gettimeofday(&end, NULL);
	millisec = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

    // inizializzazione pipeData per comunicazione
	PipeData pipeData;
	// coordinate relative iniziali nulle
	pipeData.x=(int)millisec;
	pipeData.y=0;
	pipeData.type='T';
	pipeData.id=0;
		
	// Invia le coordinate iniziali attraverso la pipe
    write(pipe_fd[1], &pipeData, sizeof(PipeData));
    
	// loop principale del processo tempo
	while (1) {
		usleep(8000);
		gettimeofday(&end, NULL);
	    millisec = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
        pipeData.x=(int)millisec;
         write(pipe_fd[1], &pipeData, sizeof(PipeData));
    }
    return;
}


/* --------     THREAD      ----------  */

// TODO  fare thread Tempo che scriva su buffer il valore della x come valore del tempo
/**
 *  Avvia il thread che gestisce il tempo
 * @param thread_arg struttura per i thread_arg da passare al nuovo thread
 * @return  ritorna il thread_ID del nuovo thread
 */
pthread_t avviaTempoThread(Params *thread_arg)
{
	pthread_t tempo_pid ;//= fork(); // pid che contiente il pid della rana
    int err = pthread_create(&tempo_pid, NULL, tempoThread, (void*)thread_arg);

  	if (err != 0) {
  		perror("Fork TEMPO failed");
    	exit(1);
  	} 
	return tempo_pid;
}


/**
 * 	Il thread che gestisce il tempo
 */
void *tempoThread(void* param){
	Params *p = (Params*) param;
	sem_t* semaforoTCB = &(p->semafori->tcb_mutex);

	ThreadControlBlock my_tcb = {pthread_self(), false, false };
	copiaTCB( &(p->gameData->allTCB->tcb_tempo), my_tcb, semaforoTCB);
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
	pipeData.x=(int)millisec;
	pipeData.y=0;
	pipeData.type='T';
	pipeData.id=0;


	while(1)
	{
		if(isThreadTarget(tcb_tempo, semaforoTCB)){
			break;
		}
		usleep(80000);
		gettimeofday(&end, NULL);
	    //millisec = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
		//pipeData.x=(int)millisec;
		
		sec = (end.tv_sec - start.tv_sec);
		pipeData.x=(int)sec;
        
		scriviSuBuffer(p, pipeData, tcb_tempo, false);
	}
	// ultima scrittura di TEMPO
	scriviSuBuffer(p, pipeData, tcb_tempo, true);
	pthread_exit(NULL);
}
