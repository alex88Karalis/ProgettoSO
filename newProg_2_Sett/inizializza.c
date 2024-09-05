#include "inizializza.h"
void inizializza(GameData* gameData, int* pipe_fd){
	gameData->pipe= pipe_fd;
	inizializzaGameInfo(gameData);
	inizializzaOggettiTane(gameData->tane);
	inizializzaFlussi(gameData);
	
	inizializzaSprites(gameData->sprites);
	
	inizializzaOldPos(&(gameData->oldPos));
	
	// inizializza posizione assoluta rana e aggiorna old pos della rana
	inizializzaPosRana(&(gameData->ranaAbsPos));
	gameData->oldPos.rana.x=gameData->ranaAbsPos.x;
	gameData->oldPos.rana.y=gameData->ranaAbsPos.y;

	inizializzaSchermo(gameData); 
	
	inizializzaContatori(gameData);
	inizializzaTempo(gameData);
	inizializzaTCB(gameData->allTCB);
	inizializzaControlloCoccodrilli(gameData);
	return;
}

void inizializzaFlussi(GameData* gameData){
	for(int i=0;i<8;i++){
		gameData->flussi[i].direction = rand()%2==0 ? 1 : -1;
		gameData->flussi[i].vel = rand()%3;
	}
	
}

void inizializzaControlloCoccodrilli(GameData* gameData){
	for(int i=0;i<MAXNCOCCODRILLI;i++){
		gameData->controlloCoccodrilli[i].id=-1;
		gameData->controlloCoccodrilli[i].direction=0;
		gameData->controlloCoccodrilli[i].offset_deep=0;
		gameData->controlloCoccodrilli[i].is_buono=false;
		gameData->controlloCoccodrilli[i].is_going_deep=false;
		gameData->controlloCoccodrilli[i].is_going_up=false;
		gameData->controlloCoccodrilli[i].passi=0;
		gameData->controlloCoccodrilli[i].passi_in_immersione=0;
		gameData->controlloCoccodrilli[i].is_fase_pre_immersione=false;
		gameData->controlloCoccodrilli[i].is_fase_immersione=false;
		gameData->controlloCoccodrilli[i].passi_in_pre_immersione=0;
		gameData->controlloCoccodrilli[i].passi_deep=0;
		gameData->controlloCoccodrilli[i].rana_on=false;
		gameData->controlloCoccodrilli[i].offset_rana=0;
	}
	return;
}



void inizializzaTCB(AllTCB *array_tcb){
	//ThreadControlBlock *rana = &(array_tcb->tcb_rana);
	ThreadControlBlock *rana = array_tcb->tcb_rana;
	ThreadControlBlock *proiettili = array_tcb->tcb_proiettili;
	ThreadControlBlock *piante = array_tcb->tcb_piante;
	ThreadControlBlock *coccodrilli = array_tcb->tcb_coccodrilli;
	ThreadControlBlock *proiettili_nemici = array_tcb->tcb_proiettili_nemici;
	ThreadControlBlock standard = {0,false,false};
	
	/*	RANA	*/
	*(rana) = standard;
	
	for(int i=0; i<MAXNPROIETTILI; i++){	// PROIETTILI
		proiettili[i] = standard;
	}

	for(int i=0; i<MAXNCOCCODRILLI; i++){	// COCCODRILLI
		coccodrilli[i] = standard;
	}

	for(int i=0; i<MAXNNEMICI; i++){		// PIANTE-NEMICI
		piante[i] = standard;
	}

	for(int i=0; i<MAXNPROIETTILINEMICI; i++){	// PROIETTILI NEMICI
		proiettili_nemici[i] = standard;
	}

}



void inizializzaContatori(GameData* gameData){
	Cont* contatori = &(gameData->contatori);
	contatori->contProiettili=0;
	contatori->contProiettiliN=0;
	contatori->contNemici=0;
	contatori->contCoccodrilli=0;
	return;
}

// Imposta la posizione iniziale della Rana nella matrice Schermo
void inizializzaPosRana(RanaAbsPos* ranaPos){
	ranaPos->x=WIDTH/2;
	ranaPos->y=MARCIAPIEDEROWSTART;
	
	ranaPos->on_coccodrillo = false;
	ranaPos->id_coccodrillo = -1;		// inizializzo a valore non valido
	ranaPos->offset_on_coccodrillo = -1;
	return;
}

void inizializzaSchermo(GameData* gameData){
	inizializzaFlagMatrice(gameData->schermo.screenMatrix); // inizializza a true i flag di modifica della matrice
	inizializzaBox(gameData);
	inizializzaHUD(gameData); // inizializza le prime 4 righe
	inizializzaTane(gameData); // inizializza la parte dello schermo dedicata alle tane
	inizializzaFiume(gameData->schermo.screenMatrix); // inizializza la parte dello schermo dedicata al fiume
	inizializzaArgine(gameData->schermo.screenMatrix); // inizializza la parte dello schermo dedicata al prato
	inizializzaMarciapiede(gameData->schermo.screenMatrix); // inizializza la parte dello schermo dedicata al marciapiede
	
	copiaMatrice(gameData->schermo.screenMatrix, gameData->schermo.staticScreenMatrix ); //copia la matrice dinamica in quella statica
	return;
}

void avviaProcessiBase(int* pipe_fd,Pids* pids){
	// avvia il processo che gestisce il movimento della rana
	pids->pidRana = avviaRana(pipe_fd); 
	return;
}

pthread_t avviaThreadBase(Pids* pids, Params *param_general){
	// avvia il processo che gestisce il movimento della rana
	pids->pidRana = avviaRanaThread(param_general); 
	return pids->pidRana;
}


void inizializzaOldPos(OldPos* old_pos){
	for(int i=0; i<MAXNCOCCODRILLI;i++){
		old_pos->coccodrilli[i].x=-1;
		old_pos->coccodrilli[i].y=-1;
		old_pos->coccodrilli[i].type=' ';
		old_pos->coccodrilli[i].id=i;
		old_pos->coccodrilli[i].thread_id=0;	// nuovo

	}

	for(int i=0; i<MAXNNEMICI;i++){
		old_pos->nemici[i].x=-1;
		old_pos->nemici[i].y=-1;
		old_pos->nemici[i].type=' ';
		old_pos->nemici[i].id=i;
		old_pos->nemici[i].thread_id=0;			// nuovo

	}
	
	for(int i=0;i<MAXNPROIETTILI;i++){
			old_pos->proiettili[i].x=-1;
			old_pos->proiettili[i].y=-1;
			old_pos->proiettili[i].type='P';
			old_pos->proiettili[i].id=i;
			old_pos->proiettili[i].thread_id=0;		
	}
	for(int i=0;i<MAXNPROIETTILINEMICI;i++){
			old_pos->proiettiliNemici[i].x=-1;
			old_pos->proiettiliNemici[i].y=-1;
			old_pos->proiettiliNemici[i].type='p';
			old_pos->proiettiliNemici[i].id=i;
			old_pos->proiettiliNemici[i].thread_id=0;		

	}
   	return;
}

void inizializzaGameInfo(GameData* gamedata){
	GameInfo* gameInfo = &(gamedata->gameInfo);
	gameInfo->tempo.secondi=0;
	gameInfo->vite=3;
	gameInfo->punteggio=0;
	gameInfo->livello= 1;
	gameInfo->manche=1;
}

void inizializzaHUD(GameData* gameData){

	Schermo* schermo = &(gameData->schermo);
	// pulizia parte superiore hud
	for(int row=HUDSUPROWSTART;row<=HUDSUPROWEND;row++){
		for(int col=FIRSTGAMECOL;col<=LASTGAMECOL;col++){
			schermo->screenMatrix[row][col].ch = ' ';
			schermo->screenMatrix[row][col].color = SFONDO_COL;
			schermo->screenMatrix[row][col].id = 0;
			schermo->screenMatrix[row][col].tipo = HUD_OBJ;
		}
	}

	printDefaultHudSup(gameData);
	printLvl(gameData);
	printManche(gameData);
	printScore(gameData);

	// pulizia sezione bottom
	for(int row=HUDINFROWSTART;row<=HUDINFROWEND;row++){
		for(int col=FIRSTGAMECOL;col<=LASTGAMECOL;col++){
			schermo->screenMatrix[row][col].ch = ' ';
			schermo->screenMatrix[row][col].color = SFONDO_COL;
			schermo->screenMatrix[row][col].id = 0;
			schermo->screenMatrix[row][col].tipo = HUD_OBJ;
		}
	}
	printVite(gameData);
	printInitTempo(gameData);
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaOggettiTane(Tana* tane){
		for(int i = 0; i < MAXNTANE; i++){
		tane[i].info.y = TANAROWSTART;
		tane[i].info.x = (i * DISTTRATANE) + FIRSTGAMECOL;
		tane[i].info.id = i;
		tane[i].status = TANA_OPENED;
		}
		return;
}
void inizializzaTane(GameData* gameData){

	// disegno degli oggetti tana
	// scorro array oggetti Tana che ha 5 elementi
	for(int i=0;i<MAXNTANE;i++){
		// per ogni elemento estraggo y, x, id
		int start_col = gameData->tane[i].info.x;
		int start_row = gameData->tane[i].info.y;
		int id = gameData->tane[i].info.id;
		
		// scrivo sulla matrice schermo la singola tana
		for(int row = start_row; row <= TANAROWEND; row++){
			for(int col = start_col; col < TANA_W + start_col; col++){
				// generazione random dello sprite della tana
				int r=rand()%1000;
				gameData->schermo.screenMatrix[row][col].ch = (r%2==0) ? '`' : ';';
				gameData->schermo.screenMatrix[row][col].color = TANE_COL;
				gameData->schermo.screenMatrix[row][col].tipo = TANA_OPEN_OBJ;
				gameData->schermo.screenMatrix[row][col].id=id;
			}
		}
	}
	
	// disegno della parte superiore
	for(int i=FULLTANAROWSTART;i<=FULLTANAROWEND;i++){
		for(int j=FIRSTGAMECOL;j<=LASTGAMECOL;j++){
			gameData->schermo.screenMatrix[i][j].ch = ' ';
			gameData->schermo.screenMatrix[i][j].color = TANE_COL;
			gameData->schermo.screenMatrix[i][j].tipo = SFONDO_OBJ;
			gameData->schermo.screenMatrix[i][j].id=0;
		}
	}
	
	// disegno del fiume tra una tana e l'altra
	for(int k = 0; k < MAXNTANE; k++){
		// estraggo dagli oggetti tana i bordi destri delle tane
		int start_col = gameData->tane[k].info.x + TANA_W;
		// la larghezza del tratto di fiume tra una tana e l'altra è di 13
		int end_col = start_col + NOTANA_W;
		
		// disegno dei singoli spazi
		for(int i = TANAROWSTART; i <= TANAROWEND; i++){
			for(int j=start_col;j<end_col;j++){
				//disegno solo se non sforo la larghezza dello schermo
				if(j <= LASTGAMECOL){
					gameData->schermo.screenMatrix[i][j].ch = ' ';
					gameData->schermo.screenMatrix[i][j].color = LAVA_COL;
					gameData->schermo.screenMatrix[i][j].tipo = LAVA_OBJ;
					gameData->schermo.screenMatrix[i][j].id=0;
				}
			}
		}
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaArgine(ScreenCell (*screenMatrix)[WIDTH]){
	char carattere=' ';
	for(int i = ARGINEROWSTART; i <= ARGINEROWEND; i++){
		for(int j = FIRSTGAMECOL; j <= LASTGAMECOL; j++){
			
			int n_rand=rand()%1000;	// genera numero random
			screenMatrix[i][j].ch = (n_rand%7==0) ? ';': ' ';

			//screenMatrix[i][j].ch = carattere;
			screenMatrix[i][j].color = ARGINE_COL;
			screenMatrix[i][j].tipo = ARGINE_OBJ;
			screenMatrix[i][j].id = 0;
		}
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaFiume(ScreenCell (*screenMatrix)[WIDTH]){
	for(int i = FIUMEROWSTART;i <= FIUMEROWEND; i++){
		for(int j = FIRSTGAMECOL; j <= LASTGAMECOL; j++){
			
			int n_rand=rand()%1000;	// genera numero random
			screenMatrix[i][j].ch = (n_rand%3==0) ? '~': ' ';
			screenMatrix[i][j].color = FIUME_COL;
			screenMatrix[i][j].tipo = FIUME_OBJ;
			screenMatrix[i][j].id = 0;
		}
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaMarciapiede(ScreenCell (*screenMatrix)[WIDTH]){
	for(int i = MARCIAPIEDEROWSTART; i <= MARCIAPIEDEROWEND; i++){
		for(int j = FIRSTGAMECOL; j <= LASTGAMECOL;j++){
			screenMatrix[i][j].ch = ' ';
			screenMatrix[i][j].color = MARCIAPIEDE_COL; 
			screenMatrix[i][j].tipo = MARCIAPIEDE_OBJ;
			screenMatrix[i][j].id = 0;
		}
	}
	return;
}
//--------------------------------------------------------------------------------------------------------------------------------
void inizializzaFlagMatrice(ScreenCell (*screenMatrix)[WIDTH]){
	for(int row=0; row < HEIGHT; row++){
		for(int cols=0; cols < WIDTH; cols++){
			screenMatrix[row][cols].is_changed = true;
		}
	} 
	return;
}
//----------------------------------------------------------------------------
void copiaMatrice(ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]){
	for (int row=0; row<HEIGHT; row++){
		for (int col=0; col<WIDTH; col++){
			staticScreenMatrix[row][col] = screenMatrix[row][col];
		}
	}
	return;
}

void inizializzaSprites(Sprite* sprites){
	// definizione sprite utili
	
	char *sprite_rana[RANA_H] = {"\\./","/-\\"};
	char *sprite_proiettile[PROIETTILE_H]={"^","|"};
	char *sprite_tana_open[TANA_H] = {"         ", "         ", "         ", "         "};
	char *sprite_tana_closed[TANA_H] = {"H       H","H       H", "H       H", "HHHHHHHHH"};
    char *sprite_pianta[PIANTA_H] = {"\\./"," | "};
	char *sprite_proiettile_nemico[PROIETTILE_H]={"|","v"};
	char *sprite_coccodrillo_sx[COCCODRILLO_H] ={">*^^^^^^-","  I   I  "};
	char *sprite_coccodrillo_dx[COCCODRILLO_H] ={"-^^^^^^*<","  I   I  "};
	char *sprite_coccodrillo_sx_c[COCCODRILLO_H] = {">*^^^^^^-","  I   I  "};
	char *sprite_coccodrillo_dx_c[COCCODRILLO_H] = {"-^^^^^^*<","  I   I  "};

	//char sprite_coccodrillo_sx_c[COCCODRILLO_H][COCCODRILLO_W] = {{'>','*','^','^','^','^','^','^','-'},{' ',' ','I',' ',' ',' ','I',' ',' '}};
	//char sprite_coccodrillo_dx_c[COCCODRILLO_H][COCCODRILLO_W] = {{'-','^','^','^','^','^','^','*','<'},{' ',' ','I',' ',' ',' ','I',' ',' '}};

	sprites[S_RANA] = inizializzaSprite(RANA_H, RANA_W, sprite_rana, RANA_COL, RANA_OBJ);
  	sprites[S_PROIETTILE] = inizializzaSprite(PROIETTILE_H, PROIETTILE_W, sprite_proiettile, PROIETTILE_COL, PROIETTILE_OBJ);
	sprites[S_TANA_APERTA] = inizializzaSprite(TANA_H, TANA_W, sprite_tana_open, TANE_COL, TANA_OPEN_OBJ);
	sprites[S_TANA_CHIUSA] = inizializzaSprite(TANA_H, TANA_W, sprite_tana_closed, TANE_COL, TANA_CLOSE_OBJ);
	sprites[S_PIANTA] = inizializzaSprite(PIANTA_H,PIANTA_W,sprite_pianta, PIANTA_COL, NEMICO_OBJ);
	sprites[S_PROIETTILE_NEMICO] = inizializzaSprite(PROIETTILE_H,PROIETTILE_W,sprite_proiettile_nemico, PROIETTILE_COL, PROIETTILE_NEMICO_OBJ);
	sprites[S_COCCODRILLO_SX] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_sx, COCCODRILLOBUONO_COL, COCCODRILLO_BUONO_OBJ);
	sprites[S_COCCODRILLO_DX] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_dx, COCCODRILLOBUONO_COL, COCCODRILLO_BUONO_OBJ);
	sprites[S_COCCODRILLO_SX_C] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_sx_c,COCCODRILLOCATTIVO_COL,COCCODRILLO_CATTIVO_OBJ);
	sprites[S_COCCODRILLO_DX_C] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_dx_c,COCCODRILLOCATTIVO_COL,COCCODRILLO_CATTIVO_OBJ);
	sprites[S_COCCODRILLO_SX_L] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_sx_c,LAMPEGGIA,COCCODRILLO_BUONO_OBJ);
	sprites[S_COCCODRILLO_DX_L] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_dx_c,LAMPEGGIA,COCCODRILLO_BUONO_OBJ);
	
	return;
}
/**/

Sprite inizializzaSprite(int rows, int cols, char **sprite, int color, TipoObj tipoObj)  {
    Sprite spriteObj;
    spriteObj.max_row = rows;
    spriteObj.max_col = cols;
	spriteObj.tipo = tipoObj;

    // Alloca memoria per la matrice di sprite e copia i dati
    spriteObj.sprite = (char **)malloc(rows * sizeof(char *));
    if (spriteObj.sprite == NULL) {
        fprintf(stderr, "Errore nell'allocazione della matrice di sprite\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        //spriteObj.sprite[i] = (char *)malloc((cols) * sizeof(char));				// correzione sulle cols da allocare
        spriteObj.sprite[i] = (char *)malloc((cols + 1) * sizeof(char));
        if (spriteObj.sprite[i] == NULL) {
            fprintf(stderr, "Errore nell'allocazione della riga %d della matrice di sprite\n", i);
            exit(EXIT_FAILURE);
        }
        strcpy(spriteObj.sprite[i], sprite[i]);
    }
    //spriteObj.sprite = sprite;
    spriteObj.color = color;
    return spriteObj;
}
/**/

void inizializzaBox(GameData* gameData){
	Schermo* schermo = &(gameData->schermo);

    // stampa il bordo superiore e inferiore
    for(int col = BORDERCOLSX; col <= BORDERCOLDX; col++){
		// parte superiore
		schermo->screenMatrix[BORDERROWSUP][col].ch = '=';
		schermo->screenMatrix[BORDERROWSUP][col].color = BORDER_COL;
		schermo->screenMatrix[BORDERROWSUP][col].tipo = SFONDO_OBJ;
		schermo->screenMatrix[BORDERROWSUP][col].id = 0;
		// parte inferiore
		schermo->screenMatrix[BORDERROWINF][col].ch = '=';
		schermo->screenMatrix[BORDERROWINF][col].color = BORDER_COL;
		schermo->screenMatrix[BORDERROWINF][col].tipo = SFONDO_OBJ;
		schermo->screenMatrix[BORDERROWINF][col].id = 0;
    }
    // stampa il bordo destro e sinistro
    for(int row = BORDERROWSUP + 1; row<= BORDERROWINF - 1; row++){
		// parte sinistra
		schermo->screenMatrix[row][BORDERCOLSX].ch = '|';
		schermo->screenMatrix[row][BORDERCOLSX].color = BORDER_COL;
		schermo->screenMatrix[row][BORDERCOLSX].tipo = SFONDO_OBJ;
		schermo->screenMatrix[row][BORDERCOLSX].id = 0;
		// parte destra
		schermo->screenMatrix[row][BORDERCOLDX].ch = '|';
		schermo->screenMatrix[row][BORDERCOLDX].color = BORDER_COL;
		schermo->screenMatrix[row][BORDERCOLDX].tipo = SFONDO_OBJ;
		schermo->screenMatrix[row][BORDERCOLDX].id = 0;
    }
    return;
}

void inizializzaTempo(GameData* gameData){
	gameData->gameInfo.tempo.start = time(NULL);
	return;
}