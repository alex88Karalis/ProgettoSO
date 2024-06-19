#include "rana.h"

// funzione che si occupa di avviare il processo rana e restituirne il pid
pid_t avviaRana(int* pipe_fd){
	pid_t move_pid = fork(); // pid che contiente il pid della rana
    
  if (move_pid < 0) {
  	perror("Fork failed");
    exit(1);
  } else if (move_pid == 0) {
  		// Processo "muovi"
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
      moveProcess(pipe_fd);
      exit(0);
  }
	return move_pid;
}

// il main del processo rana
void moveProcess(int* pipe_fd) {
	
	// inizializzazione pipeData per comunicazione
	PipeData pipeData;
	// coordinate relative iniziali nulle
	pipeData.x=0;
	pipeData.y=0;
	pipeData.type='X';
	pipeData.id=0;
	
		
	// Invia le coordinate iniziali attraverso la pipe
    write(pipe_fd[1], &pipeData, sizeof(PipeData));
    
	noecho();
	int ch='D';
    bool change=false; // flag tasto premuto/non premuto
	// loop principale del processo rana
	while (1) {
    	change=false; // reset flag tasto premuto
    	pipeData.type='X'; // resetta il normale carattere della rana
       	
        ch = getch(); // Leggi il carattere dall'input
        
        // Muovi il personaggio in base all'input dell'utente
        switch (ch) {
        	case KEY_UP:
				pipeData.y=-2;
            	change=true;
            break;
          case KEY_DOWN:
            	pipeData.y=+2;
            	change=true;
            break;
          case KEY_LEFT:
            	pipeData.x=-1;
            	change=true;  
            break;
          case KEY_RIGHT:
            	pipeData.x=1;
            	change=true;
            break;
          case 32: // KEY_SPACE 
          	pipeData.type='S'; //cambia carattere per dire a processoDisegna che  rana sta sparando
          	change=true;
            break;
          case 'p': // tasto p  PAUSA
          case 'P':
          	pipeData.type = 'Z';
          	change=true; 
            break;
          default:
          	break;
        }
		
		if(change){
			// Invia le coordinate attraverso la pipe
        	write(pipe_fd[1], &pipeData, sizeof(PipeData));
			// reset coordinate e tipo
			pipeData.x=0;
			pipeData.y=0;
			pipeData.type='X';
		}
        
        // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
        usleep(1000);
         
    }
    return;
}


//-------	VERSIONE THREADS ----------------

pthread_t avviaRanaThread(Params *thread_args){
	
	pthread_t tid_rana;
	
	if( pthread_create(&tid_rana, NULL, &moveRanaThread, thread_args) != 0){
		perror("ERR: Avvio Rana Fallito!");
		_exit(2); 
	}
	return tid_rana;
}

void *moveRanaThread(void *param){
	Params *p=(Params*)param;
	ThreadControlBlock new_tcb = {pthread_self(),false,false};
	
	// Copia i valori iniziali nel TCB della RANA
	copiaTCB(p->gameData->allTCB->tcb_rana, new_tcb, &p->semafori->tcb_mutex);
	
	ThreadControlBlock *my_tcb = p->gameData->allTCB->tcb_rana; // ThreadControlBlock Rana 
	
	// inizializzazione pipeData per comunicazione
	PipeData new_pos;
	// coordinate relative iniziali nulle
	new_pos.x=0;
	new_pos.y=0;
	new_pos.type='X';
	new_pos.id=0;
	new_pos.thread_id = pthread_self(); 
	
	
	noecho();
	int ch='D';
    bool change=false; // flag tasto premuto/non premuto
	// loop principale del processo rana
	while (1) {
    	change=false; // reset flag tasto premuto
    	new_pos.type='X'; // resetta il normale carattere della rana
       	
		if(isThreadTarget(my_tcb,&p->semafori->tcb_mutex)){
			break;
		}
        sem_wait(&p->semafori->window_mutex);
        ch = getch(); // Leggi il carattere dall'input
		sem_post(&p->semafori->window_mutex);
        // Muovi il personaggio in base all'input dell'utente
        switch (ch) {
        	case KEY_UP:
				new_pos.y=-2;
            	change=true;
            break;
          case KEY_DOWN:
            	new_pos.y=+2;
            	change=true;
            break;
          case KEY_LEFT:
            	new_pos.x=-1;
            	change=true;  
            break;
          case KEY_RIGHT:
            	new_pos.x=1;
            	change=true;
            break;
          case 32: // KEY_SPACE 
          	new_pos.type='S'; //cambia carattere per dire a processoDisegna che  rana sta sparando
          	change=true;
            break;
          case 'p': // tasto p  PAUSA
          case 'P':
          	new_pos.type = 'Z';
          	change=true; 
            break;
          default:
          	break;
        }
		
		if(change){
			// Invia le coordinate sul buffer
			scriviSuBuffer(p, new_pos, my_tcb, false);

			// reset coordinate e tipo
			new_pos.x=0;
			new_pos.y=0;
			new_pos.type='X';
		}

        // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
        usleep(1000);
    }
	scriviSuBuffer(p, new_pos, my_tcb, true);	// ultima scrittura sul buffer, segna su TCB che ha terminato

    pthread_exit(NULL);
	return NULL;

}// fine moveRanaThread


int resetRanaThread(Params* thread_args){
	pthread_t newRana = 0;
	inizializzaPosRana(&(thread_args->gameData->ranaAbsPos));
	newRana = avviaRanaThread(thread_args);
	if(newRana != 0){
		thread_args->gameData->pids.pidRana = newRana;
		return 0;
	}
	return -1;
}



// per riavviare il processo rana
void resetRana(GameData* gameData){
	kill(gameData->pids.pidRana, SIGKILL);
	waitpid(gameData->pids.pidRana, NULL,0);
	gameData->pids.pidRana = avviaRana(gameData->pipe);
	inizializzaPosRana(&(gameData->ranaAbsPos));
	return;
}