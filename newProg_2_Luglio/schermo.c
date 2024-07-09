#include "schermo.h"

void aggiorna(Params *thread_arg, GameData *gameData)
{
	// individuazione della possibile collisione

	Collisione collisione = detectCollisione(gameData);

	if (collisione.tipoCollisione == NO_COLLISIONE)
	{	

		if(gameData->pipeData.thread_id == gameData->allTCB->tcb_proiettili->thread_id){}
		// aggiornamento normale se no collisione
		normalUpdate(thread_arg, gameData);
	}
	else
	{
		mvprintw(13, 146, "                     ");
		mvprintw(14, 146, "                     ");
		mvprintw(15, 146, "                      ");
		mvprintw(16, 146, "                       ");
		mvprintw(17, 146, "                       ");
		mvprintw(18, 146, "                        ");

		mvprintw(13, 146, "last collisione");
		mvprintw(14, 146, "id oggetto attivo: %d", collisione.id_oggetto_attivo);
		mvprintw(15, 146, "id oggetto passivo: %d", collisione.id_oggetto_passivo);
		mvprintw(16, 146, "tipo oggetto attivo: %d", collisione.oggetto_attivo);
		mvprintw(17, 146, "tipo oggetto passivo: %d", collisione.oggetto_passivo);
		mvprintw(18, 146, "tipo collisione: %d", collisione.tipoCollisione);

		// se collisione aggiornamento particolareggiato
		handleCollisione(thread_arg, gameData, collisione);
		// beep();  OK
	}
	// qui aggiornamento del tempo(calcolo)
	aggiornaTempo(gameData);

	return;
}
//--------------------------------------------AGGIORNAMENTO OGGETTI IN MATRICE--------------------------------
/**
 * Se l'oggetto si è mosso rispetto alla lettura precedente,
 * 	aggiorna la posizione della sprite sulla matrice e salva la nuova posizione
 * @param gameData riferimento alla struttura con i dati del gioco
 * @param old_pos E' il vettore con le vecchie posizioni per quella classe di oggetti
 * @param tipoSprite il tipo della sprite dell'oggetto
 */
void aggiornaOggetto(GameData *gameData, PipeData *old_pos, TipoSprite tipoSprite)
{
	PipeData *datiNuovi = &(gameData->pipeData);			  // i dati nuovi passati in pipe
	PipeData *datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedentes

	// se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
	if (datiNuovi->x != datiVecchi->x || datiNuovi->y != datiVecchi->y)
	{

		pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), gameData);

		stampaSpriteInMatrice(datiNuovi, &(gameData->sprites[tipoSprite]), gameData);
		// ?? stampaMatrice(gameData->schermo.screenMatrix);

		aggiornaOldPos(datiVecchi, datiNuovi);
	}
}

void stampaSpriteInMatrice(PipeData *datiNuovi, Sprite *sprite, GameData *gameData)
{
	int startRow = datiNuovi->y;
	int startCol = datiNuovi->x;
	int maxRows = sprite->max_row;
	int maxCols = sprite->max_col;

	int row = 0, col = 0;

	Schermo *schermo = &(gameData->schermo);
	PipeData *pipeData = &(gameData->pipeData);

	for (int i = 0; i < maxRows; i++)
	{
		for (int j = 0; j < maxCols; j++)
		{
			row = startRow + i;
			col = startCol + j;

			schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
			schermo->screenMatrix[row][col].color = sprite->color;
			schermo->screenMatrix[row][col].is_changed = true;
			schermo->screenMatrix[row][col].id = pipeData->id;
			schermo->screenMatrix[row][col].tipo = sprite->tipo; // nuova
		}
	}
	return;
}

void pulisciSpriteInMatrice(PipeData *datiVecchi, Sprite *sprite, GameData *gameData)
{
	int row = datiVecchi->y;
	int col = datiVecchi->x;
	int maxRows = sprite->max_row; // 2
	int maxCols = sprite->max_col; // 3

	Schermo *schermo = &(gameData->schermo);

	if (row != -1)
	{
		for (int i = row; i < row + maxRows; i++)
		{
			for (int j = col; j < col + maxCols; j++)
			{
				// controllo che j non sfori
				if (j <= LASTGAMECOL && j >= FIRSTGAMECOL)
				{
					schermo->screenMatrix[i][j].ch = schermo->staticScreenMatrix[i][j].ch;
					schermo->screenMatrix[i][j].color = schermo->staticScreenMatrix[i][j].color;
					schermo->screenMatrix[i][j].is_changed = true;
					schermo->screenMatrix[i][j].tipo = schermo->staticScreenMatrix[i][j].tipo;
					schermo->screenMatrix[i][j].id = schermo->staticScreenMatrix[i][j].id;
				}

			}
		}
	}
}

//--------------------------------------------Stampa Puntuale----------------------------------------------------------------------
void stampaMatrice(ScreenCell (*screenMatrix)[WIDTH])
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE (è stato modificato)
			if (screenMatrix[i][j].is_changed)
			{
				attron(COLOR_PAIR(screenMatrix[i][j].color));
				mvaddch(i, j, screenMatrix[i][j].ch);
				attroff(COLOR_PAIR(screenMatrix[i][j].color));
				screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
				fflush(stdout);
				fflush(stdin);
			}
		}
	}
	return;
}

void stampaRefreshMatrice(ScreenCell (*screenMatrix)[WIDTH])
{
	// clear();
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE
			attron(COLOR_PAIR(screenMatrix[i][j].color));
			mvaddch(i, j, screenMatrix[i][j].ch);
			attroff(COLOR_PAIR(screenMatrix[i][j].color));
			screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
			fflush(stdout);
			fflush(stdin);
		}
	}
	refresh();
	return;
}

// cancella la sprite dell'oggetto dalle matrici e lo 'disattiva' (type = ' ')
void cancellaOggetto(GameData *gameData, PipeData *old_pos, TipoSprite tipoSprite)
{
	int id = gameData->pipeData.id;
	PipeData *datiVecchi = &(old_pos[id]); // dati al passo precedente

	if (id >= 0) // se l'id è un indice di array valido
	{
		pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), gameData);
		datiVecchi->x = 0;
		datiVecchi->y = 0;
		datiVecchi->type = ' ';
		// datiVecchi->id = id;
		datiVecchi->id = 0;
		datiVecchi->thread_id = 0;
	}
	return;
}

void stampaTanaChiusa(Tana tana, GameData *gameData)
{
	int y = tana.info.y;
	int x = tana.info.x;
	// sprite da disegnare

	int startRow = y;
	int startCol = x;
	int maxRows = gameData->sprites[8].max_row;
	int maxCols = gameData->sprites[8].max_col;
	int row = 0, col = 0;

	for (int i = 0; i < maxRows; i++)
	{
		for (int j = 0; j < maxCols; j++)
		{
			row = startRow + i;
			col = startCol + j;

			gameData->schermo.screenMatrix[row][col].tipo = TANA_CLOSE_OBJ;
			gameData->schermo.screenMatrix[row][col].ch = gameData->sprites[8].sprite[i][j];
			gameData->schermo.screenMatrix[row][col].color = gameData->sprites[8].color;
			gameData->schermo.screenMatrix[row][col].is_changed = true;
			gameData->schermo.screenMatrix[row][col].id = tana.info.id;

			gameData->schermo.staticScreenMatrix[row][col].tipo = TANA_CLOSE_OBJ;
			gameData->schermo.staticScreenMatrix[row][col].ch = gameData->sprites[8].sprite[i][j];
			gameData->schermo.staticScreenMatrix[row][col].color = gameData->sprites[8].color;

			gameData->schermo.staticScreenMatrix[row][col].id = tana.info.id;
		}
	}
	gameData->gameInfo.manche++;
}

void aggiornaTempo(GameData *gameData)
{
	gameData->gameInfo.tempo.current = time(NULL);
	gameData->gameInfo.tempo.secondi = difftime(gameData->gameInfo.tempo.current, gameData->gameInfo.tempo.start);
	return;
}

void normalUpdate(Params *thread_arg, GameData *gameData)
{
	//AllTCB *array_tcb = thread_arg->gameData->allTCB; // recupera il vettore di tcb
	//ThreadControlBlock *proiettili_tcb = array_tcb->tcb_proiettili;
	//ThreadControlBlock *nemici_tcb = array_tcb->tcb_piante;
	sem_t *semaforoTCB = &(thread_arg->semafori->tcb_mutex);

	switch (gameData->pipeData.type)
	{
	case 'X': // movimento rana
	{

		// controlli su spostamento rana
		int newPosAbsRanaX = gameData->pipeData.x + gameData->ranaAbsPos.x;
		int newPosAbsRanaY = gameData->pipeData.y + gameData->ranaAbsPos.y;
		// se lo spostamento è lecito
		if (isFrogMoveLecit(newPosAbsRanaX, newPosAbsRanaY))
		{
			gameData->pipeData.x = newPosAbsRanaX;
			gameData->pipeData.y = newPosAbsRanaY;
			// normale aggiornamento
			aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
			gameData->ranaAbsPos.x = gameData->pipeData.x;
			gameData->ranaAbsPos.y = gameData->pipeData.y;
		}
		break;
	}
	case 'S': // sparo proiettile da rana
	{

		// proiettile sparato da utente avvia il proiettile con posizione iniziale della rana (o dell oggetto che ha sparato)
		if (gameData->contatori.contProiettili < MAXNPROIETTILI) // se si hanno ancora munizioni
		{
			// incremento contatore e faccio partire il thread proiettile, salvo il tid del thread
			int id = id_disponibile(gameData->pids.pidProiettili, MAXNPROIETTILI);
			if (id != -1 && (id >= 0 && id < MAXNPROIETTILI))
			{

				pthread_t newThread = avviaProiettileThread(thread_arg, id);
				gameData->pids.pidProiettili[id] = newThread;
				// gameData->contatori.contProiettili++;
				gameData->contatori.contProiettili = (gameData->contatori.contProiettili + 1) % (MAXNPROIETTILI + 1);

				// NOTA: Il proiettilie aggiorna il suo TCB alla creazione
			}
		}
		break;
	}
	case 'P': // Proiettile Rana
	{
		pthread_t tid_proiettile = leggiTid( &(gameData->allTCB->tcb_proiettili[gameData->pipeData.id]), semaforoTCB);
		if(tid_proiettile == 0){break;}
		
		
		// nuove coordinate proiettile se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
		// se il proiettile è gia terminato..


		if (isThreadTerminated(&gameData->allTCB->tcb_proiettili[gameData->pipeData.id], semaforoTCB))
		{
			cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
			// beep();
			int err = joinThreadMorto(&(gameData->allTCB->tcb_proiettili[gameData->pipeData.id]), semaforoTCB);
			if (err)
			{
				// beep();
				break;
			}

			gameData->pids.pidProiettili[gameData->pipeData.id] = 0;
			gameData->contatori.contProiettili = (gameData->contatori.contProiettili - 1) % MAXNPROIETTILI;
		}
		else
		{
			if (gameData->pipeData.y <= FULLTANAROWEND) // è usicto dall'area di gioco
			{

				PipeData proiettile = (PipeData)(gameData->pipeData);
				// ThreadControlBlock* ptrTCB = cercaThreadByTid(gameData->allTCB->tcb_proiettili, proiettile.thread_id, &thread_arg->semafori->tcb_mutex,MAXNPROIETTILI);
				ThreadControlBlock *ptrTCB = &(gameData->allTCB->tcb_proiettili[proiettile.id]);

				if (!isThreadValid(ptrTCB, &thread_arg->semafori->tcb_mutex))
				{
					perror("TCB MISSING");
					break;
				}
				else
				{
					// perror("TCB OK");
				}
				if (isThreadTarget(ptrTCB, &thread_arg->semafori->tcb_mutex))
				{ // se il proiettile è già "target", esci.
					// perror("Proiettile già impostato!");
					break;
				}

				int err;
				err = impostaThreadTarget(ptrTCB, &thread_arg->semafori->tcb_mutex);

				if (err)
				{
					perror("Errore Designa Proiettile Target");
					if (err == -1)
						perror("Assegnamento Target Fallito");
					if (err == -2)
						perror("TCB Target NULLO");
				}

				gameData->gameInfo.punteggio++;
				gameData->gameInfo.punteggioIsChanged = true;
				// beep();
			}
			else // il proiettile è in campo
			{
				aggiornaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
			}
		}

		break;
	}
	case 'n': // nemico-pianta
	{

		if (isThreadTerminated(&gameData->allTCB->tcb_piante[gameData->pipeData.id], semaforoTCB)) // ultima scrittura di Nemico
		{
			cancellaOggetto(gameData, gameData->oldPos.nemici, S_PIANTA);
			joinThreadMorto(&(gameData->allTCB->tcb_piante[gameData->pipeData.id]), semaforoTCB);
			gameData->pids.pidNemici[gameData->pipeData.id] = 0;
			gameData->contatori.contNemici = (gameData->contatori.contNemici - 1) % MAXNNEMICI;
		}
		else
		{
			aggiornaOggetto(gameData, gameData->oldPos.nemici, S_PIANTA);
		}
		break;
	}
	case 's': // sparo del nemico-pianta
	{

		if (gameData->contatori.contProiettiliN < MAXNPROIETTILINEMICI)
		{
			int idNemico = gameData->pipeData.id;
			pthread_t newProiettileN = avviaProiettileNemicoThread(thread_arg, idNemico);
			gameData->pids.pidProiettiliNemici[idNemico] = newProiettileN;
			// gameData->contatori.contProiettiliN++;
			gameData->contatori.contProiettiliN = (gameData->contatori.contProiettiliN + 1) % (MAXNPROIETTILINEMICI + 1);
		}
		// beep();
		break;
	}
	case 'p': // Proiettile Nemico
	{	
		pthread_t tid_proiettileN = leggiTid( &(gameData->allTCB->tcb_proiettili_nemici[gameData->pipeData.id]), semaforoTCB);
		if(tid_proiettileN == 0){break;}	// se questo thread è gia terminato ed è stato resettto il tcb, esci.
		

		if (isThreadTerminated(&gameData->allTCB->tcb_proiettili_nemici[gameData->pipeData.id], semaforoTCB)) // hai letto proiettileN terminato
		{
			cancellaOggetto(gameData, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO); // cancella sprite

			int err = joinThreadMorto(&(gameData->allTCB->tcb_proiettili_nemici[gameData->pipeData.id]), semaforoTCB); // chiudi thread
			if (err)
			{
				beep();
				break;
			}

			gameData->pids.pidProiettiliNemici[gameData->pipeData.id] = 0;											// resetta threadID
			gameData->contatori.contProiettiliN = (gameData->contatori.contProiettiliN - 1) % MAXNPROIETTILINEMICI; // aggiorna contatore
		}
		else
		{
			PipeData proiettileN = gameData->pipeData; // oggetto appena letto da buffer

			if (gameData->pipeData.y >= MARCIAPIEDEROWEND) // se il proiettile è ancora in campo
			{
				// se y >= di MARCIAPIEDEROWEND
				/* terminare e cancellare il proiettile */

				ThreadControlBlock *tcbProiettileN = &(gameData->allTCB->tcb_proiettili_nemici[proiettileN.id]);

				if (!isThreadValid(tcbProiettileN, semaforoTCB))
				{
					perror("TCB proiettileNem  Invalido");
					//_exit(4);
				}
				if (isThreadTarget(tcbProiettileN, semaforoTCB))
				{
					break;
				} // il thread è già target

				int err = impostaThreadTarget(tcbProiettileN, semaforoTCB);
				if (err)
				{
					perror("Errore Designa ProiettileNem Target");
					if (err == -1)
						perror("Assegnamento Target Fallito");
					if (err == -2)
						perror("TCB PN_Target NULLO");
				}
				// cancellaOggetto(gameData, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);

				// gameData->pids.pidProiettiliNemici[gameData->pipeData.id] = 0;
				// gameData->contatori.contProiettiliN--;
				// beep();
			}
			else
			{
				aggiornaOggetto(gameData, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
			}
		}

		break;
	}
	case 'C': // Coccodrillo 
	{
		// controllo se il coccodrillo sfora come faccio a capire verso che direzione va?
		if (gameData->pipeData.x > LASTGAMECOL)
		{
			ThreadControlBlock *coccodrillo = &(gameData->allTCB->tcb_coccodrilli[gameData->pipeData.id]);
			impostaThreadTarget( coccodrillo, semaforoTCB);
			//uccidiCoccodrillo(gameData->pids.pidCoccodrilli, gameData->pipeData.id);
			cancellaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C);
			// todo reset del controlloCoccodrilli, da finire
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi = 0;
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi_in_immersione = 0;
			gameData->contatori.contCoccodrilli--;
		}
		else
		{
			handleCoccodrilloMovement(gameData);
		}
		break;
	}
	case 'c':
	{

		// controllo che il coccodrillo non abbia sforato a sinistra
		if (gameData->pipeData.x < FIRSTGAMECOL - 9)
		{
			ThreadControlBlock *coccodrillo = &(gameData->allTCB->tcb_coccodrilli[gameData->pipeData.id]);
			impostaThreadTarget( coccodrillo, semaforoTCB);

			//uccidiCoccodrillo(gameData->pids.pidCoccodrilli, gameData->pipeData.id);
			cancellaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C);
			// todo reset del controlloCoccodrilli, da finire
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi = 0;
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi_in_immersione = 0;
			gameData->contatori.contCoccodrilli--;
		}
		else
		{
			handleCoccodrilloMovement(gameData);
		}
		break;
	}
	case 'T':
	{
		// arrivato tempo di gioco
		gameData->gameInfo.tempo.milliseconds = gameData->pipeData.x;
	}
	case 'Y':
		// uccidiProiettileThread(thread_arg, gameData,gameData->pipeData.thread_id);
		break;
	default:
		break;
	} // end switch
	return;
}

/** Stampa del Nemico_Pianta (da PROCESSI)
 * 
 */
void printPianta(GameData *gameData, PipeData *old_pos, TipoSprite tipoSprite)
{
	PipeData *datiNuovi = &(gameData->pipeData);			  // i dati nuovi passati in pipe
	PipeData *datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedentes

	// se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
	if (datiNuovi->x != datiVecchi->x || datiNuovi->y != datiVecchi->y)
	{
		stampaSpriteInMatrice(datiNuovi, &(gameData->sprites[tipoSprite]), gameData);
		stampaMatrice(gameData->schermo.screenMatrix);
		aggiornaOldPos(datiVecchi, datiNuovi);
	}
}

/**	Gestore movimento coccodrilli
 * 
 */
void handleCoccodrilloMovement(GameData *gameData)
{
	CocodrileControl *controlloCoccodrillo = &(gameData->controlloCoccodrilli[gameData->pipeData.id]);
	// capire in che fase si trova il coccodrillo
	if (controlloCoccodrillo->is_fase_immersione)
	{
		// questione fase immersione
		// 3 casi:
		// 1. il coccodrillo si sta immergendo
		// 2. il coccodrillo è interamente sotto l'acqua
		// 3. il coccodrillo sta riemergendo
		if (controlloCoccodrillo->is_going_deep)
		{
			if (controlloCoccodrillo->direction == 1)
			{
				// il coccodrillo va verso destra
				// cancellare lo posso lasciare come prima
				// invece disegnare lo devo modificare
				aggiornaOggettoCoccodrillo(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C, controlloCoccodrillo);
			}
			else
			{
				// il coccodrillo va verso sinistra
				aggiornaOggettoCoccodrillo(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C, controlloCoccodrillo);
			}
			// incremento offset per disegno
			controlloCoccodrillo->offset_deep++;
			// controllo su offset per disegno
			if (controlloCoccodrillo->offset_deep > COCCODRILLO_W)
			{
				controlloCoccodrillo->is_going_deep = false;
				controlloCoccodrillo->is_deep = true;
			}
		}
		else
		{

			if (controlloCoccodrillo->is_going_up)
			{
				if (controlloCoccodrillo->direction != 1)
				{
					// il coccodrillo va verso destra
					// cancellare lo posso lasciare come prima
					// invece disegnare lo devo modificare
					aggiornaOggettoCoccodrillo(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C, controlloCoccodrillo);
				}
				else
				{
					// il coccodrillo va verso sinistra
					aggiornaOggettoCoccodrillo(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C, controlloCoccodrillo);
				}
				// sta riemergendo
				controlloCoccodrillo->offset_deep--;
				if (controlloCoccodrillo->offset_deep <= 0)
				{
					controlloCoccodrillo->is_going_up = false;
					controlloCoccodrillo->is_fase_immersione = false;
					controlloCoccodrillo->offset_deep = 0;
					controlloCoccodrillo->passi_deep = 0;
					controlloCoccodrillo->passi_in_immersione = 0;
					controlloCoccodrillo->passi_in_pre_immersione = 0;
				}
			}
			else
			{
				// è completamente sotto acqua non lo devo neanche disegnare
				controlloCoccodrillo->passi_deep++;
				if (controlloCoccodrillo->passi_deep >= 8)
				{
					controlloCoccodrillo->is_deep = false;
					controlloCoccodrillo->is_going_up = true;
					controlloCoccodrillo->passi_deep = 0;
					controlloCoccodrillo->offset_deep -= 2;
				}
			}
		}
		gameData->controlloCoccodrilli[gameData->pipeData.id].passi++;
	}
	else
	{
		if (controlloCoccodrillo->is_fase_pre_immersione)
		{
			// questione lampeggio e passaggio a fase immersione
			// come posso fare test?
			// dopo fase pre immersione lo faccio andare di nuovo in fase normale
			if (controlloCoccodrillo->passi % 2 == 0)
			{
				// lo faccio lampeggiare
				controlloCoccodrillo->lampeggia = true;
			}
			else
			{
				controlloCoccodrillo->lampeggia = false;
			}

			if (controlloCoccodrillo->lampeggia)
			{
				if (gameData->pipeData.type == 'C')
				{
					aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_L);
				}
				if (gameData->pipeData.type == 'c')
				{
					aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_L);
				}
			}
			else
			{

				if (gameData->pipeData.type == 'C')
				{
					aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C);
				}
				if (gameData->pipeData.type == 'c')
				{
					aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C);
				}
			}
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi++;
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi_in_pre_immersione++;
			if (gameData->controlloCoccodrilli[gameData->pipeData.id].passi_in_pre_immersione >= 5)
			{
				controlloCoccodrillo->is_fase_pre_immersione = false;
				controlloCoccodrillo->is_fase_immersione = true;
				controlloCoccodrillo->is_going_deep = true;
				controlloCoccodrillo->passi_in_pre_immersione = 0;
			}
		}
		else
		{
			// aggiornamento normale
			// switch su tipo del coccodrillo in base alla direzione
			if (gameData->pipeData.type == 'C')
			{
				if (gameData->controlloCoccodrilli[gameData->pipeData.id].is_buono)
				{
					aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX);
					if(gameData->controlloCoccodrilli[gameData->pipeData.id].rana_on){
						// ci stampo sopra la rana
						stampaSpriteInMatrice(&(gameData->oldPos.coccodrilli[gameData->pipeData.id]), &(gameData->sprites[S_RANA]),gameData);
						
					}
				}
				else
				{
					aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C);
				}
			}

			if (gameData->pipeData.type == 'c')
			{
				if (gameData->controlloCoccodrilli[gameData->pipeData.id].is_buono)
				{
					aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX);
					if(gameData->controlloCoccodrilli[gameData->pipeData.id].rana_on){
						// ci stampo sopra la rana
						stampaSpriteInMatrice(&(gameData->oldPos.coccodrilli[gameData->pipeData.id]), &(gameData->sprites[S_RANA]),gameData);
						
					}
				}
				else
				{
					aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C);
		
				}
			}
			if (!(controlloCoccodrillo->is_buono))
			{
				if ((controlloCoccodrillo->passi + 1) % 30 == 0)
				{
					controlloCoccodrillo->is_fase_pre_immersione = true;
				}
			}

			gameData->controlloCoccodrilli[gameData->pipeData.id].passi++;
		}
	}

	return;
}// end handleCoccodrilloMovement


void aggiornaOggettoCoccodrillo(GameData *gameData, PipeData *old_pos, TipoSprite tipoSprite, CocodrileControl *controlloCoccodrillo)
{
	PipeData *datiNuovi = &(gameData->pipeData);			  // i dati nuovi passati in pipe
	PipeData *datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedentes

	// se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
	if (datiNuovi->x != datiVecchi->x || datiNuovi->y != datiVecchi->y)
	{

		pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), gameData);
		stampaMatrice(gameData->schermo.screenMatrix);
		stampaCoccodrilloInMatrice(datiNuovi, &(gameData->sprites[tipoSprite]), gameData, controlloCoccodrillo);
		stampaMatrice(gameData->schermo.screenMatrix);
		aggiornaOldPos(datiVecchi, datiNuovi);
	}
}


void stampaCoccodrilloInMatrice(PipeData *datiNuovi, Sprite *sprite, GameData *gameData, CocodrileControl *controlloCoccodrillo)
{
	int startRow = datiNuovi->y;
	int startCol = datiNuovi->x;
	int maxRows = sprite->max_row;
	int maxCols = sprite->max_col;

	int direzione = controlloCoccodrillo->direction;
	int offset = controlloCoccodrillo->offset_deep;

	int row = 0, col = 0;

	Schermo *schermo = &(gameData->schermo);
	PipeData *pipeData = &(gameData->pipeData);

	for (int i = 0; i < maxRows; i++)
	{
		if (controlloCoccodrillo->is_going_deep)
		{
			if (direzione == 1)
			{
				// coccodrillo verso destra
				for (int j = 0; j < maxCols - offset; j++)
				{
					row = startRow + i;
					col = startCol + j;
					// controllo che col non sfori
					if (col <= LASTGAMECOL && col >= FIRSTGAMECOL)
					{
						schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
						schermo->screenMatrix[row][col].color = sprite->color;
						schermo->screenMatrix[row][col].is_changed = true;
						schermo->screenMatrix[row][col].id = pipeData->id;
						schermo->screenMatrix[row][col].tipo = sprite->tipo;
					}
				}
			}
			else
			{
				// coccodrillo verso sinistra
				for (int j = offset; j < maxCols; j++)
				{
					row = startRow + i;
					col = startCol + j;
					// controllo che col non sfori
					if (col <= LASTGAMECOL && col >= FIRSTGAMECOL)
					{
						schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
						schermo->screenMatrix[row][col].color = sprite->color;
						schermo->screenMatrix[row][col].is_changed = true;
						schermo->screenMatrix[row][col].id = pipeData->id;
						schermo->screenMatrix[row][col].tipo = sprite->tipo;
					}
				}
			}
		}
		else
		{
			// sto riemergendo
			int quanto_disegno = COCCODRILLO_W - offset;
			if (direzione == 1)
			{
				// coccodrillo verso destra in riemersione
				// coccodrillo verso sinistra
				for (int j = maxCols - quanto_disegno; j < maxCols; j++)
				{
					row = startRow + i;
					col = startCol + j;
					// controllo che col non sfori
					if (col <= LASTGAMECOL && col >= FIRSTGAMECOL)
					{
						schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
						schermo->screenMatrix[row][col].color = sprite->color;
						schermo->screenMatrix[row][col].is_changed = true;
						schermo->screenMatrix[row][col].id = pipeData->id;
						schermo->screenMatrix[row][col].tipo = sprite->tipo;
					}
				}
			}
			else
			{
				// coccodrillo verso sinistra in riemersione
				for (int j = 0; j < quanto_disegno; j++)
				{
					row = startRow + i;
					col = startCol + j;
					// controllo che col non sfori
					if (col <= LASTGAMECOL && col >= FIRSTGAMECOL)
					{
						schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
						schermo->screenMatrix[row][col].color = sprite->color;
						schermo->screenMatrix[row][col].is_changed = true;
						schermo->screenMatrix[row][col].id = pipeData->id;
						schermo->screenMatrix[row][col].tipo = sprite->tipo;
					}
				}
			}
		}
	}
	return;
}

/**	Aggiorna in matrice un oggetto specifico passato come new_pos
 * @param gameData
 * @param new_pos l'oggetto da aggiornare sulla matrice e in old_pos
 * @param old_pos vettore con le vecchie posizioni dell'oggetto
 * @param tipoSprite sprite dell'oggetto, da stampare in matrice
 */
void aggiornaOggettoNew(GameData *gameData,PipeData new_pos, PipeData *old_pos, TipoSprite tipoSprite)
{
	PipeData *datiNuovi = &(new_pos);			  // i dati nuovi passati in pipe
	PipeData *datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedentes

	// se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
	if (datiNuovi->x != datiVecchi->x || datiNuovi->y != datiVecchi->y)
	{

		pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), gameData);
		stampaMatrice(gameData->schermo.screenMatrix);
		stampaSpriteInMatrice(datiNuovi, &(gameData->sprites[tipoSprite]), gameData);
		stampaMatrice(gameData->schermo.screenMatrix);
		aggiornaOldPos(datiVecchi, datiNuovi);
	}
}

