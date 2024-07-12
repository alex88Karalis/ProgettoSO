#include "disegna.h"
#include "coccodrillo.h"

#define DEBUG
//#define FAKE_RANA
//#define DEBUG_PROIETTILI
#define DEBUG_COCCODRILLI

void *buff_Reader(void *param){
	Params *p=(Params*)param;
	GameData* gameData = p->gameData; //recupera puntantore a gameData
	while(1){

		leggiDaBuffer(p, &(gameData->pipeData) );

		sem_wait(&p->semafori->tcb_mutex);
		
		aggiorna(p, gameData); // aggiorna stato del gioco
		printTempo(gameData); // aggiorna hud del tempo
		stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
		sem_post(&p->semafori->tcb_mutex);
	}
	pthread_exit(NULL);
}

void* buff_Writer(void *param ){
	Params *p=(Params*)param;
	//GameData* gameData = p->gameData; //recupera puntantore a gameData
	ThreadControlBlock new_tcb = {pthread_self(), false, false};

	PipeData fakeRana = {1,0, 'X', 0, pthread_self()};
	copiaTCB(p->gameData->allTCB->tcb_rana, new_tcb, &p->semafori->tcb_mutex);
	//PipeData newItem = {1,0, 'X', 0, 0};
	while(1){
		scriviSuBuffer(p, fakeRana, p->gameData->allTCB->tcb_rana, false);
		usleep(10000);
	}
	pthread_exit(NULL);
}

// Thread di DEBUG che simula il thread Rana muovendola lateralmente
void* fakeRana(void *param )	
{
	Params *p=(Params*)param;
	ThreadControlBlock new_tcb = {pthread_self(), false, false};

	PipeData fakeRana = {1,0, 'X', 0, pthread_self()};
	copiaTCB(p->gameData->allTCB->tcb_rana, new_tcb, &p->semafori->tcb_mutex);
	//PipeData newItem = {1,0, 'X', 0, 0};
	while(1){

		if(isThreadTarget(p->gameData->allTCB->tcb_rana, &p->semafori->tcb_mutex)){
			fakeRana.x = -1;
		}else{
			fakeRana.x = 1;
		}
		
		scriviSuBuffer(p, fakeRana, p->gameData->allTCB->tcb_rana, false);
		usleep(10000);
	}
	pthread_exit(NULL);
}

void avviaNemiciThread(Params* thread_param, int* countdown);


void *drawThread (void *param){

	

	Params* p = (Params* ) param;
	
	pthread_t tidRana = 123;

	int* pipe_fd = NULL;
	GameData *gameData = p->gameData; // recupera indirizzio di gameData dai parametri
	
	Params arg_general = {p->semafori, p->buffer, gameData, 0};
	
	// inizializza i dati del gioco
	inizializza(gameData,pipe_fd);

	pthread_t  tid_tempo = avviaTempoThread(p);
	
	tidRana = avviaThreadBase(&(gameData->pids), &(arg_general)); // avvia la Rana
	
	usleep(1000);
	//gameData->pids.pidRana = tidRana;
	//gameData->allTCB->tcb_rana->thread_id = tidRana;

	if(gameData->allTCB->tcb_rana->thread_id == 123)
	{
		beep();
	};


	// prima stampa dello schermo
	printRana(&(gameData->oldPos.rana),&(gameData->sprites[S_RANA]),gameData);
	
	sem_wait(&p->semafori->window_mutex);
	stampaRefreshMatrice(gameData->schermo.screenMatrix);
	sem_post(&p->semafori->window_mutex);

	//int power_naps_piante = rand()%(100);
	
	int countdown_piante = rand()%(100);	// = 0
	int sec;
	int contatore_pari = 0;
	int contatore_dispari = 0;


	// loop principale di gioco
	while (!isGameOver(gameData)) {
		/*
		ThreadControlBlock *tcb_tempo = &(gameData->allTCB->tcb_tempo);
		if(leggiTid(tcb_tempo, &p->semafori->tcb_mutex) == 0){	// se ho azzerato il tempo, riavvialo
			pthread_t tid_tempo = avviaTempoThread(p);
			
			//copiaTCB();
		}
		/**/

		leggiDaBuffer( &(arg_general), &(gameData->pipeData) );


		//	ATTENZIONE: potresti aver letto da buffer l'ultima scrittura di un thread gia chiuso!!
		//				coontrollare se tcb.thread_id==0  	|| 		usare la cercaThreadByTid(...)
		//				oppure togliere ultima scrittura sul buffer dai thread che terminano?


		if(gameData->pipeData.type=='p' && isThreadTerminated(&gameData->allTCB->tcb_proiettili_nemici[gameData->pipeData.id],&(p->semafori->tcb_mutex) )) {
			mvprintw(23,156,"                          ");
			mvprintw(24,156,"                          ");
			mvprintw(25,156,"                          ");
			mvprintw(26,156,"                          ");
			mvprintw(27,156,"                          ");
			
			mvprintw(23,156,"last pipeData a terminated");
			mvprintw(24,156,"x: %d",gameData->pipeData.x);
			mvprintw(25,156,"y: %d",gameData->pipeData.y);
			mvprintw(26,156,"tipo: %c",gameData->pipeData.type);
			mvprintw(27,156,"id: %d",gameData->pipeData.id);
		}
		
		
		#ifdef DEBUG
		// per debug
		mvprintw(0,106,"                                      ");
		mvprintw(0,106,"pipeData: %c x: %d y: %d id: %d",gameData->pipeData.type,gameData->pipeData.x,gameData->pipeData.y,gameData->pipeData.id);

		//mvprintw(0,146,"MAXint: %d", INT16_MAX);

		mvprintw(1,106,"                                      ");
		mvprintw(1,106,"RANA tid: %ld",gameData->allTCB->tcb_rana->thread_id);

		mvprintw(2,106,"               ");
		mvprintw(2,106,"Pos rana x: %d y: %d",gameData->ranaAbsPos.x, gameData->ranaAbsPos.y);
		
		mvprintw(3,106,"               ");
		mvprintw(3,106,"MAX Proiettili : %d ", (int)(sizeof(gameData->allTCB->tcb_proiettili)/sizeof(ThreadControlBlock)));

		#ifdef DEBUG_PROIETTILI
		mvprintw(4,106,"THREAD ID BEFORE UPDATE");
		mvprintw(5,106,"                                ");
		mvprintw(5,106,"%ld",gameData->pids.pidProiettili[0]);
		mvprintw(6,106,"                                ");
		mvprintw(6,106,"%ld",gameData->pids.pidProiettili[1]);
		mvprintw(7,106,"                                ");
		mvprintw(7,106,"%ld",gameData->pids.pidProiettili[2]);

		mvprintw(20,106,"TCB PROIETTILI PRE UPDATE per indice");
		mvprintw(21,106,"                                          ");
		mvprintw(21,106,"0: %ld %d %d",gameData->allTCB->tcb_proiettili[0].thread_id,gameData->allTCB->tcb_proiettili[0].is_target,gameData->allTCB->tcb_proiettili[0].is_terminated);
		mvprintw(22,106,"                                          ");
		mvprintw(22,106,"1: %ld %d %d",gameData->allTCB->tcb_proiettili[1].thread_id,gameData->allTCB->tcb_proiettili[1].is_target,gameData->allTCB->tcb_proiettili[1].is_terminated);
		mvprintw(23,106,"                                          ");
		mvprintw(23,106,"2: %ld %d %d",gameData->allTCB->tcb_proiettili[2].thread_id,gameData->allTCB->tcb_proiettili[2].is_target,gameData->allTCB->tcb_proiettili[2].is_terminated);

		mvprintw(14,106,"                                ");
		mvprintw(14,106,"CONT PROIETTILI PRE UPDATE: %d",gameData->contatori.contProiettili);
		#endif


		#ifdef DEBUG_COCCODRILLI
		mvprintw(4,106,"COCCODRILLI THREAD ID BEFORE UPDATE");
		mvprintw(5,106,"                                ");
		mvprintw(5,106,"%ld",gameData->pids.pidCoccodrilli[0]);
		mvprintw(6,106,"                                ");
		mvprintw(6,106,"%ld",gameData->pids.pidCoccodrilli[1]);
		mvprintw(7,106,"                                ");
		mvprintw(7,106,"%ld",gameData->pids.pidCoccodrilli[2]);

		mvprintw(20,106,"TCB COCCODRILLI PRE UPDATE per indice");
		mvprintw(21,106,"                                          ");
		mvprintw(21,106,"0: %ld %d %d",gameData->allTCB->tcb_coccodrilli[0].thread_id,gameData->allTCB->tcb_coccodrilli[0].is_target,gameData->allTCB->tcb_coccodrilli[0].is_terminated);
		mvprintw(22,106,"                                          ");
		mvprintw(22,106,"1: %ld %d %d",gameData->allTCB->tcb_coccodrilli[1].thread_id,gameData->allTCB->tcb_coccodrilli[1].is_target,gameData->allTCB->tcb_coccodrilli[1].is_terminated);
		mvprintw(23,106,"                                          ");
		mvprintw(23,106,"2: %ld %d %d",gameData->allTCB->tcb_coccodrilli[2].thread_id,gameData->allTCB->tcb_coccodrilli[2].is_target,gameData->allTCB->tcb_coccodrilli[2].is_terminated);

		mvprintw(14,106,"                                ");
		mvprintw(14,106,"CONT COCCODRILLI PRE UPDATE: %d",gameData->contatori.contCoccodrilli);
		#endif

		// fine debug
		#endif
		
		
		aggiorna(&(arg_general), gameData); // aggiorna stato del gioco e gestisce collisioni
		printTempo(gameData); // aggiorna hud del tempo
		
		int n_1 = pulisciThreadMorti(gameData, arg_general.semafori); //  in prova
		
		
		if(n_1 == 0 && gameData->gameInfo.vite > 0){
		//if(gameData->gameInfo.mancheIsChanged){	// la manche è cambiata (non funziona)
			beep();
			//cancellaOggettoDaMatrice(gameData,&(gameData->oldPos.rana), &gameData->oldPos.rana, S_RANA);
			resetRanaThread(&arg_general);
			aggiornaHud(gameData);
			gameData->gameInfo.mancheIsChanged=false;
		}else{
			aggiornaHud(gameData);
		}
		/**/
		
		/**/

		//--------------AVVIO NEMICI / PIANTE ------------------
		// TODO - aggiustare cancellazione piante 
		
		//avviaNemiciThread(p, countdown_piante);

		/*
		/** */
		if(gameData->contatori.contNemici < MAXNNEMICI-1 && countdown_piante==0 ){
			
			
			int nemicoID = rand()%MAXNNEMICI;
			//int nemicoID = id_disponibile(p->gameData->pids.pidNemici,MAXNNEMICI);
			
			if(nemicoID >= 0 && (gameData->pids.pidNemici[nemicoID]==0)){
			//if(nemicoID >= 0){
				pthread_t nemico = avviaNemicoThread(&arg_general, nemicoID);
				gameData->pids.pidNemici[nemicoID] = nemico;
				//gameData->contatori.contNemici++;
				gameData->contatori.contNemici = (gameData->contatori.contNemici +1) % (MAXNNEMICI+1);
				
				countdown_piante = rand()%100;
				//power_naps_piante = rand()%1000;
			}
		}
		countdown_piante = (countdown_piante > 0) ? countdown_piante-1 : rand()%100; 

		/**/
		//----------------- AVVIO COCCODRILLI ------------------

		sec = gameData->gameInfo.tempo.secondi;

		//sec = gameData->gameInfo.tempo.milliseconds / 1000;

		if (sec % 2 == 0)
		{
			contatore_dispari = 0;
			contatore_pari++;
		}
		else
		{
			contatore_pari = 0;
			contatore_dispari++;
		}

		
		if (thereIsSpaceForCoccodrilloOnFila(gameData, 1) && sec % TEMPO_SPAWN_COCCODRILLI == 0 == 0 && contatore_dispari == 1)
		{
			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[0].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_UNO;
				coccodrillo_init.type = gameData->flussi[0].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;

				Params_coccodrilli arg_coccodrillo = {p, coccodrillo_init};
				pthread_t pid_coccodrillo = avviaCoccodrilloThread( &(arg_coccodrillo), id);

				//pthread_t pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
				//gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				usleep(8000);
				gameData->pids.pidCoccodrilli[id] = leggiTid(&(gameData->allTCB->tcb_coccodrilli[id]),&(p->semafori->tcb_mutex));

				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[0].direction;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}
		
		/**/

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 2) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[1].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_DUE;
				coccodrillo_init.type = gameData->flussi[1].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;

				Params_coccodrilli arg_coccodrillo = {p, coccodrillo_init};
				pthread_t pid_coccodrillo = avviaCoccodrilloThread( &(arg_coccodrillo), id);
				
				//int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
				//gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				usleep(8000);
				gameData->pids.pidCoccodrilli[id] = leggiTid(&(gameData->allTCB->tcb_coccodrilli[id]),&(p->semafori->tcb_mutex));
				
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[1].direction;;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}
		
		if (thereIsSpaceForCoccodrilloOnFila(gameData, 3) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[2].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_TRE;
				coccodrillo_init.type = gameData->flussi[2].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;

				Params_coccodrilli arg_coccodrillo = {p, coccodrillo_init};
				pthread_t pid_coccodrillo = avviaCoccodrilloThread( &(arg_coccodrillo), id);

				//int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
				//gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				
				usleep(8000);
				gameData->pids.pidCoccodrilli[id] = leggiTid(&(gameData->allTCB->tcb_coccodrilli[id]),&(p->semafori->tcb_mutex));

				
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[2].direction;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 5) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{
			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[4].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_CINQUE;
				coccodrillo_init.type = gameData->flussi[4].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;
				
				Params_coccodrilli arg_coccodrillo = {p, coccodrillo_init};
				pthread_t pid_coccodrillo = avviaCoccodrilloThread( &(arg_coccodrillo), id);
				//int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
				//gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				
				usleep(8000);
				gameData->pids.pidCoccodrilli[id] = leggiTid(&(gameData->allTCB->tcb_coccodrilli[id]),&(p->semafori->tcb_mutex));

				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[4].direction;;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 7) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{
			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[6].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_SETTE;
				coccodrillo_init.type = gameData->flussi[6].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;
				
				Params_coccodrilli arg_coccodrillo = {p, coccodrillo_init};
				pthread_t pid_coccodrillo = avviaCoccodrilloThread( &(arg_coccodrillo), id);
				
				//int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
				//gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				
				usleep(8000);
				gameData->pids.pidCoccodrilli[id] = leggiTid(&(gameData->allTCB->tcb_coccodrilli[id]),&(p->semafori->tcb_mutex));
				
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[6].direction;;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 8) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[7].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_OTTO;
				coccodrillo_init.type =  gameData->flussi[7].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;

				Params_coccodrilli arg_coccodrillo = {p, coccodrillo_init};
				pthread_t pid_coccodrillo = avviaCoccodrilloThread( &(arg_coccodrillo), id);

				//int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
				//gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;

				usleep(8000);
				gameData->pids.pidCoccodrilli[id] = leggiTid(&(gameData->allTCB->tcb_coccodrilli[id]),&(p->semafori->tcb_mutex));
				
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[7].direction;;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 6) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[5].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_SEI;
				coccodrillo_init.type = gameData->flussi[5].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;

				Params_coccodrilli arg_coccodrillo = {p, coccodrillo_init};
				pthread_t pid_coccodrillo = avviaCoccodrilloThread( &(arg_coccodrillo), id);


				//int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
				//gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				
				usleep(8000);
				gameData->pids.pidCoccodrilli[id] = leggiTid(&(gameData->allTCB->tcb_coccodrilli[id]),&(p->semafori->tcb_mutex));
				
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[5].direction;;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		if (thereIsSpaceForCoccodrilloOnFila(gameData, 4) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{

			// avvia coccodrillo
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != -1)
			{
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[3].direction==1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
				coccodrillo_init.y = FILA_QUATTRO;
				coccodrillo_init.type = gameData->flussi[3].direction==1 ? 'C' : 'c';
				coccodrillo_init.id = id;

				Params_coccodrilli arg_coccodrillo = {p, coccodrillo_init};
				pthread_t pid_coccodrillo = avviaCoccodrilloThread( &(arg_coccodrillo), id);
				
				//int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), id);
				//gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				
				usleep(8000);
				gameData->pids.pidCoccodrilli[id] = leggiTid(&(gameData->allTCB->tcb_coccodrilli[id]),&(p->semafori->tcb_mutex));
				
				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[3].direction;;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}

		/**/








		sem_wait(&p->semafori->window_mutex);
		stampaRefreshMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
		sem_post(&p->semafori->window_mutex);
		
		//stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente

		#ifdef DEBUG_PROIETTILI
		for (int i = 0; i < MAXNPROIETTILI; i++)
		{
			mvprintw(8, 146 + (i * 9), "%d:x:%d ", i, gameData->oldPos.proiettili[i].x);
			mvprintw(9, 146 + (i * 9), "%d:y:%d ", i, gameData->oldPos.proiettili[i].y);
			mvprintw(10, 146 + (i * 9), "%d:t:%c ", i, gameData->oldPos.proiettili[i].type);
			mvprintw(11, 146 + (i * 9), "%d:h:%ld ", i, gameData->oldPos.proiettili[i].thread_id);
		}
		
		mvprintw(9,106,"THREAD ID POST UPDATE");
		mvprintw(10,106,"                                ");
		mvprintw(10,106,"%ld",gameData->pids.pidProiettili[0]);
		mvprintw(11,106,"                                ");
		mvprintw(11,106,"%ld",gameData->pids.pidProiettili[1]);
		mvprintw(12,106,"                                ");
		mvprintw(12,106,"%ld",gameData->pids.pidProiettili[2]);

		mvprintw(16,106,"                                ");
		mvprintw(16,106,"CONT PROIETTILI POST UPDATE: %d",gameData->contatori.contProiettili);

		mvprintw(25,106,"TCB PROIETTILI POST UPDATE per indice");
		mvprintw(26,106,"                                          ");
		mvprintw(26,106,"0: %ld %d %d",gameData->allTCB->tcb_proiettili[0].thread_id,gameData->allTCB->tcb_proiettili[0].is_target,gameData->allTCB->tcb_proiettili[0].is_terminated);
		mvprintw(27,106,"                                          ");
		mvprintw(27,106,"1: %ld %d %d",gameData->allTCB->tcb_proiettili[1].thread_id,gameData->allTCB->tcb_proiettili[1].is_target,gameData->allTCB->tcb_proiettili[1].is_terminated);
		mvprintw(28,106,"                                          ");
		mvprintw(28,106,"2: %ld %d %d",gameData->allTCB->tcb_proiettili[2].thread_id,gameData->allTCB->tcb_proiettili[2].is_target,gameData->allTCB->tcb_proiettili[2].is_terminated);
		
		#endif


		#ifdef DEBUG_COCCODRILLI
		for (int i = 0; i < 3; i++)
		{
			mvprintw(8, 146 + (i * 9), "%d:x:%d ", i,   gameData->oldPos.coccodrilli[i].x);
			mvprintw(9, 146 + (i * 9), "%d:y:%d ", i,   gameData->oldPos.coccodrilli[i].y);
			mvprintw(10, 146 + (i * 9), "%d:t:%c ", i,  gameData->oldPos.coccodrilli[i].type);
			mvprintw(11, 146 + (i * 9), "%d:h:%ld ", i, gameData->oldPos.coccodrilli[i].thread_id);
		}
		
		mvprintw(9,106,"THREAD ID POST UPDATE");
		mvprintw(10,106,"                                ");
		mvprintw(10,106,"%ld",gameData->pids.pidCoccodrilli[0]);
		mvprintw(11,106,"                                ");
		mvprintw(11,106,"%ld",gameData->pids.pidCoccodrilli[1]);
		mvprintw(12,106,"                                ");
		mvprintw(12,106,"%ld",gameData->pids.pidCoccodrilli[2]);

		mvprintw(16,106,"                                ");
		mvprintw(16,106,"CONT COCCODRILLI POST UPDATE: %d",gameData->contatori.contCoccodrilli);

		mvprintw(25,106,"TCB PROIETTILI POST UPDATE per indice");
		mvprintw(26,106,"                                          ");
		mvprintw(26,106,"0: %ld %d %d",gameData->allTCB->tcb_coccodrilli[0].thread_id,gameData->allTCB->tcb_coccodrilli[0].is_target,gameData->allTCB->tcb_coccodrilli[0].is_terminated);
		mvprintw(27,106,"                                          ");
		mvprintw(27,106,"1: %ld %d %d",gameData->allTCB->tcb_coccodrilli[1].thread_id,gameData->allTCB->tcb_coccodrilli[1].is_target,gameData->allTCB->tcb_coccodrilli[1].is_terminated);
		mvprintw(28,106,"                                          ");
		mvprintw(28,106,"2: %ld %d %d",gameData->allTCB->tcb_coccodrilli[2].thread_id,gameData->allTCB->tcb_coccodrilli[2].is_target,gameData->allTCB->tcb_coccodrilli[2].is_terminated);
		;
		#endif

		
		//sem_wait((&arg_general)->mutex);
		mvprintw(30,106,"                     ");
		mvprintw(30,106,"errore gameData: %d",gameData->errore);
		//sem_post((&arg_general)->mutex);

		mvprintw(31,106,"                         ");
		mvprintw(31,106,"ViteRana da gameData: %2d",gameData->gameInfo.vite);

		mvprintw(32,106,"                  ");
		mvprintw(32,106,"count Nemici: %2d",gameData->contatori.contNemici);

		mvprintw(33,106,"                  ");
		mvprintw(33,106,"nemico #0: x %2d",gameData->oldPos.nemici[0].x);

		mvprintw(34,106,"                     ");
		mvprintw(34,106,"proiettili_Nem: %2d",gameData->contatori.contProiettiliN);

		mvprintw(36,106,"                     ");
		mvprintw(36,106,"time(sec): %5d",gameData->gameInfo.tempo.secondi);
		mvprintw(36,136,"time(start): %5d",(int)gameData->gameInfo.tempo.start);

		mvprintw(37,106,"diffTime): %5d", (int)difftime( gameData->gameInfo.tempo.current, gameData->gameInfo.tempo.start));

		
		fflush(stdout);
		fflush(stdin);
		fflush(stderr);
		
		sem_wait(&p->semafori->window_mutex);
		refresh(); // Aggiorna la finestra
		sem_post(&p->semafori->window_mutex);
		//sleep(1);

	}// end game

	

	// if sul tipo di uscita dal gioco
	if(isWin(gameData)){
		stampaWin();
	}
	else{
		stampaGameOver();
	}
	
	//free(gameData);
	pthread_exit(NULL);
  	// return NULL;  
}


pthread_t avviaDrawThread(Params *thread_args){
	pthread_t tid_draw;
	if( pthread_create(&tid_draw, NULL, &drawThread, (void*)thread_args) != 0){
		perror("ERR: Avvio DrawThread Fallito!");
		_exit(1); 
	}
	return tid_draw;
}

void avviaNemiciThread(Params* thread_param, int* countdown)
{
	//--------------AVVIO NEMICI / PIANTE ------------------
	//int countdown_piante = *(countdown);

	GameData *gameData = thread_param->gameData;
	/*
	for(int i=0; i<MAXNNEMICI;i++){
		int id = id_disponibile(gameData->pids.pidNemici,MAXNNEMICI);
		if(id!=-1){
			pthread_t nemico = avviaNemicoThread(thread_param, id);
			gameData->pids.pidNemici[id] = nemico;
			//gameData->contatori.contNemici++;
			gameData->contatori.contNemici = (gameData->contatori.contNemici +1) % (MAXNNEMICI+1);
			//gameData->pids.pidNemici[id]=avviaNemico(gameData->pipe,id);
		}
	}
	/**/

	if(gameData->contatori.contNemici < MAXNNEMICI-1 && *countdown ==0 ){
			
		int nemicoID = rand()%MAXNNEMICI;
		//int nemicoID = id_disponibile(p->gameData->pids.pidNemici,MAXNNEMICI);
		
		if(nemicoID >= 0 && (gameData->pids.pidNemici[nemicoID]==0)){
		//if(nemicoID >= 0){
			pthread_t nemico = avviaNemicoThread(thread_param, nemicoID);
			gameData->pids.pidNemici[nemicoID] = nemico;
			//gameData->contatori.contNemici++;
			gameData->contatori.contNemici = (gameData->contatori.contNemici +1) % (MAXNNEMICI+1);
			
			*countdown = rand()%100;
			//power_naps_piante = rand()%1000;
		}
	}
	*countdown = (*countdown > 0) ? (*countdown-1) : rand()%100; // se >0 decrementa, altrimenti reimposta valore
	//*(countdown) = countdown_piante;	// assegna il nuovo valore a countdown 
	/**/
}// end avviaNemiciThread

void stampaWin(){
	clear();
	stampaBox();
	stampaLogoMenu(STARTROWLOGOMENU,STARTCOLLOGOMENU);
	mvprintw(15,80,"you win!!!!!!!!!!!!!");
	refresh();
	usleep(30000000);
	return;
}

void stampaGameOver(){
	clear();
	stampaBox();
	stampaLogoMenu(STARTROWLOGOMENU,STARTCOLLOGOMENU);
	mvprintw(15,80,"Game Over!");
	refresh();
	usleep(3000000);
	return;
}

void printRana(PipeData* datiRana, Sprite* sprite,GameData* gameData){
	int startRow=datiRana->y;
    int startCol=datiRana->x;
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;
    
    int row=0, col=0;

	Schermo* schermo = &(gameData->schermo);
	
    for (int i = 0; i < maxRows; i++) {
        for (int j = 0; j < maxCols; j++) {
            row = startRow + i;
            col = startCol + j;
		
            schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
            schermo->screenMatrix[row][col].color = sprite->color;
            schermo->screenMatrix[row][col].is_changed = true;
            schermo->screenMatrix[row][col].id=datiRana->id;
    	}
    }
	return;
}


bool thereIsSpaceForCoccodrilloOnFila(GameData *gameData, int fila)
{
	int begin_row = 0;
	int begin_col = FIRSTGAMECOL;
	bool space = false;
	if (gameData->contatori.contCoccodrilli < MAXNCOCCODRILLI)
	{

		switch (fila)
		{
		case 1:
			begin_row = FILA_UNO;
			break;
		case 2:
			begin_row = FILA_DUE;
			begin_col = INITCOCCODRILLOSXCOL;
			break;
		case 3:
			begin_row = FILA_TRE;
			break;
		case 4:
			begin_row = FILA_QUATTRO;
			begin_col = INITCOCCODRILLOSXCOL;
			break;
		case 5:
			begin_row = FILA_CINQUE;
			break;
		case 6:
			begin_row = FILA_SEI;
			begin_col = INITCOCCODRILLOSXCOL;
			break;
		case 7:
			begin_row = FILA_SETTE;
			break;
		case 8:
			begin_row = FILA_OTTO;
			begin_col = INITCOCCODRILLOSXCOL;
			break;
		default:
			break;
		}

		for (int i = begin_row; i < begin_row + COCCODRILLO_H; i++)
		{
			for (int j = begin_col; j < begin_col + COCCODRILLO_W; j++)
			{
				if (gameData->schermo.screenMatrix[i][j].tipo == FIUME_OBJ)
				{
					space = true;
				}
			}
		}
	}
	return space;
}
