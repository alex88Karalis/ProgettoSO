#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "upscreenlib.h"


#define MARCIAPIEDE 1
#define SFONDO 2
#define PRATO 3
#define FIUME 4
#define TANE 5
#define HUD 6
#define TRONCHI 7

#define NUM_ROWS 37
#define NUM_COLS 100	

int main() {
    // Inizializza ncurses
    initscr();
    start_color(); // inizializza colori
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    // Imposta i colori utilizzati
    init_pair(MARCIAPIEDE, COLOR_BLACK, COLOR_WHITE);
		init_pair(SFONDO,COLOR_WHITE,COLOR_BLACK);
		init_pair(PRATO,COLOR_WHITE,COLOR_GREEN);
		init_pair(FIUME,COLOR_WHITE,COLOR_BLUE);
		init_pair(TANE,COLOR_RED,COLOR_YELLOW);
		
		init_pair(HUD,COLOR_WHITE, COLOR_BLACK);
		init_pair(TRONCHI,COLOR_RED, COLOR_YELLOW);
    // Ottieni l'altezza e la larghezza della finestra
    
    
    srand(time(NULL));
    
    
    
    char exitCode='0';
    int height,width,new_height,new_width;
    int num_rows_colored;
    resize_term(NUM_ROWS, NUM_COLS); 					//non modifica la finestra la finestra
    getmaxyx(stdscr, height, width);
    while(exitCode!='q'){
    	
    	// get dell'altezza e della larghezza della finestra di gioco
    	getmaxyx(stdscr, new_height, new_width);
    	
    	if(height!=new_height||width!=new_width){
    		// pulisce marciapiede
    		for (int row = height - num_rows_colored; row < height-1; ++row) {
        	mvchgat(row, 0, width, A_NORMAL, SFONDO, NULL);
    		}
    		// pulisce prato
    		for(int row=height/2;row<height/2+(height/12);row++){
    			mvchgat(row, 0, width, A_NORMAL, SFONDO, NULL);
    		}
    		
    		height=new_height;
    		width=new_width;
    	}
    	
    	// ricolora tutto lo schermo di nero
    	wbkgdset(stdscr, COLOR_PAIR(SFONDO));
    	
    	// Calcola il numero di righe colorate (1/12 dell'altezza)
    	num_rows_colored = height / 12;
    	
    	// Colora le ultime righe dello schermo(marciapiede)
    	for (int row = height - num_rows_colored; row < height; ++row) {
        	mvchgat(row, 0, width, A_NORMAL, MARCIAPIEDE, NULL);
    	}
    	
    	// colora il prato
    	for(int row=height/2;row<height/2+(height/12);row++){
    		mvchgat(row, 0, width, A_NORMAL, PRATO, NULL);
    	}
    	
    	// colora il fiume
    	disegnaFiume(height, NUM_COLS);
    	
    	/*
    	for(int row=5;row<height/2;row++){
    	//int mvchgat(int y, int x, int n, attr_t attr, short color, const void *opts);
    		mvchgat(row, 0, width, A_NORMAL, FIUME, NULL);
    	}
    	/**/
    	
    	int strisce_strada_row= ( (height/2)+(height/12) + (height-num_rows_colored) )/2;
    	// disegna linee stradali
    	mvchgat(strisce_strada_row, 0, width, A_NORMAL, MARCIAPIEDE, NULL);
    	
    	// disegna spazi in strisce stradali
    	for (int col = 0; col<width; ++col) {
    		if(col%3==0){
    			attron(COLOR_PAIR(SFONDO));
        	mvaddch(strisce_strada_row, col,' ');
        	attroff(COLOR_PAIR(SFONDO));
    		}
    	}
    	
    	// colora tane
    	
    	disegnaTane(width);
    	disegnaTronchi(width);
    	
    	/* -------da cancellare------------
    	for(int row=3;row<3+5;row++){
    		mvchgat(row, 0, width, A_NORMAL, TANE, NULL);
    	}
    	
    	bool tana=true;
    	for(int col=0;col<width;col++){
    		if(col%19==0){
    			tana=!tana;
    		}
    		if(!tana){
    			attron(COLOR_PAIR(FIUME));
        	mvaddch(3+2, col,' ');
        	mvaddch(3+3,col,' ');
        	mvaddch(3+4,col,' ' );
        	attroff(COLOR_PAIR(FIUME));	
    		}
    			
    	}
    	--------fine delle tane--------- 
    	/**/ 
    	disegnaHUD(width);
    	// Mostra il risultato sullo schermo
    	refresh();
    	
    	// Attendi l'input dell'utente per uscire
    	exitCode= getch();
    	
    }

    // Ripristina lo stato iniziale e termina ncurses
    endwin();
    return 0;
}
