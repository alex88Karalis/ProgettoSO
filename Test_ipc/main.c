#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/wait.h>

#define WIDTH 30
#define HEIGHT 10

// Definizione della struttura dati per le coordinate (x, y) e per il tipo
struct PipeData {
    int x;
    int y;
    char type;
};

void drawProcess(int* pipe_fd);
void moveProcess(int* pipe_fd);

// #TODO# invece che stampare la X stampare la rana (correggere lo scrittore in modo che la rana non esca dai bordi)
// #TODO# stampare il campo da gioco(quindi implementare colori)
// #TODO# eliminare effetto pac-man, non inviare dati in pipe se lo spostamento non è permesso, 
//        controllo direttamente all'interno del processo scrittore
// #TODO# ideare un metodo per stampare solamente le effettive modifiche e non tutta la matrice ogni volta,
//        cosi non si deve pulire interamente lo schermo ogni volta
//        1. possibile approccio: struttura dati che tenga le celle della matrice sporcata
//        2. un altro approccio sarebbe inserire un flag all'interno delle celle della matrice ma facendo cosi occorrerebbe scorrere
//           l'intera matrice comunque
// #TODO# dividere il codice in più file uno per il disegnatore ed uno per lo scrittore
// #TODO# scrivere il processo proiettile che deve essere fatto partire da chi?
//        se lo fa partire il disegnatore allora devo tenere la pipe in scrittura aperta all'interno cosi da poterla passare
//        il disegnatore fa la fork per il proiettile una volta che riceve in pipe il carattere spazio dal processo disegnatore

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

        // Aspetta un po' prima di generare nuove coordinate
        usleep(100000);
    }
}


void drawProcess(int* pipe_fd) {
		
		// struttura per leggere la pipe
    struct PipeData pipeData;
    
    // struttura ausiliaria per le coordinate al passo precedente
    struct PipeData pipeData_old;
    // inizializzata a valori inverosimili
    pipeData_old.x=-1;
    pipeData_old.y=-1;
    pipeData_old.type=' ';
    
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
        
        

        // Pulisci la finestra
        clear();
        
        // disegna l'intera matrice
        for(int i=0;i<HEIGHT;i++){
					for(int j=0;j<WIDTH;j++){
						mvaddch(i, j, screenMatrix[i][j]);
					}
				}

        // Aggiorna la finestra
        refresh();
    }

    endwin(); // Termina ncurses
}
