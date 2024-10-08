#include "disegna.h"
// per far partire il processo che si occupa di disegnare
void avviaDrawProcess(int pipe_fd[2]) {
    pid_t draw_pid = fork();
    if (draw_pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (draw_pid == 0) {
        // Processo "disegna"
        close(pipe_fd[1]); // Chiudi l'estremità di scrittura della pipe, TODO: è davvero utile?
        drawProcess(pipe_fd);
        exit(0); // TODO: è davvero utile?
    }
}

// processo che si occupa di disegnare lo schermo
void drawProcess(int* pipe_fd) {
		
	PipeData pipeData; // struttura per leggere la pipe
	PipeData old_pos [OLDPOSDIM]; // array di strutture che contiene dati oggetti al passo precedente; rana, tronchi x3, auto x4, ...
	inizializzaOldPos(old_pos,OLDPOSDIM);
	
	// definizione sprite utili
	char *sprite_rana[RANA_ROWS] = {"\\./","/-\\"};
	char *sprite_tronco[TRONCO_ROWS] = {"~ ~ ~ ~ ~", " ~ ~ ~ ~ "};
	char *sprite_auto[AUTO_ROWS] = {"xxxx", "oooo"};
	Sprite ranaSprite = {RANA_ROWS, RANA_COLS, sprite_rana, RANA};
  Sprite troncoSprite = {TRONCO_ROWS, TRONCO_COLS, sprite_tronco, TRONCHI};
  Sprite autoSprite = {AUTO_ROWS, AUTO_COLS, sprite_auto, AUTO};
	
  // TODO forse è meglio fare una funzione sola che inizializzi entrambe le matrici, FATTO!
  ScreenCell screenMatrix[HEIGHT][WIDTH]; // matrice che rappresenta lo schermo
	//inizializzaMatriceSchermo(screenMatrix); // inizializzazione matrice che rappresenta lo schermo
	ScreenCell staticScreenMatrix[HEIGHT][WIDTH]; // matrice degli elementi statici dello schermo
	//inizializzaMatriceSchermo(staticScreenMatrix); // inizializzazione matrice degli elementi statici che rappresenta lo schermo
	
	inizializzaMatriceSchermo(screenMatrix, staticScreenMatrix);// inizializza entrambe le matrici
	
	// inizializza vettore per registrare posizione dei proiettili
	
	
	int active_bullets=0;
	int max_bullets = 10;
	
	//pid_t pidBullet[max_bullets]; //vettore dei pid dei processiProiettile
	//pid_t pidBullet; 
	
	
	PipeData old_bullet[10];
	for(int i=0; i< max_bullets; i++){
		old_bullet[i].x = old_bullet[i].y = -1;
		old_bullet[i].type= ' ';
		old_bullet[i].id=i;
		
		//pidBullet[i]=-1; //inizializza tutti i pid a valori non validi
	}
	
	
	
	
	
  while (1) {
  	read(pipe_fd[0], &pipeData, sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe
        
    switch(pipeData.type){
    	case 'X':
    		aggiornaPosizioneOggetto(&pipeData, &old_pos[0], screenMatrix, staticScreenMatrix, &ranaSprite);
        break;
			case 'T':
        aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix,staticScreenMatrix, &troncoSprite);
        break;
      case 'A':
      	aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix, &autoSprite);
        break;
      case 'S':
      	//proiettile sparato da utente
      	// fa partire il processo proiettile se il numero di proiettili in gioco è minore di 10
      	if(active_bullets < max_bullets){
      		avviaProiettile(pipe_fd);
      		active_bullets++;
      		beep();
      	}//end if(troppi proiettili)
      	break;
      case 'P':				//PROBLEMA: non legge il type=='P' da pipe se mandato da proiettile.???
      	//aggiorna posizione dei proiettili
      	// fai il check delle coordinate ed eventualmente fai un flash se il proiettile esce dall'area di gioco
      	if(pipeData.y == 0){
      		flash();
      	}
      	break;
      default:
        break;
    }
		stampaMatrice(screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
    refresh(); // Aggiorna la finestra
	}
  return;  
}
//--------------------------------------------Stampa Puntuale----------------------------------------------------------------------
void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]){
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
		// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE
		// il flag sara TRUE solo se il carattere è stato modificato
			if(screenMatrix[i][j].is_changed){							
				attron(COLOR_PAIR(screenMatrix[i][j].color));
				mvaddch(i, j, screenMatrix[i][j].ch);
				attroff(COLOR_PAIR(screenMatrix[i][j].color));
				screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
			}
		}
	}
	return;
}
// per aggiornamento della posizione al passo precedente degli oggetti dinamici
void aggiornaOldPos(PipeData *old_pos,PipeData *pipeData){
    old_pos->x=pipeData->x;
    old_pos->y=pipeData->y;
    return;
}
//--------------------------------------------AGGIORNAMENTO OGGETTI IN MATRICE--------------------------------
void aggiornaPosizioneOggetto(PipeData *pipeData, PipeData *old_pos, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH], Sprite *sprite) {
    // se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
    if (pipeData->x != old_pos->x || pipeData->y != old_pos->y) {
        pulisciSpriteInMatrice(old_pos->y, old_pos->x, sprite, screenMatrix, staticScreenMatrix);
        stampaSpriteInMatrice(pipeData->y, pipeData->x, sprite, screenMatrix);
        aggiornaOldPos(old_pos, pipeData);
    }
}
//----------------------------------------------------------------
void stampaSpriteInMatrice(int startRow, int startCol, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH]) {
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;

    for (int i = 0; i < maxRows; i++) {
        for (int j = 0; j < maxCols; j++) {
            int row = startRow + i;
            int col = startCol + j;

            screenMatrix[row][col].ch = sprite->sprite[i][j];
            screenMatrix[row][col].color = sprite->color;
            screenMatrix[row][col].is_changed = true;
        }
    }
}
//----------------------------------------------------------------
void pulisciSpriteInMatrice(int row, int col, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]) {
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;
    
    if (row != -1) {
        for (int i = row; i < row + maxRows; i++) {
            for (int j = col; j < col + maxCols; j++) {
                screenMatrix[i][j].ch = staticScreenMatrix[i][j].ch;
                screenMatrix[i][j].color = staticScreenMatrix[i][j].color;
                screenMatrix[i][j].is_changed = true;
            }
        }
    }
}
