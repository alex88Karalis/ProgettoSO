#include <ncurses.h>
#include "upscreenlib.h"
#include <stdlib.h>
#include <time.h>



void disegnaTane(int width){
	int start_y = 3;
	
	int freqTane = NUM_COLS/NUM_TANE ; // ogni quanti char c'è una tana
	int colsTana = 9;									// larghezza tana (numero di char)
	int rowTana = 3; 							// altezza delle tane
	int rowTotal = rowTana+2; 	// altezza totale col bordo superiore
	
	
	// colora tane
    	for(int row=start_y ;row < start_y+rowTotal; row++){
    		mvchgat(row, 0, width, A_NORMAL, TANE, NULL);
    	}
    	
    	bool tana=true;
    	for(int col=0;col<width;col++){
    		if(col%(freqTane -colsTana)==0){
    			tana=!tana;
    		}
    		if(!tana){
    			attron(COLOR_PAIR(FIUME));
        	mvaddch(rowTana+2, col,' ');
        	mvaddch(rowTana+3,col,' ');
        	mvaddch(rowTana+4,col,' ' );
        	attroff(COLOR_PAIR(FIUME));
    		}
    			
    	}
}// end DisegnaTane

void disegnaHUD(int width){
	int score = rand()%100;
	char* widthSize = "Larghezza: %3d";
	attron(COLOR_PAIR(HUD));
	
	move(0,0); // sposta cursore a inizio riga
	clrtoeol(); // pulisce tutta la riga a destra del cursore
	
	mvprintw(0,1, widthSize, width );						// stampa larghezza della finestra
	mvprintw(0, width/2, "SCORE: %3d", score);  //stampa punteggio
}

// Disegna un tronco a partire da riga e colonna dati
void disegnaTronco(int row, int col){
	SpriteTronco t = {{{"~ ~ ~ ~ "},
										 {" ~ ~ ~ ~"}}};	// 2 righe x 8 char
	attron(COLOR_PAIR(TRONCHI));
	
	// ogni tronco occupa due righe, stampa riga per riga
	for(int tmp_row=0; tmp_row<2; tmp_row++){
		mvprintw(row+tmp_row, col, t.sprite[tmp_row]);
	}
	
}


void disegnaFiume(int height, int width){
	srand(time(NULL));
	
	char flow[20];
	int fate=rand()%15;   //+rand()%(20-1+1); //numero random (0 - 20)
	flow[fate]='s';
	
	// colora il fiume
	for(int row=5;row<height/2;row++){
	//int mvchgat(int y, int x, int n, attr_t attr, short color, const void *opts);
		mvchgat(row, 0, width, A_NORMAL, FIUME, NULL);
		
		if(row>8){
			attron(COLOR_PAIR(FIUME));
			//mvprintw(row, 1, flow);
			for(int col=0; col<width; col++)
			{
				fate=rand()%1000;
				if(fate%3==0)
					mvaddch(row,col, '~');
			}
		}
	}
	
	// stampe di debug
	mvprintw(2, 1, "Flow fate: %d ", fate);
}



// Questa funzione può non servire usando direttamente la disegnaTronco() con riga e col di ogni tronco
void disegnaTronchi(int width){
	int rowFiume=10;		// grande 10 righe
	int startFiume= 9;  		//inizia alla riga 9
	int endFiume= startFiume + rowFiume; //termina alla riga 19
	
	// DisegnaTronco(row, col)
	disegnaTronco(10, 15);
	disegnaTronco(15, 15);

}













