/*  Libreria di funzioni per elementi statici del gioco Frogger */

#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include "staticlib.h"  // libreria per la stampa degli elementi statici del gioco


/*	Stampa un carattere su un area di dimensione dim_y e dim_x
		a partire dalle coordinate start_y, start_x	
*/

void printSquare(int start_y, int start_x, int dim_y, int dim_x, char c){
	
	for(int i=0; i<dim_y; i++){ 								//riga
		for(int j=0; j<dim_x; j++){								// colonna
			
			mvaddch(start_y+i, start_x+j, c);
			
		}
	}	
	
}




void stampaMarciapiede(){
	int dim_x = MAXX;				// altezza e larghezza
	int dim_y = 2;
	
	int start_y = MAXY-dim_y; //posizione iniziale Y X del marciapiede (angolo alto-sinistra dell'oggetto)
	int start_x = 0;
	
	printSquare(start_y, start_x, dim_y, dim_x, 'H' );
}


void stampaPrato(){
	int dim_x = MAXX;				// altezza e larghezza
	int dim_y = 3;
	
	int start_y = MAXY/2 - dim_y; //posizione iniziale Y X del prato (angolo alto-sinistra dell'oggetto)
	int start_x = 0;
	
	printSquare(start_y, start_x, dim_y, dim_x, ';' );
}



