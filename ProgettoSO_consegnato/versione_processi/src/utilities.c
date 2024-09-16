#include "../hdr/utilities.h"

void inizializzaNcurses()
{
	srand(time(NULL));
	initscr();			   // Inizializza ncurses
	curs_set(FALSE);	   // Nasconde il cursore
	nodelay(stdscr, TRUE); // Abilita l'input non bloccante
	keypad(stdscr, TRUE);  // Abilita il keypad mode
	inizializzaColorazione();
	return;
}

void inizializzaColorazione()
{
	start_color();
	init_pair(RANA_COL, COLOR_GREEN, COLOR_BLACK);			   // colore rana
	init_pair(SFONDO_COL, COLOR_WHITE, COLOR_BLACK);		   // colore sfondo
	init_pair(FIUME_COL, COLOR_WHITE, COLOR_BLUE);			   // colore fiume
	init_pair(TANE_COL, COLOR_WHITE, COLOR_YELLOW);			   // colore tane
	init_pair(ARGINE_COL, COLOR_BLACK, COLOR_GREEN);		   // colore argine
	init_pair(MARCIAPIEDE_COL, COLOR_BLACK, COLOR_GRAY);	   // colore marciapiede
	init_pair(MENUBOX_COL, COLOR_GREEN, COLOR_BLACK);		   // colore box del menu
	init_pair(PROIETTILE_COL, COLOR_WHITE, COLOR_BLACK);	   // colore proiettile
	init_pair(BORDER_COL, COLOR_GREEN, COLOR_BLACK);		   // colore del bordo del gioco
	init_pair(LAVA_COL, COLOR_BLACK, COLOR_RED);			   // colore lava
	init_pair(LVL_COL, COLOR_BLACK, COLOR_RED);				   // colore hud livello
	init_pair(TEMPO_COL, COLOR_GREEN, COLOR_GREEN);			   // colore hud tempo
	init_pair(PIANTA_COL, COLOR_BLACK, COLOR_GREEN);		   // colore pianta
	init_pair(COCCODRILLOBUONO_COL, COLOR_GREEN, COLOR_WHITE); // colore coccodrillo
	init_pair(COCCODRILLOCATTIVO_COL, COLOR_RED, COLOR_BLACK); // colore coccodrillo cattivo
	init_pair(LAMPEGGIA, COLOR_GREEN, COLOR_WHITE);			   // colore coccodrillo che lampeggia
	return;
}

void creaPipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		perror("Pipe creation failed");
		exit(1);
	}
}

void uccidiProcesso(pid_t target_pid)
{
	kill(target_pid, SIGKILL);

	int err = waitpid(target_pid, NULL, 0);
	if (err == -1)
	{
		perror("Errore nella waitpid");
		exit(1);
	}

	return;
}

void targetUccidiProcesso(pid_t *array_pid, int id_processo)
{
	if ((id_processo != -1) && (array_pid[id_processo] != NOPID))
	{
		uccidiProcesso(array_pid[id_processo]);
		array_pid[id_processo] = NOPID;
	}
}

int id_disponibile(pid_t *array_pid, int lunghezza)
{
	for (int i = 0; i < lunghezza; i++)
	{
		if (array_pid[i] == NOPID)
			return i;
	}
	return NOID;
}

void aggiornaOldPos(PipeData *old_pos, PipeData *pipeData)
{
	setPipeData(old_pos, pipeData->type, pipeData->id, pipeData->x, pipeData->y);
	return;
}

bool isGameOver(GameData *gameData)
{
	return gameData->gameInfo.vite == 0 || gameData->gameInfo.manche == 6;
}

bool isWin(GameData *gameData)
{
	return gameData->gameInfo.manche == 6;
}

bool isFrogMoveLecit(int newX, int newY, RanaAbsPos ranaPos, PipeData pipeData)
{
	if (ranaPos.on_coccodrillo && pipeData.x != 0)
	{
		// la rana è su un coccodrillo e sto cercando di muovermi lungo il coccodrillo
		int new_offset = ranaPos.offset_on_coccodrillo + pipeData.x;
		if (new_offset <= 0 || new_offset > COCCODRILLO_W - RANA_W - 1)
		{
			return false;
		}
	}
	return newX <= LASTGAMECOL - RANA_W + 1 && newX >= FIRSTGAMECOL && newY > FIRSTGAMEROW + RANA_H && newY < LASTGAMEROW;
}

void stampaBox(int option)
{
	int delay = 0;
	if (option == 1)
	{
		delay = SUPERFAST;
	}

	attron(COLOR_PAIR(BORDER_COL));
	// stampa il bordo superiore
	for (int col = BORDERCOLSX; col <= BORDERCOLDX; col++)
	{
		mvaddch(BORDERROWSUP, col, '='); // parte superiore
		refresh();
		usleep(delay);
	}
	// stampa il bordo destro
	for (int row = BORDERROWSUP + 1; row <= BORDERROWINF - 1; row++)
	{
		mvaddch(row, BORDERCOLDX, '|'); // parte destra
		refresh();
		usleep(delay);
	}
	// stampa il bordo inferiore da destra a sinistra
	for (int col = BORDERCOLDX; col >= BORDERCOLSX; col--)
	{
		mvaddch(BORDERROWINF, col, '='); // parte superiore
		refresh();
		usleep(delay);
	}
	// stampa il bordo sinistro dal basso verso l'alto
	for (int row = BORDERROWINF - 1; row >= BORDERROWSUP + 1; row--)
	{
		mvaddch(row, BORDERCOLSX, '|'); // parte destra
		refresh();
		usleep(delay);
	}
	attroff(COLOR_PAIR(BORDER_COL));
	return;
}

int generaRandom_r(int min, int max, unsigned int *seed)
{
	return (rand_r(seed) % (max - min + 1)) + min;
}

int fromFilaToRow(int fila)
{
	int row = 0;
	switch (fila)
	{
	case 1:
		row = FILA_UNO;
		break;
	case 2:
		row = FILA_DUE;
		break;
	case 3:
		row = FILA_TRE;
		break;
	case 4:
		row = FILA_QUATTRO;
		break;
	case 5:
		row = FILA_CINQUE;
		break;
	case 6:
		row = FILA_SEI;
		break;
	case 7:
		row = FILA_SETTE;
		break;
	case 8:
		row = FILA_OTTO;
		break;
	default:
		break;
	}
	return row;
}

void setScreenCell(ScreenCell *screenCell, TipoObj tipo, char ch, int id, int color, bool changed)
{
	screenCell->ch = ch;
	screenCell->color = color;
	screenCell->tipo = tipo;
	screenCell->id = id;
	screenCell->is_changed = changed;
	return;
}

void setPipeData(PipeData *pipeData, char type, int id, int x, int y)
{
	pipeData->type = type;
	pipeData->id = id;
	pipeData->x = x;
	pipeData->y = y;
	return;
}
