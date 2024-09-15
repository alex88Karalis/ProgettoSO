#include "disegna.h"
#include "coccodrillo.h"

#define DEBUG
// #define FAKE_RANA
// #define DEBUG_PROIETTILI
#define DEBUG_COCCODRILLI

void *buff_Reader(void *param)
{
	Params *p = (Params *)param;
	GameData *gameData = p->gameData; // recupera puntantore a gameData
	while (1)
	{

		leggiDaBuffer(p, &(gameData->pipeData));

		sem_wait(&p->semafori->tcb_mutex);

		aggiorna(p, gameData);						   // aggiorna stato del gioco
		printTempo(gameData);						   // aggiorna hud del tempo
		stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
		sem_post(&p->semafori->tcb_mutex);
	}
	pthread_exit(NULL);
}

void *buff_Writer(void *param)
{
	Params *p = (Params *)param;
	// GameData* gameData = p->gameData; //recupera puntantore a gameData
	ThreadControlBlock new_tcb = {pthread_self(), false, false};

	PipeData fakeRana = {1, 0, 'X', 0, pthread_self()};
	copiaTCB(p->gameData->allTCB->tcb_rana, new_tcb, &p->semafori->tcb_mutex);
	// PipeData newItem = {1,0, 'X', 0, 0};
	while (1)
	{
		scriviSuBuffer(p, fakeRana, p->gameData->allTCB->tcb_rana, false);
		usleep(10000);
	}
	pthread_exit(NULL);
}

// Thread di DEBUG che simula il thread Rana muovendola lateralmente
void *fakeRana(void *param)
{
	Params *p = (Params *)param;
	ThreadControlBlock new_tcb = {pthread_self(), false, false};

	PipeData fakeRana = {1, 0, 'X', 0, pthread_self()};
	copiaTCB(p->gameData->allTCB->tcb_rana, new_tcb, &p->semafori->tcb_mutex);
	// PipeData newItem = {1,0, 'X', 0, 0};
	while (1)
	{

		if (isThreadTarget(p->gameData->allTCB->tcb_rana, &p->semafori->tcb_mutex))
		{
			fakeRana.x = -1;
		}
		else
		{
			fakeRana.x = 1;
		}

		scriviSuBuffer(p, fakeRana, p->gameData->allTCB->tcb_rana, false);
		usleep(10000);
	}
	pthread_exit(NULL);
}

void *drawThread(void *param)
{

	Params *p = (Params *)param;

	int *pipe_fd = NULL;
	GameData *gameData = p->gameData; // recupera indirizzio di gameData dai parametri

	Params arg_general = {p->semafori, p->buffer, gameData, 0};

	// inizializza i dati del gioco
	inizializza(gameData, pipe_fd);

	avviaTempoThread(p);

	avviaThreadBase(&(gameData->pids), &(arg_general)); // avvia la Rana

	usleep(1000);
	
	if (gameData->allTCB->tcb_rana->thread_id == 123)
	{
		beep();
	};

	// prima stampa dello schermo
	printRana(&(gameData->oldPos.rana), &(gameData->sprites[S_RANA]), gameData);

	sem_wait(&p->semafori->window_mutex);
	stampaRefreshMatrice(gameData->schermo.screenMatrix);
	sem_post(&p->semafori->window_mutex);

	// int power_naps_piante = rand()%(100);

	int countdown_piante = rand() % (100); // = 0
	int sec;
	int contatore_pari = 0;
	int contatore_dispari = 0;

	// loop principale di gioco
	while (!isGameOver(gameData))
	{
		
		leggiDaBuffer(&(arg_general), &(gameData->pipeData));

		if (gameData->pipeData.type == 'p' && isThreadTerminated(&gameData->allTCB->tcb_proiettili_nemici[gameData->pipeData.id], &(p->semafori->tcb_mutex)))
		{
			mvprintw(23, 156, "                          ");
			mvprintw(24, 156, "                          ");
			mvprintw(25, 156, "                          ");
			mvprintw(26, 156, "                          ");
			mvprintw(27, 156, "                          ");

			mvprintw(23, 156, "last pipeData a terminated");
			mvprintw(24, 156, "x: %d", gameData->pipeData.x);
			mvprintw(25, 156, "y: %d", gameData->pipeData.y);
			mvprintw(26, 156, "tipo: %c", gameData->pipeData.type);
			mvprintw(27, 156, "id: %d", gameData->pipeData.id);
		}

#ifdef DEBUG
		// per debug
		mvprintw(0, 106, "                                      ");
		mvprintw(0, 106, "pipeData: %c x: %d y: %d id: %d", gameData->pipeData.type, gameData->pipeData.x, gameData->pipeData.y, gameData->pipeData.id);

		// mvprintw(0,146,"MAXint: %d", INT16_MAX);

		mvprintw(1, 106, "                                      ");
		mvprintw(1, 106, "RANA tid: %ld", gameData->allTCB->tcb_rana->thread_id);

		mvprintw(2, 106, "               ");
		mvprintw(2, 106, "Pos rana x: %d y: %d", gameData->ranaAbsPos.x, gameData->ranaAbsPos.y);

		mvprintw(3, 106, "               ");
		mvprintw(3, 106, "MAX Proiettili : %d ", (int)(sizeof(gameData->allTCB->tcb_proiettili) / sizeof(ThreadControlBlock)));

#ifdef DEBUG_PROIETTILI
		mvprintw(4, 106, "THREAD ID BEFORE UPDATE");
		mvprintw(5, 106, "                                ");
		mvprintw(5, 106, "%ld", gameData->pids.pidProiettili[0]);
		mvprintw(6, 106, "                                ");
		mvprintw(6, 106, "%ld", gameData->pids.pidProiettili[1]);
		mvprintw(7, 106, "                                ");
		mvprintw(7, 106, "%ld", gameData->pids.pidProiettili[2]);

		mvprintw(20, 106, "TCB PROIETTILI PRE UPDATE per indice");
		mvprintw(21, 106, "                                          ");
		mvprintw(21, 106, "0: %ld %d %d", gameData->allTCB->tcb_proiettili[0].thread_id, gameData->allTCB->tcb_proiettili[0].is_target, gameData->allTCB->tcb_proiettili[0].is_terminated);
		mvprintw(22, 106, "                                          ");
		mvprintw(22, 106, "1: %ld %d %d", gameData->allTCB->tcb_proiettili[1].thread_id, gameData->allTCB->tcb_proiettili[1].is_target, gameData->allTCB->tcb_proiettili[1].is_terminated);
		mvprintw(23, 106, "                                          ");
		mvprintw(23, 106, "2: %ld %d %d", gameData->allTCB->tcb_proiettili[2].thread_id, gameData->allTCB->tcb_proiettili[2].is_target, gameData->allTCB->tcb_proiettili[2].is_terminated);

		mvprintw(14, 106, "                                ");
		mvprintw(14, 106, "CONT PROIETTILI PRE UPDATE: %d", gameData->contatori.contProiettili);
#endif

#ifdef DEBUG_COCCODRILLI
		mvprintw(4, 106, "COCCODRILLI THREAD ID BEFORE UPDATE");
		mvprintw(5, 106, "                                ");
		mvprintw(5, 106, "%ld", gameData->pids.pidCoccodrilli[0]);
		mvprintw(6, 106, "                                ");
		mvprintw(6, 106, "%ld", gameData->pids.pidCoccodrilli[1]);
		mvprintw(7, 106, "                                ");
		mvprintw(7, 106, "%ld", gameData->pids.pidCoccodrilli[2]);

		mvprintw(20, 106, "TCB COCCODRILLI PRE UPDATE per indice");
		mvprintw(21, 106, "                                          ");
		mvprintw(21, 106, "0: %ld %d %d", gameData->allTCB->tcb_coccodrilli[0].thread_id, gameData->allTCB->tcb_coccodrilli[0].is_target, gameData->allTCB->tcb_coccodrilli[0].is_terminated);
		mvprintw(22, 106, "                                          ");
		mvprintw(22, 106, "1: %ld %d %d", gameData->allTCB->tcb_coccodrilli[1].thread_id, gameData->allTCB->tcb_coccodrilli[1].is_target, gameData->allTCB->tcb_coccodrilli[1].is_terminated);
		mvprintw(23, 106, "                                          ");
		mvprintw(23, 106, "2: %ld %d %d", gameData->allTCB->tcb_coccodrilli[2].thread_id, gameData->allTCB->tcb_coccodrilli[2].is_target, gameData->allTCB->tcb_coccodrilli[2].is_terminated);

		mvprintw(14, 106, "                                ");
		mvprintw(14, 106, "CONT COCCODRILLI PRE UPDATE: %d", gameData->contatori.contCoccodrilli);
#endif

// fine debug
#endif

		aggiorna(&(arg_general), gameData); // aggiorna stato del gioco e gestisce collisioni
		printTempo(gameData);				// aggiorna hud del tempo

		// if(gameData->gameInfo.ranaIsDead){ }
		int n_1 = pulisciThreadMorti(gameData, arg_general.semafori); //  in prova

		if (n_1 == 0 && gameData->gameInfo.vite > 0)
		{

			pulisciSpriteInMatrice(&(gameData->oldPos.rana), &(gameData->sprites[S_RANA]), gameData); // pulisce sprite Rana
			resetManche(p);
			printInitTempo(gameData); // resetta HUD Tempo

			resetRanaThread(&arg_general);
		}
		else
		{
			aggiornaHud(gameData);
		}
		/**/

		/**/

		//--------------AVVIO NEMICI / PIANTE ------------------

		if (gameData->contatori.contNemici < MAXNNEMICI - 1 && countdown_piante == 0)
		{

			int nemicoID = rand() % MAXNNEMICI;
			
			if (nemicoID >= 0 && (gameData->pids.pidNemici[nemicoID] == 0))
			{
				pthread_t nemico = avviaNemicoThread(&arg_general, nemicoID);
				gameData->pids.pidNemici[nemicoID] = nemico;
				gameData->contatori.contNemici = (gameData->contatori.contNemici + 1) % (MAXNNEMICI + 1);

				countdown_piante = rand() % 1000;
			}
		}
		countdown_piante = (countdown_piante > 0) ? countdown_piante - 1 : rand() % 100;

		/**/
		//----------------- AVVIO COCCODRILLI ------------------

		sec = gameData->gameInfo.tempo.secondi;

		if (sec % TEMPO_SPAWN_COCCODRILLI != 0)
		{
			contatore_dispari = 0;
			contatore_pari++;
		}
		else
		{
			contatore_pari = 0;
			contatore_dispari++;
		}

		spawnCoccodrilli(gameData,sec,contatore_dispari,p);

		sem_wait(&p->semafori->window_mutex);
		stampaRefreshMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
		sem_post(&p->semafori->window_mutex);

		// stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente

#ifdef DEBUG_PROIETTILI
		for (int i = 0; i < MAXNPROIETTILI; i++)
		{
			mvprintw(8, 146 + (i * 9), "%d:x:%d ", i, gameData->oldPos.proiettili[i].x);
			mvprintw(9, 146 + (i * 9), "%d:y:%d ", i, gameData->oldPos.proiettili[i].y);
			mvprintw(10, 146 + (i * 9), "%d:t:%c ", i, gameData->oldPos.proiettili[i].type);
			mvprintw(11, 146 + (i * 9), "%d:h:%ld ", i, gameData->oldPos.proiettili[i].thread_id);
		}

		mvprintw(9, 106, "THREAD ID POST UPDATE");
		mvprintw(10, 106, "                                ");
		mvprintw(10, 106, "%ld", gameData->pids.pidProiettili[0]);
		mvprintw(11, 106, "                                ");
		mvprintw(11, 106, "%ld", gameData->pids.pidProiettili[1]);
		mvprintw(12, 106, "                                ");
		mvprintw(12, 106, "%ld", gameData->pids.pidProiettili[2]);

		mvprintw(16, 106, "                                ");
		mvprintw(16, 106, "CONT PROIETTILI POST UPDATE: %d", gameData->contatori.contProiettili);

		mvprintw(25, 106, "TCB PROIETTILI POST UPDATE per indice");
		mvprintw(26, 106, "                                          ");
		mvprintw(26, 106, "0: %ld %d %d", gameData->allTCB->tcb_proiettili[0].thread_id, gameData->allTCB->tcb_proiettili[0].is_target, gameData->allTCB->tcb_proiettili[0].is_terminated);
		mvprintw(27, 106, "                                          ");
		mvprintw(27, 106, "1: %ld %d %d", gameData->allTCB->tcb_proiettili[1].thread_id, gameData->allTCB->tcb_proiettili[1].is_target, gameData->allTCB->tcb_proiettili[1].is_terminated);
		mvprintw(28, 106, "                                          ");
		mvprintw(28, 106, "2: %ld %d %d", gameData->allTCB->tcb_proiettili[2].thread_id, gameData->allTCB->tcb_proiettili[2].is_target, gameData->allTCB->tcb_proiettili[2].is_terminated);

#endif

#ifdef DEBUG_COCCODRILLI
		for (int i = 0; i < 3; i++)
		{
			mvprintw(8, 146 + (i * 9), "%d:x:%d ", i, gameData->oldPos.coccodrilli[i].x);
			mvprintw(9, 146 + (i * 9), "%d:y:%d ", i, gameData->oldPos.coccodrilli[i].y);
			mvprintw(10, 146 + (i * 9), "%d:t:%c ", i, gameData->oldPos.coccodrilli[i].type);
			mvprintw(11, 146 + (i * 9), "%d:h:%ld ", i, gameData->oldPos.coccodrilli[i].thread_id);
		}

		mvprintw(9, 106, "THREAD ID POST UPDATE");
		mvprintw(10, 106, "                                ");
		mvprintw(10, 106, "%ld", gameData->pids.pidCoccodrilli[0]);
		mvprintw(11, 106, "                                ");
		mvprintw(11, 106, "%ld", gameData->pids.pidCoccodrilli[1]);
		mvprintw(12, 106, "                                ");
		mvprintw(12, 106, "%ld", gameData->pids.pidCoccodrilli[2]);

		mvprintw(16, 106, "                                ");
		mvprintw(16, 106, "CONT COCCODRILLI POST UPDATE: %d", gameData->contatori.contCoccodrilli);

		mvprintw(25, 106, "TCB PROIETTILI POST UPDATE per indice");
		mvprintw(26, 106, "                                          ");
		mvprintw(26, 106, "0: %ld %d %d", gameData->allTCB->tcb_coccodrilli[0].thread_id, gameData->allTCB->tcb_coccodrilli[0].is_target, gameData->allTCB->tcb_coccodrilli[0].is_terminated);
		mvprintw(27, 106, "                                          ");
		mvprintw(27, 106, "1: %ld %d %d", gameData->allTCB->tcb_coccodrilli[1].thread_id, gameData->allTCB->tcb_coccodrilli[1].is_target, gameData->allTCB->tcb_coccodrilli[1].is_terminated);
		mvprintw(28, 106, "                                          ");
		mvprintw(28, 106, "2: %ld %d %d", gameData->allTCB->tcb_coccodrilli[2].thread_id, gameData->allTCB->tcb_coccodrilli[2].is_target, gameData->allTCB->tcb_coccodrilli[2].is_terminated);
		;
#endif

		// sem_wait((&arg_general)->mutex);
		mvprintw(30, 106, "                     ");
		mvprintw(30, 106, "errore gameData: %d", gameData->errore);
		// sem_post((&arg_general)->mutex);

		mvprintw(31, 106, "                         ");
		mvprintw(31, 106, "ViteRana da gameData: %2d", gameData->gameInfo.vite);

		mvprintw(32, 106, "                  ");
		mvprintw(32, 106, "count Nemici: %2d", gameData->contatori.contNemici);

		mvprintw(33, 106, "                  ");
		mvprintw(33, 106, "nemico #0: x %2d", gameData->oldPos.nemici[0].x);

		mvprintw(34, 106, "                     ");
		mvprintw(34, 106, "proiettili_Nem: %2d", gameData->contatori.contProiettiliN);

		mvprintw(36, 106, "                     ");
		mvprintw(36, 106, "time(sec): %5d", gameData->gameInfo.tempo.secondi);
		mvprintw(36, 136, "time(start): %5d", (int)gameData->gameInfo.tempo.start);

		mvprintw(37, 106, "diffTime: %5d", (int)difftime(gameData->gameInfo.tempo.current, gameData->gameInfo.tempo.start));
		mvprintw(38,106,"                  ");
		mvprintw(38,106,"cont disp %d",contatore_dispari);

		fflush(stdout);
		fflush(stdin);
		fflush(stderr);

		sem_wait(&p->semafori->window_mutex);
		refresh(); // Aggiorna la finestra
		sem_post(&p->semafori->window_mutex);
		// sleep(1);

	} // end game

	// if sul tipo di uscita dal gioco
	if (isWin(gameData))
	{
		stampaWin();
	}
	else
	{
		stampaGameOver();
	}

	// free(gameData);
	pthread_exit(NULL);
	// return NULL;
}

pthread_t avviaDrawThread(Params *thread_args)
{
	pthread_t tid_draw;
	if (pthread_create(&tid_draw, NULL, &drawThread, (void *)thread_args) != 0)
	{
		perror("ERR: Avvio DrawThread Fallito!");
		_exit(1);
	}
	return tid_draw;
}

/**
 * Gestore della comparsa dei Coccodrilli
 */
void spawnCoccodrilli(GameData *gameData, int sec, int contatore_dispari,Params* p)
{
	for (int fila = 1; fila <= 8; fila++)
	{	
		// a intervalli di tempo controlla se c'è spazio sulla fila
		if (thereIsSpaceForCoccodrilloOnFila(gameData, fila) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != NOID)
			{
				// Imposta coordinate iniziali del coccodrillo, da passare al thread
				PipeData coccodrillo_init;
				coccodrillo_init.x = gameData->flussi[fila-1].direction == 1 ? FIRSTGAMECOL - 9 : LASTGAMECOL; // se è a dx o sx in base alla direzione del flusso
				
				coccodrillo_init.y = fromFilaToRow(fila); // recupera le coordinate y in base alla fila
				coccodrillo_init.type = gameData->flussi[fila-1].direction == 1 ? 'C' : 'c'; // il char in base alla direzione
				coccodrillo_init.id = id;


				Params_coccodrilli arg_coccodrillo = {p, coccodrillo_init};
				pthread_t pid_coccodrillo = avviaCoccodrilloThread(&(arg_coccodrillo), id);	// avvia thread coccodrillo

				gameData->flussi[fila-1].n_coccodrilli_attivi++;	// incrementa conteggio coccodrilli

				usleep(8000);
				gameData->pids.pidCoccodrilli[id] = leggiTid(&(gameData->allTCB->tcb_coccodrilli[id]), &(p->semafori->tcb_mutex));

				gameData->contatori.contCoccodrilli++;
				gameData->controlloCoccodrilli[id].direction = gameData->flussi[fila-1].direction;
				gameData->controlloCoccodrilli[id].fila= fila;
				gameData->controlloCoccodrilli[id].id = id;
				gameData->controlloCoccodrilli[id].offset_deep = 0;
				gameData->controlloCoccodrilli[id].is_buono = false;
				gameData->controlloCoccodrilli[id].is_going_deep = false;
				gameData->controlloCoccodrilli[id].is_going_up = false;
				gameData->controlloCoccodrilli[id].passi = 0;
			}
		}
	}
}

void stampaWin()
{
	clear();
	stampaBox();
	stampaLogoMenu(STARTROWLOGOMENU, STARTCOLLOGOMENU);
	mvprintw(15, 80, "you win!!!!!!!!!!!!!");
	refresh();
	void debugBlock();
	return;
}

void stampaGameOver()
{
	clear();
	stampaBox();
	stampaLogoMenu(STARTROWLOGOMENU, STARTCOLLOGOMENU);
	mvprintw(15, 80, "Game Over!");
	refresh();
	debugBlock();
	return;
}

void printRana(PipeData *datiRana, Sprite *sprite, GameData *gameData)
{
	int startRow = datiRana->y;
	int startCol = datiRana->x;
	int maxRows = sprite->max_row;
	int maxCols = sprite->max_col;

	int row = 0, col = 0;

	Schermo *schermo = &(gameData->schermo);

	for (int i = 0; i < maxRows; i++)
	{
		for (int j = 0; j < maxCols; j++)
		{
			row = startRow + i;
			col = startCol + j;

			schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
			schermo->screenMatrix[row][col].color = sprite->color;
			schermo->screenMatrix[row][col].is_changed = true;
			schermo->screenMatrix[row][col].id = datiRana->id;
		}
	}
	return;
}

bool thereIsSpaceForCoccodrilloOnFila(GameData *gameData, int fila)
{
	int begin_row = 0;
	int begin_col = FIRSTGAMECOL;
	bool space = false;
	if (gameData->flussi[fila - 1].n_coccodrilli_attivi >= 3)
	{
		return false;
	}
	begin_row = fromFilaToRow(fila);
	begin_col = gameData->flussi[fila - 1].direction == 1 ? FIRSTGAMECOL : INITCOCCODRILLOSXCOL;

	if (gameData->contatori.contCoccodrilli < MAXNCOCCODRILLI)
	{

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
