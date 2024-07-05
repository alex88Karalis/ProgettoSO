#include "disegna.h"

#define DEBUG
//#define FAKE_RANA

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
	GameData* gameData = p->gameData; //recupera puntantore a gameData
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
	GameData* gameData = p->gameData; //recupera puntantore a gameData
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



void *drawThread (void *param){
	Params* p = (Params* ) param;
	pthread_t tidDisegna = pthread_self();
	pthread_t tidRana = 123;
	pthread_t tidReader = 0;
	pthread_t tidWriter = 0;

	int* pipe_fd = NULL;
	GameData *gameData = p->gameData; // recupera indirizzio di gameData dai parametri
	
	Params arg_general = {p->semafori, p->buffer, gameData, 0};
	
	// inizializza i dati del gioco
	inizializza(gameData,pipe_fd);


	//pthread_create(&tidRana,NULL,&fakeRana,&arg_general);
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

	int countdown_piante = 0;
	// loop principale di gioco
	while (!isGameOver(gameData)) {

		leggiDaBuffer( &(arg_general), &(gameData->pipeData) );

		if(gameData->pipeData.type=='P' && isThreadTerminated(&gameData->allTCB->tcb_proiettili[gameData->pipeData.id],&(p->semafori->tcb_mutex) )) {
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

		mvprintw(1,106,"                                      ");
		mvprintw(1,106,"RANA tid: %ld",gameData->allTCB->tcb_rana->thread_id);

		mvprintw(2,106,"               ");
		mvprintw(2,106,"Pos rana x: %d y: %d",gameData->ranaAbsPos.x, gameData->ranaAbsPos.y);
		
		mvprintw(3,106,"               ");
		mvprintw(3,106,"MAX Proiettili : %d ", (int)(sizeof(gameData->allTCB->tcb_proiettili)/sizeof(ThreadControlBlock)));

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
		
		// fine debug
		#endif
		
		//if(isThreadTarget(gameData->allTCB->tcb_rana,&p->semafori->tcb_mutex)){}

		aggiorna(&(arg_general), gameData); // aggiorna stato del gioco e gestisce collisioni
		printTempo(gameData); // aggiorna hud del tempo
		
		//int n = pulisciThreadMorti(arg_general.semafori, gameData->allTCB);  //per ora solo per (la Rana e) proiettili
		
		
		int n_1 = pulisciThreadMorti_2(gameData, arg_general.semafori); //  in prova
		if(n_1 == 0 && gameData->gameInfo.vite > 0){
			//cancellaOggettoDaMatrice(gameData,&(gameData->oldPos.rana), &gameData->oldPos.rana, S_RANA);
			resetRanaThread(&arg_general);
			aggiornaHud(gameData);
		}else{
			aggiornaHud(gameData);
		}
		/**/
		
		//if(n_1 == 1){		aggiornaHud(gameData);}

		/*
		if(n == 0  && gameData->gameInfo.vite > 0){		// la rana è morta ma ha ancora vite	
			// reset RANA 
			//resetRanaThread(&arg_general);
			//printRana(&(gameData->oldPos.rana),&(gameData->sprites[S_RANA]),gameData);
			//aggiornaHud(gameData);
		}else if (n == 1){ //join su proiettile
			gameData->pids.pidProiettili[gameData->pipeData.id] = 0;
			aggiornaHud(gameData);
			//beep();
		}else{			// nessun thread è terminato
		}
		/**/

		//--------------AVVIO NEMICI / PIANTE ------------------
		// TODO - aggiustare cancellazione piante 

		if(gameData->contatori.contNemici < MAXNNEMICI && countdown_piante==0 ){
			//int nemicoID = 2;
			int nemicoID = id_disponibile(p->gameData->pids.pidNemici,MAXNNEMICI);
			if(nemicoID >= 0){
				pthread_t nemico = avviaNemicoThread(&arg_general, nemicoID);
				gameData->pids.pidNemici[nemicoID] = nemico;
				//gameData->contatori.contNemici++;
				gameData->contatori.contNemici = (gameData->contatori.contNemici +1) % (MAXNNEMICI+1);

			}
		}
		countdown_piante = (countdown_piante+1) %100;



		sem_wait(&p->semafori->window_mutex);
		stampaRefreshMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
		sem_post(&p->semafori->window_mutex);
		
		//stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente


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

		for (int i = 0; i < MAXNPROIETTILI; i++)
		{
			mvprintw(8, 146 + (i * 9), "%d:x:%d ", i, gameData->oldPos.proiettili[i].x);
			mvprintw(9, 146 + (i * 9), "%d:y:%d ", i, gameData->oldPos.proiettili[i].y);
			mvprintw(10, 146 + (i * 9), "%d:t:%c ", i, gameData->oldPos.proiettili[i].type);
			mvprintw(11, 146 + (i * 9), "%d:h:%ld ", i, gameData->oldPos.proiettili[i].thread_id);
		}

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


