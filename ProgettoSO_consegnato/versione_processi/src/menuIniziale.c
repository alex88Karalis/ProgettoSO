#include "../hdr/menuIniziale.h"

void menuIniziale(){
	clear(); // pulisce lo schermo
	
	int scelta=0;
	int sottolineato=0;
	bool stay=true;
	avviaSuoni();
	// load della musica di gioco la musica deve essere formato mp3
	Mix_Music *music = Mix_LoadMUS("suoni/lvl_music.mp3");
	// load del suono di swith tra opzioni
	Mix_Chunk *switch_sound = caricaSuono("suoni/change_option.wav");
	// load del suono di invio della scelta
	Mix_Chunk *enter_sound = caricaSuono("suoni/menu_option_selected.wav");
	// play della musica di gioco in loop
	int volume = 40; // Valore tra 0 (muto) e 128 (massimo)
	Mix_VolumeMusic(volume);
	Mix_PlayMusic(music, -1);
	stampaBox(0);
	stampaLogoMenu(STARTROWLOGOMENU,STARTCOLLOGOMENU);
	boxMenu(STARTROWBLUBOX,STARTCOLBLUBOX,ENDROWBLUBOX,ENDCOLBLUBOX);
	refresh(); // rende visibili le modifiche allo schermo
	
	// ciclo per il menu
	while(stay){
		// stampa menu
		for(int i=0;i<NUMOP;i++){
			stampaOpzione(i,sottolineato);
		}
		refresh();
		
		scelta=getch();
		switch(scelta){
			case KEY_UP:
			case 'w':
			case 'W':
				if(sottolineato>0){
					sottolineato--;
					Mix_PlayChannel(-1, switch_sound, 0);
				}
				break;
			case KEY_DOWN:
			case 's':
			case 'S':
				if(sottolineato<NUMOP-1){
					sottolineato++;
					Mix_PlayChannel(-1, switch_sound, 0);
				}
				break;
			case 10:
				stay=false; // per uscire
				Mix_PlayChannel(-1, enter_sound, 0);
				SDL_Delay(1000);
				break;
			default:
				break;
		}
		
	}
	
	clear();
	refresh();

	// libera spazio dedicato ai suoni
	Mix_FreeChunk(switch_sound);
	Mix_FreeChunk(enter_sound);
	Mix_FreeMusic(music);

	// chiusura sdl e sdl mixer
	Mix_CloseAudio();
	SDL_Quit();

	exit(sottolineato);
}

/** @brief fa partire il processo che menuIniziale*/
int avviaMenuIniziale() {
    pid_t pid_menu_iniziale;

    pid_menu_iniziale = fork();

    if (pid_menu_iniziale < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (pid_menu_iniziale == 0) {
        menuIniziale();
    }
	int status;
	// Aspetta che il processo figlio termini
    waitpid(pid_menu_iniziale, &status, 0);

    // Estrai il vero valore di ritorno
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }

    return -1; // Se il processo figlio non è terminato normalmente
}

void stampaLogoMenu(int start_row,int start_col){
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
		}
	}																
}

void stampaOpzione(int i,int sottolineato){
	char opzioni[NUMOP][DIMOP]={"|-      Start      -|","|-      Esci       -|"};
	// per la stampa del sottolineato
			if(i==sottolineato){
				attron(COLOR_PAIR(RANA_COL));
				mvprintw((i*4)+STARTROWOP,STARTCOLOP,"---------------------");
				for(int j=0;j<DIMOP;j++){
					mvaddch((i*4)+1+STARTROWOP,STARTCOLOP+j,opzioni[i][j]);
				}
				mvprintw((i*4)+2+STARTROWOP,STARTCOLOP,"---------------------");
				mvaddch((i*4)+1+STARTROWOP,STARTCOLOP-2,'>');
				attroff(COLOR_PAIR(RANA_COL));
			}
			else{
				// stampa opzioni non sottolineate
				attron(COLOR_PAIR(SFONDO_COL));
				mvprintw((i*4)+STARTROWOP,STARTCOLOP,"---------------------");
				for(int j=0;j<DIMOP;j++){
					mvaddch((i*4)+1+STARTROWOP,STARTCOLOP+j,opzioni[i][j]);
				}
				mvprintw((i*4)+2+STARTROWOP,STARTCOLOP,"---------------------");
				mvaddch((i*4)+1+STARTROWOP,STARTCOLOP-2,' ');
				attroff(COLOR_PAIR(SFONDO_COL));
			}
			return;
}

void boxMenu(int start_row,int start_col,int end_row,int end_col){
	for(int row=start_row;row<end_row+1;row++){
		for(int col=start_col;col<end_col+1;col++){
			if(row==start_row||row==end_row){
				attron(COLOR_PAIR(MENUBOX_COL));
				mvaddch(row, col,'.');
				attroff(COLOR_PAIR(MENUBOX_COL));
			}
			if(col==start_col||col==end_col||col==start_col+1||col==end_col-1){
				attron(COLOR_PAIR(MENUBOX_COL));
				mvaddch(row, col,'.');
				attroff(COLOR_PAIR(MENUBOX_COL));
			}
		}
	}

}
