#include "nemico.h"


// ----- Thread ------
/**
 * Avvia il thread Nemico(Pianta) e restituisce il threadID
 * @param thread_args Struttura con i parametri per il thread
 * @param id 			Id assegnato all'oggetto nemico
 * @return il threadID del nemicoPianta
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
	// copia i valori nel nuovo TCB (new_tcb) nel TCB della pianta(tcb_piante[id])
	copiaTCB(&p->gameData->allTCB->tcb_piante[p->id] ,new_tcb, &p->semafori->tcb_mutex); 
	ThreadControlBlock *my_tcb = &p->gameData->allTCB->tcb_piante[p->id]; 
	int initialX = 0;

	// Seme unico per ogni thread
    
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
				scriviSuBuffer(p,nemico,my_tcb,false);
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


