#include "proiettile.h"

pthread_t avviaProiettileThread(Params *thread_arg, int id)
{
	pthread_t tid_proiettile;
	thread_arg->id = id; // incapsula l'id dell'oggetto nei parametri da passare al thread

	// crea il thread proiettile e controlla.
	
	if (pthread_create(&tid_proiettile, NULL, &moveProiettileThread, (void*)thread_arg) != 0)
	{
		perror("ERR: Avvio Proiettile Fallito!");
		_exit(3);
	}
	else
	{
		return tid_proiettile;
	}
}

void *moveProiettileThread(void *param)
{
	Params *p = (Params *)param;
	pthread_t my_tid = pthread_self();
	ThreadControlBlock my_tcb = {my_tid,false,false};
	
	/*
	ThreadControlBlock my_tcb;
	// inizializzazione del tcb
	my_tcb.is_target=false;
	my_tcb.is_terminated=false;
	my_tcb.thread_id=my_tid;
	/**/

	copiaTCB(&(p->gameData->allTCB->tcb_proiettili[p->id]), my_tcb, &(p->semafori->tcb_mutex));

	RanaAbsPos *initPos = &(p->gameData->ranaAbsPos);
	/*
	if (sem_wait(&p->semafori->tcb_mutex) != 0)
	{
		perror("ERR: Semaforo proiettile WAIT Fallita");
	};
	// non serve
	//ThreadControlBlock *array_tcb = p->gameData->allTCB->tcb_proiettili; // vettore di tutti TCB proiettili
	//pthread_t th0= array_tcb[0].thread_id;
	//my_tcb = recuperaThreadTCB(array_tcb, my_tid, MAXNPROIETTILI);
	// assegnazione del tcb a gamedata all tcb la posizione è quella dell' id passato al thread
	p->gameData->allTCB->tcb_proiettili[p->id].is_target=false;
	p->gameData->allTCB->tcb_proiettili[p->id].is_terminated=false;
	p->gameData->allTCB->tcb_proiettili[p->id].thread_id=my_tid;

	if (sem_post(&p->semafori->tcb_mutex) != 0)
	{
		perror("ERR: Semaforo proiettile POST Fallita");
	};
	/**/

	int dirY = -1;
	PipeData proiettile;
	proiettile.x = initPos->x; // le coordinate iniziali del proiettile sono quelle dell' oggetto che ha sparato
	proiettile.y = initPos->y;
	proiettile.type = 'P';
	proiettile.id = p->id;
	proiettile.thread_id = my_tid;

	ThreadControlBlock* tcbFromGameData = cercaThreadByTid(p->gameData->allTCB->tcb_proiettili,my_tid, &(p->semafori->tcb_mutex),MAXNPROIETTILI);

	//ThreadControlBlock* tcbFromGameData = recuperaThreadTCB(p->gameData->allTCB->tcb_proiettili,my_tid,MAXNPROIETTILI);
	

	while (1)
	{
		
		if (!(isThreadTarget(tcbFromGameData, &(p->semafori->tcb_mutex))))
		{
			proiettile.y = proiettile.y + dirY;
			scriviSuBuffer(p, proiettile, tcbFromGameData, false);
			usleep(100000);
		}
		else{
			break;
		}
		
	}

	scriviSuBuffer(p, proiettile, tcbFromGameData, true);
	usleep(600000);
	pthread_exit(NULL);
	
}


/*
void uccidiProiettileThread(Params *thread_args, GameData *_gameData, pthread_t thread_id)
{

	GameData *gameData = thread_args->gameData;

	ThreadControlBlock *my_tcb = recuperaThreadTCB(gameData->allTCB->tcb_proiettili, thread_id, MAXNPROIETTILI);
	ThreadControlBlock perDebug = *my_tcb;
	pthread_t perDebug2ThreadIdPassato= thread_id;
	int ciao=0;
	if (sizeof(my_tcb) != sizeof(ThreadControlBlock *))
	{
		perror("ERR: Dimensione TCB Proiettile Errata!");
	}
	if (my_tcb == NULL)
	{
		perror("ERR: Ricerca del TCB Fallita");
		exit(2);
	}

	if (my_tcb->thread_id != thread_id)
	{
		perror("ERR: Incoerenza di Trhead ID");
		exit(-1);
	}

	sem_wait(thread_args->mutex);
	int err = impostaThreadTarget( thread_args->gameData->allTCB->tcb_proiettili, thread_id, MAXNPROIETTILI);
	if (err)
	{
		perror("ERR: Terminazione proiettile Fallita");
		exit(2);
	}
	sem_post(thread_args->mutex);

	//	---	da qui in poi da rimuovere	---
	// spinlock
	while(!(my_tcb->is_target) && !(my_tcb->is_terminated)){
		;
	}
	
	if (my_tcb->is_target && my_tcb->is_terminated)
	{
		if (sizeof(my_tcb->thread_id) != sizeof(pthread_t))
		{
			perror("Errore nel tid del thread");
			exit(2);
		}

		int err2 = pthread_join(my_tcb->thread_id, NULL);
		if (err2)
		{
			sem_wait(thread_args->mutex);
			gameData->errore=1;
			sem_post(thread_args->mutex);
		}
		else
		{
			sem_wait(thread_args->mutex);
			gameData->errore=2;
			sem_post(thread_args->mutex);

			//resetta valori tcb proietile
			my_tcb->thread_id = 0;
			my_tcb->is_target = my_tcb->is_terminated = false;
		}	
	}
}
/**/


