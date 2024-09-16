#include "../hdr/disegna.h"

void avviaDrawProcess(int pipe_fd[2])
{
	pid_t draw_pid = fork();
	if (draw_pid < 0)
	{
		perror("Fork failed");
		exit(1);
	}
	else if (draw_pid == 0)
	{
		// Processo "disegna"
		drawProcess(pipe_fd);
		exit(0);
	}
}

void drawProcess(int *pipe_fd)
{
	// la struttura che contiene i dati per la gestione del gioco
	GameData *gameData = (GameData *)malloc(sizeof(GameData));

	int pipe_suoni_fd[2];	 // Pipe file descriptors
	creaPipe(pipe_suoni_fd); // Crea la pipe

	gameData->pipe_suoni = pipe_suoni_fd;
	gameData->pipe = pipe_fd;

	gameData->randomSeed =  time(NULL) ^ (getpid() << 16);

	inizializzaPid(gameData);

	gameData->pids.pidRana = avviaRana(gameData->pipe);
	gameData->pids.pidTempo = avviaTempo(gameData->pipe);
	gameData->pids.soundPlayer = avvia_soundPlayer(gameData->pipe_suoni);
	gameData->pids.pidMusica = avviaMusica();

	// inizializza i dati del gioco
	inizializza(gameData);

	// prima stampa dello schermo
	printRana(&(gameData->oldPos.rana), &(gameData->sprites[S_RANA]), gameData);
	stampaMatrice(gameData->schermo.screenMatrix);

	refresh();

	int sec;
	int contatore_pari = 0;
	int contatore_dispari = 0;

	// loop principale di gioco
	while (!isGameOver(gameData))
	{

		read(gameData->pipe[0], &(gameData->pipeData), sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe

		// suoni di movimento
		switch (gameData->pipeData.type)
		{
		case 'X': // rana
			PipeData dataSound;
			dataSound.type = 'F';
			write(gameData->pipe_suoni[1], &dataSound, sizeof(PipeData));
			break;
		default:
			break;
		}


		aggiorna(gameData); // aggiorna stato del gioco

		printVite(gameData);
		printManche(gameData);
		printScore(gameData);
		aggiornaHud(gameData);

		sec = gameData->gameInfo.secondi_di_gioco;

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

		// se il tempo di gioco supera il max per manche la rana muore e la manche riparte
		if (gameData->gameInfo.secondi_di_gioco > TEMPOLVL1)
		{
			// imposto rana is dead a true
			gameData->gameInfo.ranaIsDead = true;
		}

		// se la rana è morta
		if (gameData->gameInfo.ranaIsDead)
		{
			// tolgo una vita
			gameData->gameInfo.vite--;

			// schermata vite --
			printViteMinus();
			// termino tutti i processi, pulisco tutte le sprite, refresh della schermata di gioco
			terminaTuttiProcessi(gameData);
			pulisciTutteLeSprite(gameData);

			inizializzaFlagMatrice(gameData->schermo.screenMatrix);
			stampaMatrice(gameData->schermo.screenMatrix);
			refresh();

			if (!isGameOver(gameData))
			{
				// chiudo pipe
				close(gameData->pipe[0]);
				close(gameData->pipe[1]);
				int pipe[2];
				creaPipe(pipe);
				gameData->pipe = pipe;

				// faccio ripartire tutto
				gameData->pids.pidRana = avviaRana(gameData->pipe);
				gameData->pids.pidTempo = avviaTempo(gameData->pipe);
				gameData->pids.soundPlayer = avvia_soundPlayer(gameData->pipe_suoni);
				gameData->pids.pidMusica = avviaMusica();

				inizializzaPosRana(gameData);
				printRana(&(gameData->oldPos.rana), &(gameData->sprites[S_RANA]), gameData);
				inizializzaFlussi(gameData);
				printInitTempo(gameData);
				printVite(gameData);
				stampaMatrice(gameData->schermo.screenMatrix);
				inizializzaOldPos(&(gameData->oldPos));
				inizializzaContatori(gameData);
				inizializzaControlloCoccodrilli(gameData);
			}

			gameData->gameInfo.ranaIsDead = false;
		}

		// se si è vinta una manche
		if (gameData->gameInfo.mancheWin)
		{
			printSchermataManche();
			// termino tutti i processi, pulisco tutte le sprite, refresh della schermata di gioco
			terminaTuttiProcessi(gameData);
			pulisciTutteLeSprite(gameData);

			inizializzaFlagMatrice(gameData->schermo.screenMatrix);
			stampaMatrice(gameData->schermo.screenMatrix);
			refresh();
			if(!isWin(gameData)){
			// reset pipe
			close(gameData->pipe[0]);
			close(gameData->pipe[1]);
			int pipe[2];
			creaPipe(pipe);
			gameData->pipe = pipe;

			// faccio ripartire i processi principali
			gameData->pids.pidRana = avviaRana(gameData->pipe);
			gameData->pids.pidTempo = avviaTempo(gameData->pipe);
			gameData->pids.soundPlayer = avvia_soundPlayer(gameData->pipe_suoni);
			gameData->pids.pidMusica = avviaMusica();

			// inizializzazione manche
			inizializzaPosRana(gameData);
			printRana(&(gameData->oldPos.rana), &(gameData->sprites[S_RANA]), gameData);
			inizializzaFlussi(gameData);
			printInitTempo(gameData);
			printVite(gameData);
			stampaMatrice(gameData->schermo.screenMatrix);
			inizializzaOldPos(&(gameData->oldPos));
			inizializzaContatori(gameData);
			inizializzaControlloCoccodrilli(gameData);

			
			}
			gameData->gameInfo.mancheWin = false;
			
		}

		spawnCoccodrilli(gameData, sec, contatore_dispari);

		avviaNemici(gameData, sec, contatore_dispari);

		refresh();
	}

	// if sul tipo di uscita dal gioco
	if (isWin(gameData))
	{
		printSchermataWin(gameData->gameInfo.punteggio);
	}
	else
	{
		printSchermataGameOver();
	}

	terminaTuttiProcessi(gameData);
	free(gameData);
	return;
}

void spawnCoccodrilli(GameData *gameData, int sec, int contatore_dispari)
{
	for (int fila = 1; fila <= 8; fila++)
	{
		if (thereIsSpaceForCoccodrilloOnFila(gameData, fila) && sec % TEMPO_SPAWN_COCCODRILLI == 0 && contatore_dispari == 1)
		{
			int id = id_disponibile(gameData->pids.pidCoccodrilli, MAXNCOCCODRILLI);
			if (id != NOID)
			{
				PipeData coccodrillo_init;
				inizializzaCoccodrillo(&(coccodrillo_init), gameData, fila, id);
				int pid_coccodrillo = avviaCoccodrillo(gameData->pipe, &(coccodrillo_init), gameData->flussi[fila - 1].direction, gameData->flussi[fila - 1].vel);
				gameData->pids.pidCoccodrilli[id] = pid_coccodrillo;
				gameData->flussi[fila - 1].n_coccodrilli_attivi++;
				gameData->contatori.contCoccodrilli++;
				gameData->oldPos.coccodrilli[id].id = id;
			}
		}
	}
}

void inizializzaCoccodrillo(PipeData *coccodrillo_init, GameData *gameData, int fila, int id)
{
	int assigned_row = fromFilaToRow(fila);

	coccodrillo_init->x = gameData->flussi[fila - 1].direction == 1 ? FIRSTGAMECOL - 9 : LASTGAMECOL;
	coccodrillo_init->y = assigned_row;
	coccodrillo_init->type = gameData->flussi[fila - 1].direction == 1 ? 'C' : 'c';
	coccodrillo_init->id = id;

	gameData->controlloCoccodrilli[id].fila = fila;
	gameData->controlloCoccodrilli[id].direction = gameData->flussi[fila - 1].direction;
	gameData->controlloCoccodrilli[id].id = id;
	gameData->controlloCoccodrilli[id].offset_deep = 0;
	gameData->controlloCoccodrilli[id].is_buono = false;
	gameData->controlloCoccodrilli[id].is_going_deep = false;
	gameData->controlloCoccodrilli[id].is_going_up = false;
	gameData->controlloCoccodrilli[id].passi = 0;
	return;
}

void pulisciTutteLeSprite(GameData *gameData)
{
	// rana
	pulisciSpriteInMatrice(&(gameData->oldPos.rana), &(gameData->sprites[S_RANA]), gameData);

	// coccodrilli
	for (int i = 0; i < MAXNCOCCODRILLI; i++)
	{
		if (gameData->oldPos.coccodrilli[i].id != NOID)
		{
			pulisciSpriteInMatrice(&(gameData->oldPos.coccodrilli[i]), &(gameData->sprites[S_COCCODRILLO_DX]), gameData);
		}
	}

	// nemici
	for (int i = 0; i < MAXNNEMICI; i++)
	{
		if (gameData->oldPos.nemici[i].id != NOID)
		{
			pulisciSpriteInMatrice(&(gameData->oldPos.nemici[i]), &(gameData->sprites[S_PIANTA]), gameData);
		}
	}

	// proiettili amici
	for (int i = 0; i < MAXNPROIETTILI; i++)
	{
		if (gameData->oldPos.proiettili[i].id != NOID)
		{
			pulisciSpriteInMatrice(&(gameData->oldPos.proiettili[i]), &(gameData->sprites[S_PROIETTILE]), gameData);
		}
	}

	// proiettili nemici
	for (int i = 0; i < MAXNPROIETTILINEMICI; i++)
	{
		if (gameData->oldPos.proiettiliNemici[i].id != NOID)
		{
			pulisciSpriteInMatrice(&(gameData->oldPos.proiettiliNemici[i]), &(gameData->sprites[S_PROIETTILE_NEMICO]), gameData);
		}
	}
}

void terminaTuttiProcessi(GameData *gameData)
{
	// terminazione rana
	if (gameData->pids.pidRana != NOPID)
	{
		kill(gameData->pids.pidRana, SIGKILL);
		waitpid(gameData->pids.pidRana, NULL, 0);
		gameData->pids.pidRana = NOPID;
	}

	// terminazione di tutti i coccodrilii
	for (int i = 0; i < MAXNCOCCODRILLI; i++)
	{
		if (gameData->pids.pidCoccodrilli[i] != NOPID)
		{
			kill(gameData->pids.pidCoccodrilli[i], SIGKILL);
			waitpid(gameData->pids.pidCoccodrilli[i], NULL, 0);
			gameData->pids.pidCoccodrilli[i] = NOPID;
		}
	}

	// terminazione di tutti i nemici
	for (int i = 0; i < MAXNNEMICI; i++)
	{
		if (gameData->pids.pidNemici[i] != NOPID)
		{
			kill(gameData->pids.pidNemici[i], SIGKILL);
			waitpid(gameData->pids.pidNemici[i], NULL, 0);
			gameData->pids.pidNemici[i] = NOPID;
		}
	}

	// terminazione di tutti i proiettili amici
	for (int i = 0; i < MAXNPROIETTILI; i++)
	{
		if (gameData->pids.pidProiettili[i] != NOPID)
		{
			kill(gameData->pids.pidProiettili[i], SIGKILL);
			waitpid(gameData->pids.pidProiettili[i], NULL, 0);
			gameData->pids.pidProiettili[i] = NOPID;
		}
	}

	// terminazione di tutti i proiettili nemici
	for (int i = 0; i < MAXNPROIETTILINEMICI; i++)
	{
		if (gameData->pids.pidProiettiliNemici[i] != NOPID)
		{
			kill(gameData->pids.pidProiettiliNemici[i], SIGKILL);
			waitpid(gameData->pids.pidProiettiliNemici[i], NULL, 0);
			gameData->pids.pidProiettiliNemici[i] = NOPID;
		}
	}

	// terminazione processo tempo
	if (gameData->pids.pidTempo != NOPID)
	{
		kill(gameData->pids.pidTempo, SIGKILL);
		waitpid(gameData->pids.pidTempo, NULL, 0);
		gameData->pids.pidTempo = NOPID;
	}

	// terminazione processo musica
	spegniMusica(gameData);

	// terminazione processo suoni
	termina_soundPlayer(gameData);

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
			setScreenCell(&(schermo->screenMatrix[row][col]), RANA_OBJ, sprite->sprite[i][j], datiRana->id, sprite->color, true);
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

	if (gameData->contatori.contCoccodrilli < MAXNCOCCODRILLI)
	{

		begin_row = fromFilaToRow(fila);
		begin_col = gameData->flussi[fila - 1].direction == 1 ? FIRSTGAMECOL : INITCOCCODRILLOSXCOL;

		for (int i = begin_row; i < begin_row + COCCODRILLO_H; i++)
		{
			for (int j = begin_col; j < begin_col + COCCODRILLO_W + 1; j++)
			{
				if (gameData->schermo.screenMatrix[i][j].tipo == FIUME_OBJ)
				{
					space = true;
				}
				else{
					return false;
				}
			}
		}
	}
	return space;
}

void avviaNemici(GameData *gameData, int sec, int contatore_dispari)
{
	if ((sec + 1) % 3 == 0 && contatore_dispari == 1)
	{
		int id = id_disponibile(gameData->pids.pidNemici, MAXNNEMICI);
		if (id != NOID)
		{
			gameData->pids.pidNemici[id] = avviaNemico(gameData->pipe, id);
		}
	}
	return;
}