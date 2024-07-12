#include "nemico.h"

// Nemico-Pianta versione Processi


// TODO - Convertirla in versione Thread. 

pid_t avviaNemico(int *pipe_fd, int id)
{
	pid_t nemico_pid = fork();
	if (nemico_pid < 0)
	{
		perror("Fork failed");
		exit(1);
	}
	else
	{
		if (nemico_pid == 0)
		{
			close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
			nemico(pipe_fd, id);
			exit(0);
		}
		else
		{
			return nemico_pid;
		}
	}
	return nemico_pid;
}

void nemico(int *pipe_fd, int id)
{ // l'id fa riferimento all'id del tronco su cui il processo nemico sta

	PipeData nemico;
	nemico.id = id;
	int initialX = 0;
	nemico.y = ARGINEROWSTART + 1;
	// switch su id in modo da asseganre coordinate iniziali
	switch (id)
	{
	case 0:
		initialX = PLANT0STARTCOL;
		break;
	case 1:
		initialX = PLANT1STARTCOL;
		break;
	case 2:
		initialX = PLANT2STARTCOL;
		break;
	case 3:
		initialX = PLANT3STARTCOL;
		break;

	default:
		break;
	}
	nemico.x = initialX;
	nemico.type = 'n';
	write(pipe_fd[1], &nemico, sizeof(PipeData)); // prima scrittura

	// cambio tipo d'ora in poi lancerà soltanto proiettili
	nemico.type = 's';
	int contatore = 0;
	while (1)
	{	
		// parte relativa al lancio dei proiettili
		
		if (contatore % 100 == 0)
		{
			write(pipe_fd[1], &nemico, sizeof(PipeData));
		}
		contatore = (contatore + 1) % 50;
		usleep(100000*(id+1)); // Aspetta un po' prima di generare nuove coordinate
	}
}

void killNemico(pid_t pid_nemico)
{
	kill(pid_nemico, SIGKILL);
	waitpid(pid_nemico, NULL, 0);
	return;
}


// ----- Thread ------
/**
 * Avvia il thread Nemico(Pianta) e restituisce il threadID
 * @param thread_args Struttura con i parametri per il thread
 * @param id 			Id assegnato all'oggetto nemico
 */
pthread_t avviaNemicoThread(Params *thread_args, int id){
	pthread_t tid_nemico;
	thread_args->id = id; //Passo l'id valido del nemico agli argomenti del thread
	
	if( pthread_create(&tid_nemico, NULL, &nemicoThread, (void*)thread_args) != 0){
		perror("ERR: Avvio Nemico Fallito!");
		_exit(3);
	}
	return tid_nemico;
}

/**
 * Funzione principale del thread Nemico(Pianta)
 */
void* nemicoThread(void *param){
	Params *p=(Params*)param;
	ThreadControlBlock new_tcb = {pthread_self(),false,false};
	copiaTCB(&p->gameData->allTCB->tcb_piante[p->id] ,new_tcb, &p->semafori->tcb_mutex);
	ThreadControlBlock *my_tcb = &p->gameData->allTCB->tcb_piante[p->id];
	int initialX = 0;

	// Seme unico per ogni processo
    unsigned int seed = time(NULL) ^ (pthread_self() << 16); 
	// switch su id in modo da asseganre coordinate iniziali
	switch (p->id)
	{
	case 0:
		initialX = PLANT0STARTCOL;
		break;
	case 1:
		initialX = PLANT1STARTCOL;
		break;
	case 2:
		initialX = PLANT2STARTCOL;
		break;
	case 3:
		initialX = PLANT3STARTCOL;
		break;

	default:
		break;
	}

	// Inizializza dati del nuovo nemico 
	PipeData nemico;
	nemico.x = initialX;
	nemico.y = ARGINEROWSTART + 1;
	nemico.type = 'n';
	nemico.id = p->id; // l'ID specifico del nemico
	nemico.thread_id = pthread_self();
	
	//int randNaps = rand()%5 * (p->id);
	//usleep(1000* randNaps);
	/*
	// numero randomico tra min e max compresi
	int randomico = generaRandom_r(600000 , 600000*25,&seed);
	// piccola usleep
	usleep(randomico);
	/**/
	// Prima scrittura sul buffer 
	scriviSuBuffer(p,nemico,my_tcb,false);

	// cambio tipo d'ora in poi lancerà soltanto proiettili
	nemico.type = 's';
	int contatore = 1;
	while (1)
	{	
		// parte relativa al lancio dei proiettili

		if(!(isThreadTarget(my_tcb, &p->semafori->tcb_mutex)))
		{	// se il thread NON deve terminare, dice a Disegna di sparare un proiettile
			if (contatore % 100 == 0)
			{	
				/*
				if(nemico.x-initialX>=3){
					nemico.x=initialX;
				}
				if(nemico.x-initialX==1){
					nemico.y = ARGINEROWSTART + 2;
				}
				else{
					nemico.y= ARGINEROWSTART + 1;
				}
				/**/
				//nemico.type = 's';
				scriviSuBuffer(p,nemico,my_tcb,false);
				//nemico.x++;
			}
			
			contatore = (contatore + 1) % 50;
			usleep(100000*(nemico.id+1)); // Pausa diversa per ogni nemico
		}else{
			break;
		}
	}
	//Ultima scrittura per dire che sta terminando
	nemico.type = 'n';
	scriviSuBuffer(p, nemico, my_tcb, true);
	pthread_exit(NULL);

}// end nemicoPianta


