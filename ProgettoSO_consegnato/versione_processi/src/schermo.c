#include "../hdr/schermo.h"

void aggiorna(GameData *gameData)
{
	Collisione collisione = detectCollisione(gameData); // individuazione della possibile collisione

	if (collisione.tipoCollisione == NO_COLLISIONE)
	{
		normalUpdate(gameData); // aggiornamento normale se no collisione
	}
	else
	{
		handleCollisione(gameData, collisione);				  // se collisione aggiornamento particolareggiato
	}
	return;
}
//--------------------------------------------AGGIORNAMENTO OGGETTI IN MATRICE--------------------------------
void aggiornaOggetto(GameData *gameData, PipeData *old_pos, TipoSprite tipoSprite)
{
	PipeData *datiNuovi = &(gameData->pipeData);			  // i dati nuovi passati in pipe
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
			// controllo che col non sfori
			if (col <= LASTGAMECOL && col >= FIRSTGAMECOL)
			{
				setScreenCell(&(schermo->screenMatrix[row][col]), sprite->tipo, sprite->sprite[i][j], pipeData->id, sprite->color, true);
			}
		}
	}
	return;
}

void pulisciSpriteInMatrice(PipeData *datiVecchi, Sprite *sprite, GameData *gameData)
{
	// nel caso del coccodrillo avro che maxRow 2 e macCol 9
	// la row non sfora mai percio posso anche non controllarla ma la col si

	int row = datiVecchi->y;
	int col = datiVecchi->x;
	int maxRows = sprite->max_row;
	int maxCols = sprite->max_col;

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
					ScreenCell staticCell = schermo->staticScreenMatrix[i][j];
					setScreenCell(&(schermo->screenMatrix[i][j]), staticCell.tipo, staticCell.ch, staticCell.id, staticCell.color, true);
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
			}
		}
	}
	return;
}

void stampaRefreshMatrice(ScreenCell (*screenMatrix)[WIDTH])
{
	clear();
	usleep(20000);
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE
			attron(COLOR_PAIR(screenMatrix[i][j].color));
			mvaddch(i, j, screenMatrix[i][j].ch);
			attroff(COLOR_PAIR(screenMatrix[i][j].color));
			screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
		}
	}
	refresh();
	usleep(2000);
	return;
}

// cancella la sprite dell'oggetto dalle matrici e lo 'disattiva' (type = ' ')
void cancellaOggetto(GameData *gameData, PipeData *old_pos, TipoSprite tipoSprite)
{
	PipeData *datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedente
	int id = gameData->pipeData.id;
	if (id >= 0) // se l'id è un indice di array valido
	{
		pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), gameData);
		stampaMatrice(gameData->schermo.screenMatrix);
		setPipeData(datiVecchi, ' ', NOID, NOPOS, NOPOS);
	}
	return;
}

void stampaTanaChiusa(Tana tana, GameData *gameData)
{
	int y = tana.info.y;
	int x = tana.info.x;
	// sprite da disegnare
	Sprite sprite = gameData->sprites[S_TANA_CHIUSA];

	int startRow = y;
	int startCol = x;
	int maxRows = sprite.max_row;
	int maxCols = sprite.max_col;
	int row = 0, col = 0;

	for (int i = 0; i < maxRows; i++)
	{
		for (int j = 0; j < maxCols; j++)
		{
			row = startRow + i;
			col = startCol + j;
			setScreenCell(&(gameData->schermo.screenMatrix[row][col]), TANA_CLOSE_OBJ, sprite.sprite[i][j], tana.info.id, sprite.color, true);
			setScreenCell(&(gameData->schermo.staticScreenMatrix[row][col]), TANA_CLOSE_OBJ, sprite.sprite[i][j], tana.info.id, sprite.color, false);
		}
	}
	gameData->gameInfo.manche++;
}

void normalUpdate(GameData *gameData)
{
	switch (gameData->pipeData.type)
	{
	case 'X': // movimento rana
	{
		// controlli su spostamento rana
		int newPosAbsRanaX = gameData->pipeData.x + gameData->ranaAbsPos.x;
		int newPosAbsRanaY = gameData->pipeData.y + gameData->ranaAbsPos.y;
		// se lo spostamento è lecito
		if (isFrogMoveLecit(newPosAbsRanaX, newPosAbsRanaY, gameData->ranaAbsPos, gameData->pipeData))
		{
			if (gameData->ranaAbsPos.id_coccodrillo != NOID && gameData->pipeData.x != 0)
			{
				// sono su un coccodrillo e mi sto muovendo lungo la x
				// mofico solo l'offest?
				gameData->ranaAbsPos.offset_on_coccodrillo += gameData->pipeData.x;
				// forse meglio ristampare il coccodrillo con sopra la rana?
			}
			else
			{
				// normale aggiornamento
				gameData->pipeData.x = newPosAbsRanaX;
				gameData->pipeData.y = newPosAbsRanaY;

				aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
				gameData->ranaAbsPos.x = gameData->pipeData.x;
				gameData->ranaAbsPos.y = gameData->pipeData.y;
			}
		}
		break;
	}
	case 'S': // sparo proiettile da rana
	{
		// proiettile sparato da utente avvia il proiettile con posizione iniziale della rana (o dell oggetto che ha sparato)
		if (gameData->contatori.contProiettili < MAXNPROIETTILI) // se si hanno ancora munizioni
		{
			// incremento contatore e faccio partire il processo proiettile, salvo il pid del processo
			int id = id_disponibile(gameData->pids.pidProiettili, MAXNPROIETTILI);
			if (id != NOID)
			{
				gameData->pids.pidProiettili[id] = avviaProiettile(gameData->pipe, &(gameData->ranaAbsPos), id);
				gameData->contatori.contProiettili++;
				gameData->oldPos.proiettili[id].id = id;
			}
		}
		break;
	}
	case 's': // sparo proiettile da nemico pianta
	{
		// sparo proiettile da pianta
		if (gameData->contatori.contProiettiliN < MAXNPROIETTILINEMICI) // se si hanno ancora munizioni
		{
			// incremento contatore e faccio partire il processo proiettile, salvo il pid del processo
			int id = id_disponibile(gameData->pids.pidProiettiliNemici, MAXNPROIETTILINEMICI);
			if (id != NOID)
			{
				gameData->pids.pidProiettiliNemici[id] = avviaProiettileNemico(gameData->pipe, &(gameData->pipeData), id);
				gameData->contatori.contProiettiliN++;
				gameData->oldPos.proiettiliNemici[id].id = id;
			}
		}

		break;
	}
	case 'P': // movimento proiettile amico
	{
		// nuove coordinate proiettile se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
		if (gameData->pipeData.y < FULLTANAROWEND)
		{
			uccidiProiettile(gameData->pids.pidProiettili, gameData->pipeData.id); // uccide il processo proiettile
			// ucciso processo proiettile e impostato a zero il pid in array pid proiettili
			cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
			gameData->contatori.contProiettili--;
		}
		else
		{
			aggiornaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
		}
		break;
	}
	case 'p': // movimento proiettile nemico
	{
		if (gameData->pipeData.y + 1 == LASTGAMEROW)
		{
			uccidiProiettileNemico(gameData->pids.pidProiettiliNemici, gameData->pipeData.id);
			cancellaOggetto(gameData, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
			gameData->contatori.contProiettiliN--;
		}
		else
		{

			aggiornaOggetto(gameData, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
		}
		break;
	}
	case 'n': // spawn nemico
	{
		// prima print del nemico
		printPianta(gameData, gameData->oldPos.nemici, S_PIANTA);
		gameData->contatori.contNemici++;
		gameData->oldPos.nemici[gameData->pipeData.id].id = gameData->pipeData.id;
		break;
	}
	case 'C': // movimento coccodrillo verso destra
	{
		// controllo se il coccodrillo sfora come faccio a capire verso che direzione va?
		if (gameData->pipeData.x > LASTGAMECOL)
		{
			uccidiCoccodrillo(gameData->pids.pidCoccodrilli, gameData->pipeData.id);
			cancellaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C);
			// todo reset del controlloCoccodrilli, da finire
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi = 0;
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi_in_immersione = 0;
			gameData->contatori.contCoccodrilli--;
			int fila = gameData->controlloCoccodrilli[gameData->pipeData.id].fila;
			gameData->flussi[fila - 1].n_coccodrilli_attivi--;
		}
		else
		{
			handleCoccodrilloMovement(gameData);
		}
		break;
	}
	case 'c': // movimento coccodrillo verso sinistra
	{
		// controllo che il coccodrillo non abbia sforato a sinistra
		if (gameData->pipeData.x < FIRSTGAMECOL - 9)
		{
			uccidiCoccodrillo(gameData->pids.pidCoccodrilli, gameData->pipeData.id);
			cancellaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C);
			// todo reset del controlloCoccodrilli, da finire
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi = 0;
			gameData->controlloCoccodrilli[gameData->pipeData.id].passi_in_immersione = 0;
			gameData->contatori.contCoccodrilli--;
			int fila = gameData->controlloCoccodrilli[gameData->pipeData.id].fila;
			gameData->flussi[fila - 1].n_coccodrilli_attivi--;
		}
		else
		{
			handleCoccodrilloMovement(gameData);
		}
		break;
	}
	case 'T': // tempo di gioco
	{
		// update tempo di gioco
		gameData->gameInfo.secondi_di_gioco = gameData->pipeData.x;
		gameData->gameInfo.tempoIsChanged = true;
		break;
	}
	default:
	{
		break;
	}
	}
	return;
}

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
		gameData->oldPos.nemici[gameData->pipeData.id].id = gameData->pipeData.id;
	}
}

void handleCoccodrilloMovement(GameData *gameData)
{
	CocodrileControl *controlloCoccodrillo = &(gameData->controlloCoccodrilli[gameData->pipeData.id]);

	int spriteDaDisegnare = determinaSpriteCoccodrillo(controlloCoccodrillo);

	if (controlloCoccodrillo->is_fase_immersione)
	{
		if (controlloCoccodrillo->is_going_deep)
		{
			if (controlloCoccodrillo->direction == 1)
			{
				// il coccodrillo va verso destra
				aggiornaOggettoCoccodrillo(gameData, gameData->oldPos.coccodrilli, spriteDaDisegnare, controlloCoccodrillo);

				if (gameData->ranaAbsPos.id_coccodrillo == gameData->pipeData.id)
				{
					// se sommando i due ottengo 7 devo uccidere la rana
					if (gameData->ranaAbsPos.offset_on_coccodrillo + gameData->controlloCoccodrilli[gameData->ranaAbsPos.id_coccodrillo].offset_deep >= 7)
					{
						// imposto rana is dead a true
						gameData->gameInfo.ranaIsDead = true;
					}
					else
					{
						stampaRanaOnCoccodrillo(gameData);
					}
				}
			}
			else
			{
				// il coccodrillo va verso sinistra
				aggiornaOggettoCoccodrillo(gameData, gameData->oldPos.coccodrilli, spriteDaDisegnare, controlloCoccodrillo);
				if (gameData->ranaAbsPos.id_coccodrillo == gameData->pipeData.id)
				{
					if (gameData->controlloCoccodrilli[gameData->pipeData.id].offset_deep - gameData->ranaAbsPos.offset_on_coccodrillo >= 1)
					{
						// imposto rana is dead a true
						gameData->gameInfo.ranaIsDead = true;
					}
					else
					{
						stampaRanaOnCoccodrillo(gameData);
					}
				}
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
				aggiornaOggettoCoccodrillo(gameData, gameData->oldPos.coccodrilli, spriteDaDisegnare, controlloCoccodrillo);
				if (gameData->ranaAbsPos.id_coccodrillo == gameData->pipeData.id)
				{
					stampaRanaOnCoccodrillo(gameData);
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
			// lampeggio e passaggio a fase immersione
			controlloCoccodrillo->lampeggia = (controlloCoccodrillo->passi % 2 == 0);
			spriteDaDisegnare = determinaSpriteCoccodrillo(controlloCoccodrillo);
			aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, spriteDaDisegnare);
			if (gameData->ranaAbsPos.id_coccodrillo == gameData->pipeData.id)
			{
				stampaRanaOnCoccodrillo(gameData);
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
			aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, spriteDaDisegnare);
			if (gameData->ranaAbsPos.id_coccodrillo == gameData->pipeData.id)
			{
				stampaRanaOnCoccodrillo(gameData);
			}

			if (!(controlloCoccodrillo->is_buono))
			{
				if (generaRandom_r(0,30,&(gameData->randomSeed)) == 1 && controlloCoccodrillo->passi>10 && controlloCoccodrillo->passi<80)
				{
					controlloCoccodrillo->is_fase_pre_immersione = true;
				}
			}

			gameData->controlloCoccodrilli[gameData->pipeData.id].passi++;
		}
	}

	return;
}

int determinaSpriteCoccodrillo(CocodrileControl *controlloCoccodrillo)
{
	if (controlloCoccodrillo->is_fase_immersione)
	{
		if (controlloCoccodrillo->direction == 1)
		{
			return S_COCCODRILLO_DX_C;
		}
		else
		{
			return S_COCCODRILLO_SX_C;
		}
	}
	else if (controlloCoccodrillo->is_fase_pre_immersione)
	{
		if (controlloCoccodrillo->direction == 1)
		{
			return (controlloCoccodrillo->lampeggia) ? S_COCCODRILLO_DX_L : S_COCCODRILLO_DX_C;
		}
		else
		{
			return (controlloCoccodrillo->lampeggia) ? S_COCCODRILLO_SX_L : S_COCCODRILLO_SX_C;
		}
	}
	else
	{
		if (controlloCoccodrillo->direction == 1)
		{
			return (controlloCoccodrillo->is_buono) ? S_COCCODRILLO_DX : S_COCCODRILLO_DX_C;
		}
		else
		{
			return (controlloCoccodrillo->is_buono) ? S_COCCODRILLO_SX : S_COCCODRILLO_SX_C;
		}
	}
}

void stampaRanaOnCoccodrillo(GameData *gameData)
{
	// ci stampo sopra la rana
	PipeData rana;
	rana.x = gameData->oldPos.coccodrilli[gameData->pipeData.id].x + gameData->ranaAbsPos.offset_on_coccodrillo;
	rana.y = gameData->oldPos.coccodrilli[gameData->pipeData.id].y;
	rana.id = 0;
	rana.type = 'X';
	// aggiorno rana abs pos
	gameData->ranaAbsPos.x = rana.x;
	gameData->ranaAbsPos.y = rana.y;
	stampaSpriteInMatrice(&(rana), &(gameData->sprites[S_RANA]), gameData);

	// se la rana esce fuori dallo schermo allora muore
	if (gameData->ranaAbsPos.x >= LASTGAMECOL - 1 || gameData->ranaAbsPos.x < FIRSTGAMECOL)
	{
		// imposto rana is dead a true
		gameData->gameInfo.ranaIsDead = true;
	}
	return;
}

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
						setScreenCell(&(schermo->screenMatrix[row][col]), sprite->tipo, sprite->sprite[i][j], pipeData->id, sprite->color, true);
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
						setScreenCell(&(schermo->screenMatrix[row][col]), sprite->tipo, sprite->sprite[i][j], pipeData->id, sprite->color, true);
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
						setScreenCell(&(schermo->screenMatrix[row][col]), sprite->tipo, sprite->sprite[i][j], pipeData->id, sprite->color, true);
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
						setScreenCell(&(schermo->screenMatrix[row][col]), sprite->tipo, sprite->sprite[i][j], pipeData->id, sprite->color, true);
					}
				}
			}
		}
	}
	return;
}

/** Cancella dalla matrice uno specifico oggetto passato al parametro nemico

@param gameData riferimento a tutti i dati di gioco
@param oggetto è il riferimento al nemico da cancellare dalla matrice
@param old_pos vettore delle posizioni di tutti gli oggetti
@param tiposprite il tipo della sprite
*/
void cancellaOggettoDaMatrice(GameData *gameData, PipeData oggetto, PipeData *old_pos, TipoSprite tipoSprite)
{
	int id = oggetto.id;

	PipeData *datiVecchi = &(old_pos[id]); // dati al passo precedente

	if (id >= 0) // se l'id è un indice di array valido
	{
		pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), gameData);
		stampaMatrice(gameData->schermo.screenMatrix);
		setPipeData(datiVecchi, ' ', NOID, NOPOS, NOPOS);
	}
	return;
}

void aggiornaOggettoNew(GameData *gameData, PipeData new_pos, PipeData *old_pos, TipoSprite tipoSprite)
{
	PipeData *datiNuovi = &(new_pos);						  // i dati nuovi passati in pipe
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