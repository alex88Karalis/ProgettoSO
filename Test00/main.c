#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "staticlib.h"

#define MAXX 80     /* Numero di colonne dello schermo */
#define MAXY 24     /* Numero di righe dello schermo */
#define SPRITE_ROWS 2
#define SPRITE_COLS 3

/*-------STRUTTURE--------------------*/

struct posizione {
    int x;        // coordinata x
    int y;        // coordinata y
    char sprite [SPRITE_ROWS] [SPRITE_COLS];
}; 



/*-------DICHIARAZIONI-----------------*/

void stampaSprite(int x,int y, char sprite [SPRITE_ROWS] [SPRITE_COLS]);

/*
void printSquare(int start_y, int start_x, int dim_y, int dim_x, char c);
void stampaMarciapiede(int dim_y, int dim_x);
void stampaPrato(int dim_y, int dim_x);
/**/


int main()
{
  initscr();              // Inizializza lo schermo /
  noecho();             // Disabilita la visualizzazione dei tasti premuti /
  keypad(stdscr, 1);         // Abilita la gestione dei tasti freccia / 
  curs_set(0);             // Nasconde il cursore /

  
  struct posizione oggetto = { MAXX/2, MAXY/2, {{'\\','.','/'},{'/','-','\\'}}	};
	
	
	stampaMarciapiede();
	stampaPrato();
	
  //mvaddch(oggetto.y, oggetto.x, '+'); // Prima visualizzazione al centro /
  stampaSprite(oggetto.x,oggetto.y,oggetto.sprite);
  
  
  refresh();

	while(true){
    int c = getch();
    switch(c) {
	    case KEY_UP:
	        if(oggetto.y > 0) 
	       	 oggetto.y -= 1; 
	        break;

	    case KEY_DOWN:
	        if(oggetto.y < MAXY - 1) 
	          oggetto.y += 1;
	        break;

	    case KEY_LEFT:
	        if(oggetto.x > 0) 
	          oggetto.x -= 1;
	        break;

	    case KEY_RIGHT:
	        if(oggetto.x < MAXX - 4) 
	          oggetto.x += 1;
	        break;

	    case 113: // Tasto 'q' */
	        endwin();
	        exit(0);
    }

    clear();
    
		stampaPrato();
		stampaMarciapiede();
		
  	stampaSprite(oggetto.x,oggetto.y,oggetto.sprite);
        
    //mvaddch(oggetto.y, oggetto.x, '+');
    refresh();
  }
}

/*------------DEF.FUNZIONI-------------------------*/

void stampaSprite(int x,int y, char sprite [SPRITE_ROWS] [SPRITE_COLS]){
  for(int i=0;i<SPRITE_ROWS;i++){
		for(int j=0;j<SPRITE_COLS;j++){
      mvaddch(y + i, x + j,sprite[i][j]);
      fflush(NULL);
    }
  }
}


/*
void printSquare(int start_y, int start_x, int dim_y, int dim_x, char c){
	
	for(int i=0; i<dim_y; i++){ 								//riga
		for(int j=0; j<dim_x; j++){								// colonna
			
			mvaddch(start_y+i, start_x+j, c);
			
		}
	}	
	
}




void stampaMarciapiede(int dim_y, int dim_x){
	int offset = MAXY-dim_y; //posizione Y del marciapiede
	
	printSquare(offset, 0, dim_y, dim_x, 'H' );
	
}

void stampaPrato(int dim_y, int dim_x){
	int offset = 7; //posizione Y del marciapiede
	
	printSquare(offset, 0, dim_y, dim_x, ';' );
	
}
/**/







