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
/*-----------------------------------------------------------------------------------------------------------------------------------------------*/
// #1TODO# invece che stampare la X stampare la rana:
//         (correggere lo scrittore in modo che la rana non esca dai bordi)
//      OK serve una funzione che stampi lo script della rana void printFrog()

// #2TODO# stampare il campo da gioco(quindi implementare colori)

// #3TODO# eliminare effetto pac-man, non inviare dati in pipe se lo spostamento non è permesso, 
//        controllo direttamente all'interno del processo scrittore

// #4TODO# ideare un metodo per stampare solamente le effettive modifiche e non tutta la matrice ogni volta,
//        cosi non si deve pulire interamente lo schermo ogni volta
//        1. possibile approccio: struttura dati che tenga le celle della matrice sporcata
//        2. un altro approccio sarebbe inserire un flag all'interno delle celle della matrice ma facendo cosi occorrerebbe scorrere
//           l'intera matrice comunque

// #5TODO# dividere il codice in più file uno per il disegnatore ed uno per lo scrittore

// #6TODO# scrivere il processo proiettile che deve essere fatto partire da chi?
//        se lo fa partire il disegnatore allora devo tenere la pipe in scrittura aperta all'interno cosi da poterla passare
//        il disegnatore fa la fork per il proiettile una volta che riceve in pipe il carattere spazio dal processo disegnatore

// #7TODO# schermata iniziale di gioco: all'avvio del programma appare a schermo magari con un animazione la schermata iniziale
//        con il nome del gioco in grande, sotto il nome del gioco una scritta di benvenuto e l'invito a premere un tasto per contiunare...
//        il programma principale lancia una fork ed esegue il programma schermataIniziale() ed aspetta che termini per proseguire

// #8TODO# generalità sui menu, il menu deve apparire al centro dello schermo racchiuso dentro una box(), in alto il logo del gioco,
//        seguito dalle opzioni, le opzione possono essere navigabili attraverso i tasti freccia su e giu e l'opzione preselezionata deve essere
//        evidenzionata colorando lo sfondo
//        ogni opzione è alta 3 con una riga vuota seguita da una riga in cui in modo centrato è scritta l'opzione seguita a sua volta da una riga
//        vuota tutte e 3 le righe sono colorate se preselezionate

// #9TODO# munu iniziale: dopo la schermata iniziale si fa partire un altro programma menuIniziale() con le opzioni continua(solo se ci sono    
//        salvataggi, in quel caso si carica quello più recente), una opzione nuovapartita, una opzione carica partita, una opzione setting ed una
//        opzione esci.
//        il processo principale aspetta che il processo figlio menuprincipale termini e restituisca un intero che indica la scelta dell'utente

// #10TODO# legenda tasti + breve introduzione al gioco: se nel menu iniziale si è scleta l'opzione nuova partita il programma principale si
//          occuperà di lanciare il programma creazioneNuovaPartita() a cui viene passata una struttura dati come puntatore,
//          la struttura dati contine il nome del giocatore (max sei caratteri), il nome della partita (max 20 caratteri)
//          il programma si occupa di richiedere i dati all'utente controllandone la validità e scrive sulla struttura passata attravero 
//          il puntatore inoltre legge il file di testo settings in cui sono scritti i settaggi ed scrive nella struttura la difficoltà 
//          di gioco dopo l'inserimento dei dati il programma stampa una legenda dei tasti e fa una breve descrizione del gioco e di come giocare

// #11TODO# carica partita: se nel menu iniziale si preme carica partita (cosa che è possibile fare solamente se ci sono partite salvate)
//        il programma principale lancia il programma sceltaPartita() che restituira un intero identificativo del salvataggio scelto
//        i dati delle partite salvate devono essere letti dal file partiteSalvate.txt 

// #12TODO# menu settings: se premo il tasto settings nel menu iniziale si avvia il processo settings() che stampa a video un menù con le opzioni
//          volume, difficoltà, volume on/off, volume, torna indietro
//          la preselezione funziona come per i normali menu ma una volta che si clicca su un opzione questa è modificabile (attraverso i
//          tasti sx,dx si scorrono le opzioni)
//          una volta premuto il tasto torna indietro si leggono i valori inseriti e si sovrascrive il file settings

// #13TODO# animazione rana: potrebbero muoversi le zampe?

// #14TODO# test suoni: come funzionano i suoni con ncurses? quanti suoni diversi sono possibili? soundtrack?

// #15TODO# tronchi: 
//          i tronchi sono tre uno per corsia min_x: 6 max_x: 9, direzione del flusso deve essere casuale, i tronchi rimbalzano e tornano
//          indietro

// #16TODO# automobili e camion

// #17TODO# nemici

// #18TODO# piu livelli

// #19TODO# gestione difficoltà

// #20TODO# salvataggi

/*-----------------------------------------------------------------------------------------------------------------------------------------------*/


int main() {
		
		initscr(); // Inizializza ncurses
    curs_set(FALSE); // Nasconde il cursore
    
    

		// Abilita l'input non bloccante
    nodelay(stdscr, TRUE);
    
    // Abilita il keypad mode
    keypad(stdscr, TRUE);		
		
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
        //close(pipe_fd[1]); // Chiudi l'estremità di scrittura della pipe
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
        //close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
        tronco(pipe_fd,7,0);
        exit(0);
    }
    /*
    // Crea il processo per il tronco 2
    pid_t tronco_pid2 = fork();
    if (tronco_pid2 < 0) {
        perror("Fork failed");
        return 1;
    } else if (tronco_pid2 == 0) {
        // Processo tronco 1
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
        // Processo tronco 1
        close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
        tronco(pipe_fd,13,2);
        exit(0);
    }
    /**/
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
		pipeData.id=0;
		
    while (1) {
        // Leggi il carattere dall'input
        int ch = getch();
        if (ch != ERR && ch != 'q') {
            
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
        }else{break;} // se ch non valido esce dal ciclo

        // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
        usleep(100000); // andrebbe tolto nel processo che prende input
    }//end while
    exit(0);
}//end moveProcess()


void drawProcess(int* pipe_fd) {

		start_color();
    init_pair(RANA, COLOR_GREEN, COLOR_BLACK); // Crea una combinazione di colori (colore rosso su sfondo nero)
    init_pair(TRONCHI,COLOR_YELLOW,COLOR_RED);
		
		// struttura per leggere la pipe
    struct PipeData pipeData;
    
    // struttura ausiliaria per le coordinate al passo precedente
    struct PipeData pipeData_old;
    // inizializzata a valori inverosimili
    pipeData_old.x=-1;
    pipeData_old.y=-1;
    pipeData_old.type=' ';
    pipeData_old.id=0;
    
    // struttura ausiliaria per le coordinate al passo precedente del tronco1
    struct PipeData troncoData_old;
    // inizializzata a valori inverosimili
    troncoData_old.x=-1;
    troncoData_old.y=-1;
    troncoData_old.type=' ';
    troncoData_old.id=0;
    
    /*
    // struttura ausiliaria per le coordinate al passo precedente del tronco1
    struct PipeData troncoData_old2;
    // inizializzata a valori inverosimili
    troncoData_old2.x=-1;
    troncoData_old2.y=-1;
    troncoData_old2.type=' ';
    troncoData_old2.id=1;
    
    /*
    // struttura ausiliaria per le coordinate al passo precedente del tronco1
    struct PipeData troncoData_old3;
    // inizializzata a valori inverosimili
    troncoData_old3.x=-1;
    troncoData_old3.y=-1;
    troncoData_old3.type=' ';
    troncoData_old3.id=2;
    
    /**/
    
    
    // inizializzazione matrice che rappresenta lo schermo
		char screenMatrix[HEIGHT][WIDTH];
		
		for(int i=0;i<HEIGHT;i++){
			for(int j=0;j<WIDTH;j++){
				screenMatrix[i][j]=' ';
			}
		}

    while (1) {
    		
        // Leggi le coordinate inviate dalla pipe
        close(pipe_fd[1]); // Chiudi l'estremità di scrittura della pipe
        read(pipe_fd[0], &pipeData, sizeof(struct PipeData));
        
        mvprintw(30,5,"problema se non visualizzi questo");
        
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
        			// se pipedata è diverso da troncodata_old allora le coordinate sono cambiate 
        			// devo cancellare la T nella posizione vecchia e scriverla nella posizione nuova all'interno della matrice
        			if(pipeData.x!=troncoData_old.x || pipeData.y!=troncoData_old.y){
        	
		      			// sovrascrivo posizione vecchia
		      			for(int i=troncoData_old.y;i<troncoData_old.y+2;i++){
		      				for(int j=troncoData_old.x;j<troncoData_old.x+9;j++){
		      					screenMatrix[i][j]=' ';
		      				}
		      			}
		      		
		      	
		      			// scrivo posizione nuova
		      			//screenMatrix[pipeData.y][pipeData.x]=pipeData.type;
		      			for(int i=pipeData.y;i<pipeData.y+2;i++){
		      				for(int j=pipeData.x;j<pipeData.x+9;j++){
		      					screenMatrix[i][j]='T';
		      				}
		      			}
		      		
		      			//disegnaTronco(pipeData.y,pipeData.x);
		      	
		      			// aggiorno coords_old
		      			troncoData_old.x=pipeData.x;
		      			troncoData_old.y=pipeData.y;
        			}// end if case0
        			break;
      			/*
        		case 1:
        			// se pipedata è diverso da troncodata_old allora le coordinate sono cambiate 
        			// devo cancellare la T nella posizione vecchia e scriverla nella posizione nuova all'interno della matrice
        			if(pipeData.x!=troncoData_old2.x || pipeData.y!=troncoData_old2.y){
        	
        				// sovrascrivo posizione vecchia
        				for(int i=troncoData_old2.y;i<troncoData_old2.y+2;i++){
        					for(int j=troncoData_old2.x;j<troncoData_old2.x+8;j++){
        						screenMatrix[i][j]=' ';
        					}
        				}
        		
        	
        				// scrivo posizione nuova
        				//screenMatrix[pipeData.y][pipeData.x]=pipeData.type;
        				for(int i=pipeData.y;i<pipeData.y+2;i++){
        					for(int j=pipeData.x;j<pipeData.x+8;j++){
        						screenMatrix[i][j]='T';
        					}
        				}
        		
        				//disegnaTronco(pipeData.y,pipeData.x);
        	
        				// aggiorno coords_old
        				troncoData_old2.x=pipeData.x;
        				troncoData_old2.y=pipeData.y;
        			}	//end if case1
        			break;
        			
        			/*
        		case 2:
        			// se pipedata è diverso da troncodata_old allora le coordinate sono cambiate 
        			// devo cancellare la T nella posizione vecchia e scriverla nella posizione nuova all'interno della matrice
        			if(pipeData.x!=troncoData_old3.x || pipeData.y!=troncoData_old3.y){
        	
		      			// sovrascrivo posizione vecchia
		      			for(int i=troncoData_old3.y;i<troncoData_old3.y+2;i++){
		      				for(int j=troncoData_old3.x;j<troncoData_old3.x+8;j++){
		      					screenMatrix[i][j]=' ';
		      				}
		      			}
		      		
		      	
		      			// scrivo posizione nuova
		      			//screenMatrix[pipeData.y][pipeData.x]=pipeData.type;
		      			for(int i=pipeData.y;i<pipeData.y+2;i++){
		      				for(int j=pipeData.x;j<pipeData.x+8;j++){
		      					screenMatrix[i][j]='T';
		      				}
		      			}
		      		
		      			//disegnaTronco(pipeData.y,pipeData.x);
		      	
		      			// aggiorno coords_old
		      			troncoData_old3.x=pipeData.x;
		      			troncoData_old3.y=pipeData.y;
        			}	//end if case2
        			break;
        		default:
        			break;
        		/**/
      		}// chiusura switch
        	
        }// chiusura if type==T
 
        // Pulisci la finestra
        clear();
        
        // disegna l'intera matrice
        for(int i=0;i<HEIGHT;i++){
					for(int j=0;j<WIDTH;j++){
						mvaddch(i, j, screenMatrix[i][j]);
					}
				}
        // Aggiorna la finestra
        box(stdscr,ACS_VLINE, ACS_HLINE);
        refresh();
    } // chiusura while
    
		endwin(); // Termina ncurses
}	// end drawProcess()

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
    	
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe

      // Invia le coordinate attraverso la pipe
      write(pipe_fd[1], &pipeData, sizeof(struct PipeData));

      // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
      usleep(100000);
    }
}



