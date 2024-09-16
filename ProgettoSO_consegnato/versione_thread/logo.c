#include "logo.h"


/** @brief Stampa il logo (la Rana) a partire dalle coordinate di inizio 
 * @param start_row Riga di inizio (coordinata Y)
 * @param start_col Colonna di inizio (coordinata X)
*/
void stampaLogo(int start_row,int start_col){
	char logoImg[DIMLOGOROW][DIMLOGOCOL]={
																		{' ',' ',' ',' ',' ',' ',' ','_',' ',' ',' ','_',' ',' ',' ',' ',' ',' ',' '},
																		{' ',' ',' ',' ',' ',' ','(','.',')','_','(','.',')',' ',' ',' ',' ',' ',' '},
																		{' ',' ',' ','_',' ','(',' ',' ',' ','_',' ',' ',' ',')',' ','_',' ',' ',' '},
																		{' ',' ','/',' ','\\','/','\'','-','-','-','-','-','\'','\\','/',' ','\\',' ',' '},
																		{'_','_','\\',' ','(',' ','(',' ',' ',' ',' ',' ',')',' ',')',' ','/','_','_'},
																		{')',' ',' ',' ','/','\\',' ','\\','.','_','.','/',' ','/','\\',' ',' ',' ','('},
																		{' ',')','_','/',' ','/','|','\\',' ',' ',' ','/','|','\\',' ','\\','_','(',' '}
																	};
	for(int row=start_row;row<start_row+DIMLOGOROW;row++){
		for(int col=start_col;col<start_col+DIMLOGOCOL;col++){
			attron(COLOR_PAIR(RANA_COL));
			mvaddch(row, col, logoImg[row - start_row][col - start_col]);
			attroff(COLOR_PAIR(RANA_COL));
			usleep(5000);
			refresh();
		}
	}																
}

/** @brief Stampa il titolo a partire dalle coordinate di inizio 
 * @param start_row Riga di inizio (coordinata Y)
 * @param start_col Colonna di inizio (coordinata X)
*/
void stampaTitolo(int start_row,int start_col){

	char titoloMatrix[DIMTITOLOROW][DIMTITOLOCOL] = {
        " _____                   _____                ",
        "|  ___| __ ___   __ _   |  ___| __ ___   __ _ ",
        "| |_ | '__/ _ \\ / _` |  | |_ | '__/ _ \\ / _` |",
        "|  _|| | | (_) | (_| |  |  _|| | | (_) | (_| |",
        "|_|  |_|  \\___/ \\__,  | |_|  |_|  \\___/ \\__, |",
        "                |___/                   |___/ "
    };
    
    for(int row=start_row;row<start_row+DIMTITOLOROW;row++){
			for(int col=start_col;col<start_col+DIMTITOLOCOL;col++){
				attron(COLOR_PAIR(RANA_COL));
				mvaddch(row, col, titoloMatrix[row - start_row][col - start_col]);
				attroff(COLOR_PAIR(RANA_COL));
				usleep(10000);
				refresh();
			}
	}		

}
