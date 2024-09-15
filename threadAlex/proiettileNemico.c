#include "proiettileNemico.h"

// Versione PROCESSI
// TODO - Convertire in versione Thread

pid_t avviaProiettileNemico(int* pipe_fd, PipeData *shooter,int id){
	pid_t proiettile_nemico_pid = fork();
    
  if (proiettile_nemico_pid < 0) {
  	perror("Fork failed");
    exit(1);
  } else if (proiettile_nemico_pid == 0) {
  		// Processo "muovi"
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
     	proiettileNemico(pipe_fd, shooter,id);
      exit(0);
  }
  else{
  	// sono nel padre
  	return proiettile_nemico_pid;
  }
}
void proiettileNemico(int *pipe_fd, PipeData *shooter,int id) {
		int dirY = 1;
		PipeData proiettileNemico;
		proiettileNemico.x=shooter->x;	// le coordinate iniziali del proiettile sono quelle dell' oggetto che ha sparato
		proiettileNemico.y=shooter->y;
		proiettileNemico.type='p';
		proiettileNemico.id=id;
		
    while (1) {
    	proiettileNemico.y = proiettileNemico.y + dirY;
        write(pipe_fd[1], &proiettileNemico, sizeof(PipeData));
        usleep(300000);// Aspetta un po' prima di generare nuove coordinate
    }
}


void uccidiProiettileNemico( pid_t *array_pid_proiettili, int id_proiettile){
	if((id_proiettile != -1) && (array_pid_proiettili[id_proiettile] != 0))
	{
		kill(array_pid_proiettili[id_proiettile], SIGKILL);
		waitpid(array_pid_proiettili[id_proiettile],NULL,0);
		array_pid_proiettili[id_proiettile]=0;
	}
}


//------- THREAD ------------

pthread_t avviaProiettileNemicoThread(Params* thread_arg, int id){
	pthread_t tid_proiettileNemico=0;
	thread_arg->id = id;

	if(pthread_create(&tid_proiettileNemico,NULL,&proiettileNemicoThread, (void*)thread_arg) != 0){
		perror("ERR: Avvio ProiettileNemico Fallito !");
		_exit(4);
	}

	return tid_proiettileNemico;
}


void* proiettileNemicoThread(void* param){
	Params *p = (Params *)param;
	pthread_t my_tid = pthread_self();
	ThreadControlBlock my_tcb = {my_tid,false,false};

	// Inizializzo il TCB della stuttura generale coi valori iniziali
	copiaTCB(&(p->gameData->allTCB->tcb_proiettili_nemici[p->id]), my_tcb, &(p->semafori->tcb_mutex));

	PipeData shooter = p->gameData->oldPos.nemici[p->id]; //identifica il nemico che ha sparato
	
	PipeData proiettileNemico;
	proiettileNemico.x = shooter.x;	
	proiettileNemico.y = shooter.y;
	proiettileNemico.type = 'p';
	proiettileNemico.id = shooter.id; 
	proiettileNemico.thread_id = my_tid;
	
	ThreadControlBlock *proiettileNemicoTCB = &(p->gameData->allTCB->tcb_proiettili_nemici[p->id]);

	while(1){
		if(isThreadTarget(proiettileNemicoTCB, &(p->semafori->tcb_mutex))){
			break;
		}else{
			proiettileNemico.y++;
			//if(proiettileNemico.y >= MARCIAPIEDEROWEND){ break;}
			// if(proiettileY è troppo avanti) esci
			scriviSuBuffer(p, proiettileNemico, proiettileNemicoTCB, false);
			usleep(100000);
		}
	}

	// Ultima scrittura prima di uscire
	// y = MARCIAPIEDEROWEND
	scriviSuBuffer(p, proiettileNemico, proiettileNemicoTCB, true);

	pthread_exit(NULL);
}







