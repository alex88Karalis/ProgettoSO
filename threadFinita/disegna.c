#include "disegna.h"


/** @brief Il Thread che disegna e legge da buffer 
 * @param param: parametri del thread (Param *)
*/
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

	int countdown_piante = rand() % (100); // = 0
	int sec;
	int contatore_pari = 0;
	int contatore_dispari = 0;

	// loop principale di gioco
	while (!isGameOver(gameData))
	{
		leggiDaBuffer(&(arg_general), &(gameData->pipeData));

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

		//----------------- AVVIO COCCODRILLI ------------------
		// Prende il tempo e incrementa contatori
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
		// Gestisce la comparsa dei coccodrilli
		spawnCoccodrilli(gameData,sec,contatore_dispari,p);

		// aggiorna matrice
		sem_wait(&p->semafori->window_mutex);
		stampaRefreshMatrice(gameData->schermo.screenMatrix); 
		sem_post(&p->semafori->window_mutex);
		
		// Aggiorna la finestra
		sem_wait(&p->semafori->window_mutex);
		refresh(); 
		sem_post(&p->semafori->window_mutex);

	} // end game

	// if sul tipo di uscita dal gioco
	if (isWin(gameData))
	{
		stampaWin(gameData);
	}
	else
	{
		stampaGameOver(gameData);
	}

	// free(gameData);
	pthread_exit(NULL);
	// return NULL;
}


/** @brief Avvia il thread Disegna e restituisce il suo thread_id.
 *  @param thread_args: Parametri da passare al thread: semafori, buffer, e GameData.
*/
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

/** @brief Gestore della comparsa dei Coccodrilli
 * @param gameData Struttura generale con i dati di gioco
 * @param sec Secondi trascorsi
 * @param contatore_dispari Contatore usato per la generazione dei coccodrilli 
 * @param p Struttura con i parametri per i thread
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


/** @brief stampa logo, la schermata di win e punteggio 
 * @param gameData Struttura da cui prendere il punteggio
*/
void stampaWin(GameData *gameData)
{
	clear();
	stampaBox(0); // in utilities.c
	stampaLogo(STARTROWLOGO, STARTCOLLOGO); // in logo.c
	mvprintw(15, 80, "you win!!!!!!!!!!!!!");
	mvprintw(17, 80, "SCORE: %d",gameData->gameInfo.punteggio);
	mvprintw(25, 50, "Premi un tasto per uscire");
	refresh();
	debugBlock();
	return;
}


/** @brief stampa logo, schermata di lose e punteggio 
 * @param gameData Struttura da cui prendere il punteggio
*/
void stampaGameOver(GameData *gameData)
{
	clear();
	stampaBox(0);
	stampaLogo(STARTROWLOGO, STARTCOLLOGO);
	mvprintw(15, 80, "Game Over!");
	mvprintw(17, 80, "SCORE: %d",gameData->gameInfo.punteggio);
	mvprintw(25, 50, "Premi un tasto per uscire");
	refresh();
	debugBlock();
	return;
}

/** @brief stampa lo sprite della rana
 * @param datiRana : i dati della rana tipo PipeData
 * @param sprite : la sprite della rana tipo Sprite
 * @param gameData : per la gestione dello schermo
*/
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


/** @brief Determina se sulla corsia del fiume ci sia spazio per un nuovo coccodrillo
 * @param gameData Strutura generale con i dati di gioco
 * @param fila numero della corsia in cui cercare se c'è spazio
 * @return TRUE se c'è spazio, FALSE altrimenti.
 */
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
