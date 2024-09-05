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
	
	copiaTCB(&(p->gameData->allTCB->tcb_proiettili[p->id]), my_tcb, &(p->semafori->tcb_mutex));

	RanaAbsPos *initPos = &(p->gameData->ranaAbsPos);
	
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
	//usleep(600000);
	pthread_exit(NULL);
	
}



