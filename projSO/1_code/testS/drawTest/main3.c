#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/wait.h>

#define WIDTH 104
#define HEIGHT 37

#define RANA 1
#define TRONCHI 2

// Definizione della struttura dati per le coordinate (x, y) e per il tipo
struct PipeData {
    int x;
    int y;
    char type;
    int id;
};

typedef struct {
    char sprite[2][9]; // 2 righe - 8 char + terminatore di stringa 
} SpriteTronco;

void drawProcess(int* pipe_fd);
void moveProcess(int* pipe_fd);
void tronco(int* pipe_fd, int y,int id);
void printFrog(int x,int y);
void disegnaTronco(int row, int col);
void stampaTroncoInMatrice(int row, int col, char (*screenMatrix)[WIDTH]);
void pulisciTroncoInMatrice(int row, int col, char (*screenMatrix)[WIDTH]);
void stampaMatrice(char (*screenMatrix)[WIDTH]);

int main() {
		
		initscr(); // Inizializza ncurses
    curs_set(FALSE); // Nasconde il cursore
    nodelay(stdscr, TRUE); // Abilita l'input non bloccante
    keypad(stdscr, TRUE); // Abilita il keypad mode
		
    // Crea la pipe
    int pipe_fd[2]; // Pipe file descriptors
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    // Crea il processo per il disegno
    pid_t draw_pid = fork();
    if (draw_pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (draw_pid == 0) {
        // Processo "disegna"
        close(pipe_fd[1]); // Chiudi l'estremità di scrittura della pipe
        drawProcess(pipe_fd);
        exit(0);
    }

    // Crea il processo per il movimento
    pid_t move_pid = fork();
    if (move_pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (move_pid == 0) {
        // Processo "muovi"
        close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
        moveProcess(pipe_fd);
        exit(0);
    }
    
    // Crea il processo per il tronco 1
    pid_t tronco_pid = fork();
    if (tronco_pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (tronco_pid == 0) {
        // Processo tronco 1
        close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
        tronco(pipe_fd,7,0);
        exit(0);
    }
    
    // Crea il processo per il tronco 2
    pid_t tronco_pid2 = fork();
    if (tronco_pid2 < 0) {
        perror("Fork failed");
        return 1;
    } else if (tronco_pid2 == 0) {
        // Processo tronco 2
        close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
        tronco(pipe_fd,10,1);
        exit(0);
    }
    
    // Crea il processo per il tronco 3
    pid_t tronco_pid3 = fork();
    if (tronco_pid3 < 0) {
        perror("Fork failed");
        return 1;
    } else if (tronco_pid3 == 0) {
        // Processo tronco 3
        close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
        tronco(pipe_fd,13,2);
        exit(0);
    }
    
    // Chiudi le estremità della pipe nel processo padre
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    // Aspetta che i processi figlio terminino
    wait(NULL);
    wait(NULL);
    return 0;
}

void moveProcess(int* pipe_fd) {
   
		struct PipeData pipeData;
		pipeData.x=6;
		pipeData.y=6;
		pipeData.type='X';
		
    while (1) {
        // Leggi il carattere dall'input
        int ch = getch();
        if (ch != ERR) {
            
            // Muovi il personaggio in base all'input dell'utente
            switch (ch) {
                case KEY_UP:
                    pipeData.y = (pipeData.y - 1 + HEIGHT) % HEIGHT;
                    break;
                case KEY_DOWN:
                    pipeData.y = (pipeData.y + 1) % HEIGHT;
                    break;
                case KEY_LEFT:
                    pipeData.x = (pipeData.x - 1 + WIDTH) % WIDTH;
                    break;
                case KEY_RIGHT:
                    pipeData.x = (pipeData.x + 1) % WIDTH;
                    break;
            }

            // Invia le coordinate attraverso la pipe
            write(pipe_fd[1], &pipeData, sizeof(struct PipeData));
        }

        // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
        usleep(100000);
    }
}


void drawProcess(int* pipe_fd) {
		start_color();
    init_pair(RANA, COLOR_GREEN, COLOR_BLACK); // Crea una combinazione di colori (colore verde su sfondo nero)
    init_pair(TRONCHI,COLOR_YELLOW,COLOR_RED); // per la colorazione dei tronchi
		
		// struttura per leggere la pipe
    struct PipeData pipeData;
    
    // struttura ausiliaria per le coordinate al passo precedente
    struct PipeData pipeData_old;
    // inizializzata a valori inverosimili
    pipeData_old.x=-1;
    pipeData_old.y=-1;
    pipeData_old.type=' ';
    
    // struttura ausiliaria per le coordinate al passo precedente del tronco1
    struct PipeData troncoData_old;
    // inizializzata a valori inverosimili
    troncoData_old.x=-1;
    troncoData_old.y=-1;
    troncoData_old.type=' ';
    troncoData_old.id=0;
    
    // struttura ausiliaria per le coordinate al passo precedente del tronco1
    struct PipeData troncoData_old2;
    // inizializzata a valori inverosimili
    troncoData_old2.x=-1;
    troncoData_old2.y=-1;
    troncoData_old2.type=' ';
    troncoData_old2.id=1;
    
    // struttura ausiliaria per le coordinate al passo precedente del tronco1
    struct PipeData troncoData_old3;
    // inizializzata a valori inverosimili
    troncoData_old3.x=-1;
    troncoData_old3.y=-1;
    troncoData_old3.type=' ';
    troncoData_old3.id=2;
    
    // inizializzazione matrice che rappresenta lo schermo
		char screenMatrix[HEIGHT][WIDTH];
		
		for(int i=0;i<HEIGHT;i++){
			for(int j=0;j<WIDTH;j++){
				screenMatrix[i][j]=' ';
			}
		}

    while (1) {
        // Leggi le coordinate inviate dalla pipe
        read(pipe_fd[0], &pipeData, sizeof(struct PipeData));
        
        // se pipeData.type=='X' ovvero sono coordinate della rana
        if(pipeData.type=='X'){
        	// se pipedata è diverso da pipedata_old allora le coordinate sono cambiate 
        	// devo cancellare la X nella posizione vecchia e scriverla nella posizione nuova all'interno della matrice
        	if(pipeData.x!=pipeData_old.x || pipeData.y!=pipeData_old.y){
        	
        		// sovrascrivo posizione vecchia
        		screenMatrix[pipeData_old.y][pipeData_old.x]=' ';
        	
        		// scrivo posizione nuova
        		screenMatrix[pipeData.y][pipeData.x]=pipeData.type;
        		
        		// aggiorno coords_old
        		pipeData_old.x=pipeData.x;
        		pipeData_old.y=pipeData.y;
        	}
        }
        
        // se pipeData.type=='T' ovvero sono coordinate del tronco
        if(pipeData.type=='T'){
        	
        	switch(pipeData.id){
        		case 0:
        			// se pipedata è diverso da troncodata_old allora le coordinate sono cambiate pulisco la matrice e ristampo sulla matrice
        			if(pipeData.x!=troncoData_old.x || pipeData.y!=troncoData_old.y){
        				
        				pulisciTroncoInMatrice(troncoData_old.y,troncoData_old.x,screenMatrix);
        		
        				stampaTroncoInMatrice(pipeData.y,pipeData.x,screenMatrix);
        	
        				// aggiorno coords_old
        				troncoData_old.x=pipeData.x;
        				troncoData_old.y=pipeData.y;
        			}
        			break;
        		case 1:
        			// se pipedata è diverso da troncodata_old allora le coordinate sono cambiate pulisco la matrice e ristampo sulla matrice
        			if(pipeData.x!=troncoData_old2.x || pipeData.y!=troncoData_old2.y){
        				
        				pulisciTroncoInMatrice(troncoData_old2.y,troncoData_old2.x,screenMatrix);
        				
        				stampaTroncoInMatrice(pipeData.y,pipeData.x,screenMatrix);
        	
        				// aggiorno coords_old
        				troncoData_old2.x=pipeData.x;
        				troncoData_old2.y=pipeData.y;
        			}
        			break;
        		case 2:
        			// se pipedata è diverso da troncodata_old allora le coordinate sono cambiate pulisco la matrice e ristampo sulla matrice
        			if(pipeData.x!=troncoData_old3.x || pipeData.y!=troncoData_old3.y){
        	
		      			pulisciTroncoInMatrice(troncoData_old3.y,troncoData_old3.x,screenMatrix);
        				
        				stampaTroncoInMatrice(pipeData.y,pipeData.x,screenMatrix);
		      	
		      			// aggiorno coords_old
		      			troncoData_old3.x=pipeData.x;
		      			troncoData_old3.y=pipeData.y;
        			}
        			break;
        		default:
        			break;
        	
        		}// chiusura switch
        	
        	}// chiusura if type==T
        
        
        	clear(); // Pulisci la finestra di gioco
        
					stampaMatrice(screenMatrix); // stampa a video l'intera matrice

        	refresh(); // Aggiorna la finestra
        
        }
    
    		endwin(); // Termina ncurses
}

// ok
void printFrog(int x,int y){
	// lo script della rana
	char script[2][3]={{'/','.','\\'},{'\\','-','/'}};
	
	// ciclo che disegna la rana
	for(int i=0;i<2;i++){
		for(int j=0;j<3;j++){
    	attron(A_BOLD | COLOR_PAIR(RANA));
    	mvaddch(i,j,script[i][j]);
    	attroff(A_BOLD | COLOR_PAIR(1));
		}
	}
	return;
}

// ok
void disegnaTronco(int row, int col){
    SpriteTronco t = {{{"~ ~ ~ ~ "},
                                         {" ~ ~ ~ ~"}}};    // 2 righe x 8 char
    attron(COLOR_PAIR(TRONCHI));

    // ogni tronco occupa due righe, stampa riga per riga
    for(int tmp_row=0; tmp_row<2; tmp_row++){
        mvprintw(row+tmp_row, col, t.sprite[tmp_row]);
    }
    
    attroff(COLOR_PAIR(TRONCHI));
		return;
}

// ok
void tronco(int* pipe_fd, int y,int id) {
   
		struct PipeData pipeData;
		pipeData.x=2;
		pipeData.y=y;
		pipeData.type='T';
		pipeData.id=id;
				
		int lunghezza_tronco= 9;
		
		int direzione;
		srand(time(NULL));
		int direzione_casuale= rand()%2;
		if(direzione_casuale==1){
			direzione=1;
		}
		else{
			direzione=-1;
		}
		
    while (1) {
    	if(direzione==1){
    		if(pipeData.x + lunghezza_tronco + 1 < WIDTH){
      		pipeData.x++;
      	}
      	else{
      		direzione*=-1;
      	}
    	}else{
    		if(pipeData.x - 1 > 0){
    			pipeData.x--;
    		}
    		else{
    			direzione*=-1;
    		}
    	}
    	
      

      // Invia le coordinate attraverso la pipe
      write(pipe_fd[1], &pipeData, sizeof(struct PipeData));

      // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
      usleep(100000);
    }
}

// stampa un tronco nella matrice che rappresenta lo schermo
void stampaTroncoInMatrice(int row,int col, char (*screenMatrix)[WIDTH]){
	int max_row=2;
	int max_col=9;
        				
  for(int i=row;i<row+max_row;i++){
  	for(int j=col;j<col+max_col;j++){
    	screenMatrix[i][j]='T';
    }
  }
  return;
}

void pulisciTroncoInMatrice(int row, int col, char (*screenMatrix)[WIDTH]){
	int max_row=2;
	int max_col=9;
  if(row!=-1){
  	for(int i=row;i<row+max_row;i++){
  	for(int j=col;j<col+max_col;j++){
    	screenMatrix[i][j]=' ';
    }
  }
  }      				
  
  return;
}
void stampaMatrice(char (*screenMatrix)[WIDTH]){
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			mvaddch(i, j, screenMatrix[i][j]);
		}
	}
	return;
}

