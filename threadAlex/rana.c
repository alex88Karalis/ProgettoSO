#include "rana.h"

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
	cbreak();
	nodelay(stdscr,TRUE);
    wtimeout(stdscr,0);

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
			case 'w':
			case 'W':
				new_pos.y=-2;
            	change=true;
            	break;
          	case KEY_DOWN:
		  	case 's':
		  	case 'S':
            	new_pos.y=+2;
            	change=true;
            	break;
          	case KEY_LEFT:
		  	case 'a':
		  	case 'A':
            	new_pos.x=-2;
            	change=true;  
            	break;
          	case KEY_RIGHT:
		  	case 'd':
		  	case 'D':
            	new_pos.x=2;
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


void uccidiRana(Params* thread_args){
	GameData* gameData = thread_args->gameData;
	sem_t* semaforoTCB = &(thread_args->semafori->tcb_mutex);
	ThreadControlBlock* tcb_rana = gameData->allTCB->tcb_rana;

	if(isThreadTarget(tcb_rana, semaforoTCB)){ return;}
	impostaThreadTarget(tcb_rana, semaforoTCB);
	gameData->gameInfo.vite--;
	gameData->gameInfo.manche--;
	gameData->gameInfo.viteIsChanged = true;
	gameData->gameInfo.mancheIsChanged = true;
	gameData->gameInfo.ranaIsDead=true;
}




