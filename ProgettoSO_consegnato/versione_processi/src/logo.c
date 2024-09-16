#include "../hdr/logo.h"
void logo(){
	
	nodelay(stdscr, FALSE); // riabilita l'input bloccante
	
	clear(); // pulisce lo schermo
	
	avviaSuoni();

	riproduciSuono("suoni/intro_test.wav");

	stampaBox(1); // stampa la box attorno al logo con delay
	
	mvprintw(STARTROWBENVENUTO,STARTCOLBENVENUTO,"Benvenuto...");
	refresh();
  
  	stampaTitolo(STARTROWTITOLO,STARTCOLTITOLO); // stampa il titolo
  
  	stampaLogo(STARTROWLOGO,STARTCOLLOGO); // stampa il logo
  
	usleep(400000); // delay per match con suono
	
	// stampa la scritta premi un tasto per continuare
  	mvprintw(STARTROWCONTINUA,STARTCOLCONTINUA,"Premi un tasto per continuare...");
  
  	getch(); // Aspetta che l'utente prema un tasto qualsiasi

  	nodelay(stdscr, TRUE); // riabilita l'input non bloccante

  	exit(0);
}

/** @brief fa partire il processo logo*/
void avviaLogo() {
    pid_t pid_logo;
    pid_logo = fork();

    if (pid_logo < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (pid_logo == 0) {
        logo();
    }
    waitpid(pid_logo, NULL, 0);
	return;
}

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
			usleep(10000);
			refresh();
		}
	}																
}

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

	 // char titoloMancheWin[6][61] = {
    //     "  __  __                  _           __        _____       ",
    //     " |  \\/  | __ _ _ __   ___| |__   ___  \\ \\      / /_ _|_ __  ",
    //     " | |\\/| |/ _` | '_ \\ / __| '_ \\ / _ \\  \\ \\ /\\ / / | || '_ \\ ",
    //     " | |  | | (_| | | | | (__| | | |  __/   \\ V  V /  | || | | |",
    //     " |_|  |_|\\__,_|_| |_|\\___|_| |_|\\___|    \\_/\\_/  |___|_| |_|",
    //     "                                                            "
    // };		

}