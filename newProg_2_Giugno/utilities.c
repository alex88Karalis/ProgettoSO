
#include "utilities.h"

#define SEMAPHORE 

void scriviSuBuffer(Params *p, PipeData new_item, ThreadControlBlock *my_tcb, bool terminated_value){
	struct Semaphore *semafori = p->semafori;
    /*	sez. di ingresso (nella sez.critica)	*/
	#if defined(SEMAPHORE)
    sem_wait( &semafori->disponibili);
    sem_wait( &semafori->mutex);
    #endif // SEMAPHORE
    
	/* 	sez. critica 	-	scrittura sul buffer */
    *(p->buffer) = new_item;
    
	/*	sez. di uscita (dalla sez.critica)	*/
    #if defined(SEMAPHORE)
	sem_post( &semafori->mutex);
	sem_post( &semafori->presenti);
    #endif // SEMAPHORE
    
    // scrive sul tcb --- si può mettere in funzione separata 
    
    sem_wait(&semafori->tcb_mutex);
    ThreadControlBlock new_tcb = *(my_tcb);
    
	new_tcb.is_terminated = terminated_value;		// il threads segna se ha terminato
    *(my_tcb) = new_tcb;
    sem_post(&semafori->tcb_mutex);

	/**/
}

void leggiDaBuffer(Params *p, PipeData *readed){
    struct Semaphore *semafori = p->semafori;
	/*	sez. di ingresso (nella sez.critica)	*/
    #if defined(SEMAPHORE)
    sem_wait( &semafori->presenti);
    sem_wait( &semafori->mutex);
    #endif
    /* sez. critica 	-	legge/rimuove elemento dal buffer */
    
    *(readed) = *(p->buffer);

    /*	sez. di uscita (dalla sez.critica)	*/
    #if defined(SEMAPHORE)
    sem_post( &semafori->mutex);
    sem_post( &semafori->disponibili);
    #endif
}


/*
ThreadControlBlock getTCB(Params* p, pthread_t tid, TipoObj tipo_obj){
    struct Semaphore *semafori = p->semafori;
    AllTCB *allTCB = p->gameData->allTCB;
    ThreadControlBlock item;
    //switch

    sem_wait(&semafori->tcb_mutex);
    
    sem_post(&semafori->tcb_mutex);
}

/**/

void inizializzaNcurses() {
    srand(time(NULL));
    initscr(); // Inizializza ncurses
    curs_set(FALSE); // Nasconde il cursore
	noecho();
    nodelay(stdscr, TRUE); // Abilita l'input non bloccante
    cbreak();
    wtimeout(stdscr,0);
    keypad(stdscr, TRUE); // Abilita il keypad mode
    inizializzaColorazione();
    return;
}

void inizializzaColorazione(){
	start_color();
    init_pair(RANA_COL, COLOR_GREEN, COLOR_BLACK); // colore rana
    init_pair(SFONDO_COL,COLOR_WHITE,COLOR_BLACK); // colore sfondo
    init_pair(FIUME_COL,COLOR_WHITE,COLOR_BLUE); // colore fiume 
    init_pair(TANE_COL,COLOR_WHITE,COLOR_YELLOW); // colore tane
    init_pair(ARGINE_COL,COLOR_BLACK,COLOR_GREEN); // colore argine
    init_pair(MARCIAPIEDE_COL,COLOR_BLACK,COLOR_GRAY); // colore marciapiede
    init_pair(MENUBOX_COL,COLOR_GREEN,COLOR_BLACK); // colore box del menu
    init_pair(PROIETTILE_COL,COLOR_WHITE,COLOR_BLACK); // colore proiettile
    init_pair(PIANTA_COL,COLOR_BLACK, COLOR_GREEN); // colore nemico-pianta
    init_pair(BORDER_COL, COLOR_GREEN, COLOR_BLACK); // colore del bordo del gioco
    init_pair(LAVA_COL, COLOR_BLACK,COLOR_RED); // colore lava
    init_pair(LVL_COL, COLOR_BLACK,COLOR_RED); // colore hud livello
    init_pair(TEMPO_COL, COLOR_GREEN, COLOR_GREEN); // colore hud tempo
    return;
}

/*  ----    FUNZ PER SEMAFORI   ----    */
//void creaSemafori(sem_t *mutex, sem_t *presenti, sem_t *disponibili){
void creaSemafori(struct Semaphore* allSem){
    if (sem_init(&allSem->window_mutex,0,1) != 0){ perror("Inizializzazione Semaforo window_mutex fallita"); exit(-1); }

    if (sem_init(&allSem->mutex,0,1) != 0){ perror("Inizializzazione Semaforo mutex fallita"); exit(-1); }
    
    if (sem_init(&allSem->presenti,0,0) != 0){ perror("Inizializzazione Semaforo presenti fallita"); exit(-1);}
    
    if (sem_init(&allSem->disponibili,0,1) != 0) { perror("Inizializzazione Semaforo disponibili fallita"); exit(-1); }

    if (sem_init(&allSem->tcb_mutex,0,1) != 0) { perror("Inizializzazione Semaforo tcb_mutex fallita"); exit(-1); }

}

void distruggiSemafori(struct Semaphore* allSem){
    if (sem_destroy(&allSem->window_mutex) != 0){ perror("Distruzione Semaforo window_mutex fallita"); }

    if (sem_destroy(&allSem->mutex) != 0){ perror("Distruzione Semaforo mutex fallita"); }
    
    if (sem_destroy(&allSem->presenti) != 0){ perror("Distruzione Semaforo presenti fallita");  }
    
    if (sem_destroy(&allSem->disponibili) != 0) { perror("Distruzione Semaforo disponibili fallita"); }

    if (sem_destroy(&allSem->tcb_mutex) != 0) { perror("Inizializzazione Semaforo tcb_mutex fallita"); exit(-1); }
}


/*    ----      FUNZ PER TCB        ----        */

bool isThreadValid(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    bool value = false;
    
    sem_wait(semaforo);
    if(thread_tcb != NULL){
        value = true;
    }else{
        //perror("TCB NULLO");
    }
    sem_post(semaforo);
    return value;
}

bool isThreadTerminated(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    bool value = false;
    sem_wait(semaforo);
    value = thread_tcb->is_terminated;
    sem_post(semaforo); 
    return value;
}
bool isThreadTarget(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    bool value = false;
    sem_wait(semaforo);
    value = thread_tcb->is_target;
    sem_post(semaforo); 
    return value;
}


int resetTCB(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    ThreadControlBlock tcbStandard = {0,false,false};
    if(sem_wait(semaforo) != 0){ return -1;};
    *(thread_tcb) = tcbStandard;
    if(sem_post(semaforo)!=0){return -1;}
    return 0;
}

int impostaThreadTarget(ThreadControlBlock* thread_tcb,sem_t *semaforo){
    if(sem_wait(semaforo)!= 0){ return -1;}
    thread_tcb->is_target = true;
    if(sem_post(semaforo)!= 0){ return -1;}

    return 0;
}

int copiaTCB(ThreadControlBlock* tcb_dest, ThreadControlBlock tcb_source, sem_t* semaforo){
    
    if(sem_wait(semaforo)){return -1;}
    *(tcb_dest) = tcb_source;
    if(sem_post(semaforo)){return -1;}
    return 0;
}

int scriviSuTCB(Params *param, ThreadControlBlock* dst_tcb, ThreadControlBlock new_item){
    
    if(sem_wait(&param->semafori->tcb_mutex)!= 0){ return -1;}
    *(dst_tcb) = new_item;
    if(sem_post(&param->semafori->tcb_mutex) != 0){return -1;}
    return 0;
}

pthread_t leggiTid(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    pthread_t tid = 0;
    sem_wait(semaforo);
    tid = thread_tcb->thread_id;
    sem_post(semaforo); 
    return tid;
}

ThreadControlBlock leggiTCB(ThreadControlBlock* thread_tcb, sem_t* semaforo){
    ThreadControlBlock letto;
    sem_wait(semaforo);
    letto = *thread_tcb;
    sem_post(semaforo);
    return letto;
}

;
int joinThreadMorto(ThreadControlBlock* deadThread, sem_t* semaforo){
    
    if(isThreadValid(deadThread, semaforo)){
    }else{ }//return -1;}
    
    if(isThreadTerminated(deadThread, semaforo)){
        //perror("TCB Terminato");
        if(pthread_join(deadThread->thread_id, NULL) == 0 ){    // la join è riuscita
            resetTCB(deadThread, semaforo);
            return 0;
        }else{
            perror("Join Thread Fallita");
        }
        return 0;
        //beep();
    }
    
    /**/
    return -1; // return pthread_t thread_id;
}

int pulisciThreadMorti(struct Semaphore* allSem,  AllTCB* vettoreAllTCB){
    
    // NOTA: Tutte le funzioni interne usano semaforo mutex
    
    ThreadControlBlock localTCB;
    ThreadControlBlock *ptrTCB = NULL;

    /*  RANA    */
    //sem_wait(&allSem->tcb_mutex);
    //if(ptrTCB->is_target){perror("la RANA è TARGET ");}     //per DEBUG
    //if(ptrTCB->is_terminated){perror("la RANA è MORTA ");}
    
    /*
    bool flag = false;
    ptrTCB = vettoreAllTCB->tcb_rana;
    if(isThreadTarget(ptrTCB, &allSem->tcb_mutex)){ }
        if(joinThreadMorto(ptrTCB, &allSem->tcb_mutex) == 0){       // la RANA è terminata
            beep();
            return 0;
        }else{
            //beep();
            //perror("Rana Join Missed!");
        }
    /**/
    
    /*  PROIETTILI  */
    
    for(int i=0; i<MAXNPROIETTILI; i++){                // scorre il vettore di tcb_proiettili
        ptrTCB = &(vettoreAllTCB->tcb_proiettili[i]);   // seleziona il tcb del singolo proiettile 
        if(joinThreadMorto(ptrTCB, &allSem->tcb_mutex) == 0){           // la join è riuscita
                // tutto ok
            return 1;  // OR return tcb
        };
    }
    /**/


    

    
    return -1;
}

/* ------- pulitura thread morti di prova ---------------- */

/** Cancella dalla matrice uno specifico oggetto passato al parametro nemico
 * @param gameData riferimento a tutti i dati di gioco
 * @param oggetto è il riferimento al nemico da cancellare dalla matrice
 * @param old_pos vettore delle posizioni di tutti gli oggetti
 * @param tiposprite il tipo della sprite
 */
void cancellaOggettoDaMatrice(GameData *gameData, PipeData *oggetto ,PipeData *old_pos, TipoSprite tipoSprite)
{
	//int id = gameData->pipeData.id;
	
    int id = oggetto->id;

	PipeData *datiVecchi = &(old_pos[id]); // dati al passo precedente
	
	if (id >= 0) // se l'id è un indice di array valido
	{
		pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), gameData);
		datiVecchi->x = 0;
		datiVecchi->y = 0;
		datiVecchi->type = ' ';
		datiVecchi->id = id;
		datiVecchi->thread_id = 0;
	}
	return;
}


/**
 * @param deadThread Il TCB del thread da controllare
 * @param semaforo  Il semaforo per sincronizzare operazioni sui TCB
 * @return 0 se tutto OK, 1 in caso di errore 
 */
int joinThreadMorto_2(ThreadControlBlock* deadThread, sem_t* semaforo){
    if(isThreadTarget(deadThread,semaforo)){
        if(isThreadTerminated(deadThread, semaforo)){
            pthread_join(deadThread->thread_id,NULL);
            resetTCB(deadThread,semaforo);
            return 0;
        }
    }
    return 1;
}

int pulisciThreadMorti_2( GameData* gameData , struct Semaphore* allSem) // solo RANA
{
    /* chiama la joinThreadMorto_2 per ogni thread + aggiorna/resetta pids */
    ThreadControlBlock *tcb_thread = NULL;
    sem_t *semaforo_tcb = &(allSem->tcb_mutex); 
    int err ;
    
    /* RANA */
    tcb_thread = gameData->allTCB->tcb_rana;
    err = joinThreadMorto_2(tcb_thread,semaforo_tcb);
    if (!err){
        gameData->pids.pidRana = 0;
        return 0;
    }

    /* PROIETTILI RANA  */
    
    ThreadControlBlock *tcb_proiettili = gameData->allTCB->tcb_proiettili;
    for(int i=0; i< MAXNPROIETTILI; i++){
        tcb_thread = &(tcb_proiettili[i]);
        if(joinThreadMorto_2(tcb_thread, semaforo_tcb) == 0){
            gameData->pids.pidProiettili[i]=0;
            gameData->contatori.contProiettili = (gameData->contatori.contProiettili-1)%MAXNPROIETTILI;
        }
    }
    /**/

    /*  PROIETILE NEMICO    */
    ThreadControlBlock *tcb_proiettili_nemici = gameData->allTCB->tcb_proiettili_nemici;
    for(int i=0; i<MAXNPROIETTILINEMICI; i++){
        tcb_thread = &(tcb_proiettili_nemici[i]);
        if(joinThreadMorto_2(tcb_thread, semaforo_tcb) == 0){
            gameData->pids.pidProiettiliNemici[i]=0;  // ??
            gameData->contatori.contProiettiliN = (gameData->contatori.contProiettiliN-1)%MAXNPROIETTILINEMICI;
        }
    }


    /*  NEMICO_PIANTA    */
    ThreadControlBlock *tcb_nemici = gameData->allTCB->tcb_piante;
    for(int i=0; i<MAXNNEMICI; i++){
        tcb_thread = &(tcb_nemici[i]);
        if(joinThreadMorto_2(tcb_thread, semaforo_tcb)==0){
            //PipeData *nemico = &(gameData->oldPos.nemici[i]); // sarà il nemico giusto ??
            //cancellaOggettoDaMatrice(gameData, nemico, gameData->oldPos.nemici, S_PIANTA);
            gameData->pids.pidNemici[i] = 0;
            gameData->contatori.contNemici = (gameData->contatori.contNemici-1)%MAXNNEMICI;
            //beep();
        }
    }


    return -1;
}






// Cercun TCB in array per tid, ritorna TCB* o NULL
ThreadControlBlock* cercaThreadByTid(ThreadControlBlock* array_tcb, pthread_t thread_id, sem_t* semaforo, int dimVettore){
    
    ThreadControlBlock *ptrTCB = NULL;
    bool found=false;
    for (int i = 0; i < dimVettore; i++)    // scorre il vettore
    {
        sem_wait(semaforo);
        if(array_tcb[i].thread_id == thread_id){    // confronta i thread_id
            ptrTCB = &array_tcb[i];                 // salva l'indirizzo 
            found = true;
        }
        sem_post(semaforo);
        if(found) break;            // se trova qualcosa ferma il ciclo
    }
    return ptrTCB;
}



ThreadControlBlock* recuperaThreadTCB(ThreadControlBlock* array_tcb, pthread_t thread_id, int dimVettore){
    ThreadControlBlock* thread_tcb = NULL;
    for(int i=0; i<dimVettore; i++)
    {
        if(array_tcb[i].thread_id == thread_id){
            thread_tcb = &(array_tcb[i]);
        }
    }
    return thread_tcb;
}

/*
int impostaThreadTarget(ThreadControlBlock* array_tcb, pthread_t target_tid, int dimVettore){
	//cerco il tcb del thread che ha sforato lo schermo
    ThreadControlBlock* thisThread = recuperaThreadTCB( array_tcb,  target_tid, dimVettore);
    if(thisThread!=NULL){                   // se ho trovato il mio TCB
        thisThread->is_target = true;       // imposto il thread per la terminazione
        if(thisThread->is_target)           // controllo
            return 0;
    }else{
        return -2;
    }

    return -1;      // se qualcosa non va come dovrebbe
}
/**/




void creaPipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }
}

void uccidiProcesso(pid_t target_pid){
    kill(target_pid, SIGKILL);
	waitpid(target_pid,NULL,0);
    return;
}

void targetUccidiProcesso( pid_t *array_pid, int id_processo){
	if((id_processo != -1) && (array_pid[id_processo] != 0))
	{ 
		uccidiProcesso(array_pid[id_processo]);
		array_pid[id_processo]=0;
	}
}

int id_disponibile(pthread_t *array_pid, int lunghezza){
	for(int i=0;i<lunghezza;i++){
		if(array_pid[i]==0)return i;
	}
	return -1;
}

void aggiornaOldPos(PipeData *old_pos,PipeData *pipeData){
    /*
    old_pos->x=pipeData->x;
    old_pos->y=pipeData->y;
    old_pos->type=pipeData->type;
    old_pos->id=pipeData->id;
    old_pos->thread_id=pipeData->thread_id;
    /**/

    *(old_pos) = *(pipeData);

    return;
}

bool isGameOver(GameData* gameData){
    return gameData->gameInfo.vite==0 || gameData->gameInfo.manche==3;
}

bool isWin(GameData* gameData){
    return gameData->gameInfo.manche==3;
}

bool isFrogMoveLecit(int newX, int newY){
	return newX <= LASTGAMECOL - RANA_W + 1 && newX >= FIRSTGAMECOL && newY>FIRSTGAMEROW + RANA_H && newY < LASTGAMEROW;
}

void stampaBox(int option){
    int delay=0;
    if(option==1){delay=SUPERFAST;}

    attron(COLOR_PAIR(BORDER_COL));
    // stampa il bordo superiore
    for(int col = BORDERCOLSX; col <= BORDERCOLDX; col++){
        mvaddch(BORDERROWSUP,col,'='); // parte superiore
        refresh();
        usleep(delay);
    }
    // stampa il bordo destro
    for(int row = BORDERROWSUP + 1; row<= BORDERROWINF - 1; row++){
        mvaddch(row,BORDERCOLDX,'|'); // parte destra
        refresh();
        usleep(delay);
    }
    // stampa il bordo inferiore da destra a sinistra
    for(int col = BORDERCOLDX; col >= BORDERCOLSX; col--){
        mvaddch(BORDERROWINF,col,'='); // parte superiore
        refresh();
        usleep(delay);
    }
    // stampa il bordo sinistro dal basso verso l'alto
    for(int row = BORDERROWINF - 1; row >= BORDERROWSUP + 1; row--){
        mvaddch(row,BORDERCOLSX,'|'); // parte destra
        refresh();
        usleep(delay);
    }
    attroff(COLOR_PAIR(BORDER_COL));
    return;
}