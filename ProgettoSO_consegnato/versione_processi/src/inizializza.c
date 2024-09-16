#include "../hdr/inizializza.h"

void inizializza(GameData* gameData){
	
	inizializzaGameInfo(gameData);
	inizializzaOggettiTane(gameData->tane);
	inizializzaFlussi(gameData);
	
	inizializzaSprites(gameData->sprites);
	
	inizializzaOldPos(&(gameData->oldPos));
	
	// inizializza posizione assoluta rana e aggiorna old pos della rana
	inizializzaPosRana(gameData);
	inizializzaSchermo(gameData); 
	
	inizializzaContatori(gameData);
	inizializzaControlloCoccodrilli(gameData);
	return;
}

// inizializza i pid di tutti i processi
void inizializzaPid(GameData* gameData){
	// pid dei nemici
	for(int i=0;i<MAXNNEMICI;i++){
		gameData->pids.pidNemici[i]=NOPID;
	}

	// coccodrilli
	for(int i=0;i<MAXNCOCCODRILLI;i++){
		gameData->pids.pidCoccodrilli[i]=NOPID;
	}

	// proiettili
	for(int i=0;i<MAXNPROIETTILI;i++){
		gameData->pids.pidProiettili[i]=NOPID;
	}

	// proiettili nemici
	for(int i=0;i<MAXNPROIETTILINEMICI;i++){
		gameData->pids.pidProiettiliNemici[i]=NOPID;
	}

	// rana
	gameData->pids.pidRana=NOPID;

	// tempo
	gameData->pids.pidTempo = NOPID;

	// musica
	gameData->pids.pidMusica= NOPID;

	// pid sound player
	gameData->pids.soundPlayer=NOPID;
}

void inizializzaFlussi(GameData* gameData){
	for(int i=0;i<8;i++){
		gameData->flussi[i].direction = rand()%2==0 ? 1 : -1;
		gameData->flussi[i].vel = rand()%3;
		gameData->flussi[i].n_coccodrilli_attivi=0;
	}
}

void inizializzaControlloCoccodrilli(GameData* gameData){
	for(int i=0;i<MAXNCOCCODRILLI;i++){
		gameData->controlloCoccodrilli[i].id=NOID;
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
		gameData->controlloCoccodrilli[i].fila=0;
	}
	return;
}

void inizializzaContatori(GameData* gameData){
	Cont* contatori = &(gameData->contatori);
	contatori->contProiettili=0;
	contatori->contProiettiliN=0;
	contatori->contNemici=0;
	contatori->contCoccodrilli=0;
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

void inizializzaOldPos(OldPos* old_pos){
	for(int i=0; i<MAXNCOCCODRILLI;i++){
		setPipeData(&(old_pos->coccodrilli[i]),'C',NOID,NOPOS,NOPOS);
	}

	for(int i=0; i<MAXNNEMICI;i++){
		setPipeData(&(old_pos->nemici[i]),' ',NOID,NOPOS,NOPOS);
	}
	
	for(int i=0;i<MAXNPROIETTILI;i++){
		setPipeData(&(old_pos->proiettili[i]),'P',NOID,NOPOS,NOPOS);
	}
	for(int i=0;i<MAXNPROIETTILINEMICI;i++){
		setPipeData(&(old_pos->proiettiliNemici[i]),'p',NOID,NOPOS,NOPOS);
	}
   	return;
}

void inizializzaGameInfo(GameData* gamedata){
	GameInfo* gameInfo = &(gamedata->gameInfo);
	gameInfo->secondi_di_gioco=0;
	gameInfo->vite=3;
	gameInfo->punteggio=0;
	gameInfo->livello= 1;
	gameInfo->manche=1;
	gameInfo->ranaIsDead= false;
	gameInfo->mancheWin = false;
}

void inizializzaHUD(GameData* gameData){

	Schermo* schermo = &(gameData->schermo);
	// pulizia parte superiore hud
	for(int row=HUDSUPROWSTART;row<=HUDSUPROWEND;row++){
		for(int col=FIRSTGAMECOL;col<=LASTGAMECOL;col++){
			setScreenCell(&(schermo->screenMatrix[row][col]),HUD_OBJ,' ',0,SFONDO_COL,true);
		}
	}

	printDefaultHudSup(gameData);
	printLvl(gameData);
	printManche(gameData);
	printScore(gameData);

	// pulizia sezione bottom
	for(int row=HUDINFROWSTART;row<=HUDINFROWEND;row++){
		for(int col=FIRSTGAMECOL;col<=LASTGAMECOL;col++){
			setScreenCell(&(schermo->screenMatrix[row][col]),HUD_OBJ,' ',0,SFONDO_COL,true);
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
				char ch= ((rand()%1000)%3==0) ? '`' : ';';
				setScreenCell(&(gameData->schermo.screenMatrix[row][col]),TANA_OPEN_OBJ,ch,id,TANE_COL,true);
			}
		}
	}
	
	// disegno della parte superiore
	for(int i=FULLTANAROWSTART;i<=FULLTANAROWEND;i++){
		for(int j=FIRSTGAMECOL;j<=LASTGAMECOL;j++){
			setScreenCell(&(gameData->schermo.screenMatrix[i][j]),SFONDO_OBJ,' ',0,TANE_COL,true);
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
					setScreenCell(&(gameData->schermo.screenMatrix[i][j]),LAVA_OBJ,' ',0,LAVA_COL,true);
				}
			}
		}
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaArgine(ScreenCell (*screenMatrix)[WIDTH]){
	for(int i = ARGINEROWSTART; i <= ARGINEROWEND; i++){
		for(int j = FIRSTGAMECOL; j <= LASTGAMECOL; j++){
			setScreenCell(&(screenMatrix[i][j]),ARGINE_OBJ,' ',0,ARGINE_COL,true);
		}
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaFiume(ScreenCell (*screenMatrix)[WIDTH]){
	for(int i = FIUMEROWSTART;i <= FIUMEROWEND; i++){
		for(int j = FIRSTGAMECOL; j <= LASTGAMECOL; j++){
			char ch = ((rand()%1000)%3==0) ? '~': ' ';
			setScreenCell(&(screenMatrix[i][j]),FIUME_OBJ,ch,0,FIUME_COL,true);
		}
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaMarciapiede(ScreenCell (*screenMatrix)[WIDTH]){
	for(int i = MARCIAPIEDEROWSTART; i <= MARCIAPIEDEROWEND; i++){
		for(int j = FIRSTGAMECOL; j <= LASTGAMECOL;j++){
			setScreenCell(&(screenMatrix[i][j]),MARCIAPIEDE_OBJ,' ',0,MARCIAPIEDE_COL,true);
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
	char sprite_rana[RANA_H][RANA_W] = {{'\\','.','/'},{'/','-','\\'}};
	char sprite_proiettile[PROIETTILE_H][PROIETTILE_W] = {{'^'},{'|'}};
	char sprite_tana_open[TANA_H][TANA_W] = {{' ',' ',' ',' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' ',' ',' ',' '},{' ',' ',' ',' ',' ',' ',' ',' ',' '}};
	char sprite_tana_closed[TANA_H][TANA_W] = {{'H',' ',' ',' ',' ',' ',' ',' ','H'},{'H',' ',' ',' ',' ',' ',' ',' ','H'},{'H',' ',' ',' ',' ',' ',' ',' ','H'},{'H','H','H','H','H','H','H','H','H'}};
	char sprite_pianta[PIANTA_H][PIANTA_W] = {{'\\','.','/'},{' ','|',' '}};
	char sprite_proiettile_nemico[PROIETTILE_H][PROIETTILE_W] = {{'|'},{'v'}};
	char sprite_coccodrillo_sx[COCCODRILLO_H][COCCODRILLO_W] = {{'>','*','^','^','^','^','^','^','-'},{' ',' ','I',' ',' ',' ','I',' ',' '}};
	char sprite_coccodrillo_dx[COCCODRILLO_H][COCCODRILLO_W] = {{'-','^','^','^','^','^','^','*','<'},{' ',' ','I',' ',' ',' ','I',' ',' '}};
	char sprite_coccodrillo_sx_c[COCCODRILLO_H][COCCODRILLO_W] = {{'>','*','^','^','^','^','^','^','-'},{' ',' ','I',' ',' ',' ','I',' ',' '}};
	char sprite_coccodrillo_dx_c[COCCODRILLO_H][COCCODRILLO_W] = {{'-','^','^','^','^','^','^','*','<'},{' ',' ','I',' ',' ',' ','I',' ',' '}};

	sprites[S_RANA] = inizializzaSprite(RANA_H, RANA_W,sprite_rana, RANA_COL,RANA_OBJ);
  	sprites[S_PROIETTILE] = inizializzaSprite(PROIETTILE_H, PROIETTILE_W, sprite_proiettile, PROIETTILE_COL,P_OBJ);
	sprites[S_TANA_APERTA] = inizializzaSprite(TANA_H, TANA_W, sprite_tana_open, TANE_COL,TANA_OPEN_OBJ);
	sprites[S_TANA_CHIUSA] = inizializzaSprite(TANA_H, TANA_W, sprite_tana_closed, TANE_COL,TANA_CLOSE_OBJ);
	sprites[S_PIANTA] = inizializzaSprite(PIANTA_H,PIANTA_W,sprite_pianta,PIANTA_COL,N_OBJ);
	sprites[S_PROIETTILE_NEMICO] = inizializzaSprite(PROIETTILE_H,PROIETTILE_W,sprite_proiettile_nemico,PROIETTILE_COL,PN_OBJ);
	sprites[S_COCCODRILLO_SX] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_sx,COCCODRILLOBUONO_COL,COCCODRILLO_BUONO_OBJ);
	sprites[S_COCCODRILLO_DX] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_dx,COCCODRILLOBUONO_COL,COCCODRILLO_BUONO_OBJ);
	sprites[S_COCCODRILLO_SX_C] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_sx_c,COCCODRILLOCATTIVO_COL,COCCODRILLO_CATTIVO_OBJ);
	sprites[S_COCCODRILLO_DX_C] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_dx_c,COCCODRILLOCATTIVO_COL,COCCODRILLO_CATTIVO_OBJ);
	sprites[S_COCCODRILLO_SX_L] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_sx_c,LAMPEGGIA,COCCODRILLO_BUONO_OBJ);
	sprites[S_COCCODRILLO_DX_L] = inizializzaSprite(COCCODRILLO_H,COCCODRILLO_W,sprite_coccodrillo_dx_c,LAMPEGGIA,COCCODRILLO_BUONO_OBJ);
	return;
}
Sprite inizializzaSprite(int rows, int cols, char sprite[rows][cols], int color, TipoObj tipo_oggetto) {
    Sprite spriteObj;
    spriteObj.max_row = rows;
    spriteObj.max_col = cols;
	spriteObj.tipo=tipo_oggetto;
    // Alloca memoria per la matrice di sprite e copia i dati
    spriteObj.sprite = malloc(rows * sizeof(char *));
    if (spriteObj.sprite == NULL) {
        fprintf(stderr, "Errore nell'allocazione della matrice di sprite\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        spriteObj.sprite[i] = malloc(cols * sizeof(char));
        if (spriteObj.sprite[i] == NULL) {
            fprintf(stderr, "Errore nell'allocazione della riga %d della matrice di sprite\n", i);
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < cols; j++) {
            spriteObj.sprite[i][j] = sprite[i][j];
        }
    }
    spriteObj.color = color;
    return spriteObj;
}


void inizializzaBox(GameData* gameData){
	Schermo* schermo = &(gameData->schermo);

    // stampa il bordo superiore e inferiore
    for(int col = BORDERCOLSX; col <= BORDERCOLDX; col++){
		// parte superiore
		setScreenCell(&(schermo->screenMatrix[BORDERROWSUP][col]),SFONDO_OBJ,'=',0,BORDER_COL,true);
		// parte inferiore
		setScreenCell(&(schermo->screenMatrix[BORDERROWINF][col]),SFONDO_OBJ,'=',0,BORDER_COL,true);
    }
    // stampa il bordo destro e sinistro
    for(int row = BORDERROWSUP + 1; row<= BORDERROWINF - 1; row++){
		// parte sinistra
		setScreenCell(&(schermo->screenMatrix[row][BORDERCOLSX]),SFONDO_OBJ,'|',0,BORDER_COL,true);
		// parte destra
		setScreenCell(&(schermo->screenMatrix[row][BORDERCOLDX]),SFONDO_OBJ,'|',0,BORDER_COL,true);
    }
    return;
}