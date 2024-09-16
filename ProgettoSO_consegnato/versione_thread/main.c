#include "disegna.h"

int mortiRana=0;
//pthread_t tidRana=0;
GameData gD;
struct Semaphore mainSem;
AllTCB mainTCBtable; // Tabella con tutti i ThreadControlBlock
PipeData buffer;

int main() {
	int pipe_fd[2]; // Pipe file descriptors
	inizializzaNcurses(); // inizializzazione della libreria ncurses + inizializzazione seme per random
		
    creaPipe(pipe_fd);  // Crea la pipe
    
    //sem_t mutex;
    //sem_t presenti;
    //sem_t disponibili;
    
    creaSemafori(&mainSem);
    GameData* gameData = &gD;  //(GameData*)malloc(sizeof(GameData));

    //GameData* gameData = (GameData*)malloc(sizeof(GameData));
    gameData->errore=0;

    //ThreadControlBlock main_tcb_proiettili[MAXNPROIETTILI];
    ThreadControlBlock tcb_Rana = {0,false,false}; // dichiaro tcb rana

    mainTCBtable.tcb_rana = &tcb_Rana; // assegno indirizzo del tcbRana alla struct principale
    //mainTCBtable.tcb_proiettili = main_tcb_proiettili;
    gameData->allTCB = (AllTCB*)&mainTCBtable;
    
    
    Params draw_arg = {&mainSem, &buffer, gameData, 0, &mortiRana};// Parametri per drawThread

    //avviaLogo();   // visualizza il logo ed aspetta che l'utente prema un tasto qualsiasi
    //int scelta = avviaMenuIniziale(); // fa partire il processo per il menu iniziale, aspetta che termini e poi prosegue
    
    pthread_t drawTid = avviaDrawThread(&draw_arg);
    
    int err = pthread_join(drawTid, NULL); // attende chiusura del threadDisegna
    if(err){
        perror("ERR: Join su Disegna Fallita!!");
        strerror(err);
        return err;
    }else{
        printf("\nMain Thread Exit Normally");
    }

    // Chiudi le estremità della pipe
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    refresh();

    distruggiSemafori(&mainSem);
    //free(gameData);

    endwin(); // Termina ncurses
    printf("\n FROGGER: CLOSED \n\n");
    return 0;
}