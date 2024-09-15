
#include "utilities.h"

#define SEMAPHORE 

int generaRandom_r(int min, int max, unsigned int *seed) {
    return (rand_r(seed) % (max - min + 1)) + min;
}

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
    init_pair(ARGINE_COL,COLOR_WHITE,COLOR_GREEN); // colore argine
    init_pair(MARCIAPIEDE_COL,COLOR_BLACK,COLOR_GRAY); // colore marciapiede
    init_pair(MENUBOX_COL,COLOR_GREEN,COLOR_BLACK); // colore box del menu
    init_pair(PROIETTILE_COL,COLOR_WHITE,COLOR_BLACK); // colore proiettile
    init_pair(BORDER_COL, COLOR_GREEN, COLOR_BLACK); // colore del bordo del gioco
    init_pair(LAVA_COL, COLOR_BLACK,COLOR_RED); // colore lava
    init_pair(LVL_COL, COLOR_BLACK,COLOR_RED); // colore hud livello
    init_pair(TEMPO_COL, COLOR_GREEN, COLOR_GREEN); // colore hud tempo
    init_pair(PIANTA_COL,COLOR_BLACK, COLOR_GREEN); // colore nemico-pianta
    init_pair(COCCODRILLOBUONO_COL, COLOR_GREEN, COLOR_WHITE); // colore coccodrillo
    init_pair(COCCODRILLOCATTIVO_COL,COLOR_RED,COLOR_BLACK); // colore coccodrillo cattivo
    init_pair(LAMPEGGIA,COLOR_GREEN,COLOR_WHITE);  // colore coccodrillo che lampeggia
    
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


/*    ----      FUNZIONI PER TCB        ----        */

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


/* ------- pulitura thread morti ---------------- */

/** Cancella dalla matrice la sprite di uno specifico oggetto passato al parametro oggetto,
 *  e resetta le posizioni salvate in old_pos.
 * @param gameData riferimento a tutti i dati di gioco
 * @param oggetto è il riferimento al oggetto da cancellare dalla matrice
 * @param old_pos vettore delle posizioni salvate di tutti gli oggetti
 * @param tiposprite il tipo della sprite
 */
void cancellaOggettoDaMatrice(GameData *gameData, PipeData *oggetto ,PipeData *old_pos, TipoSprite tipoSprite)
{
	
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
int joinThreadMorto(ThreadControlBlock* deadThread, sem_t* semaforo){
    if(isThreadTarget(deadThread,semaforo)){
        if(isThreadTerminated(deadThread, semaforo)){
            pthread_join(deadThread->thread_id,NULL);
            resetTCB(deadThread,semaforo);
            return 0;
        }
    }
    return 1;
}

int pulisciThreadMorti( GameData* gameData , struct Semaphore* allSem)
{
    /* chiama la joinThreadMorto per ogni thread
        - chiama la join sul thread
        - aggiorna/resetta pids 
        - aggiorna contatore 
        - cancella sprite dalla matrice
    */

    ThreadControlBlock *tcb_thread = NULL;
    sem_t *semaforo_tcb = &(allSem->tcb_mutex); 
    int err ;
    
    /* RANA */
    tcb_thread = gameData->allTCB->tcb_rana;
    err = joinThreadMorto(tcb_thread,semaforo_tcb);
    if (!err){
        gameData->pids.pidRana = 0;
        PipeData *rana = &(gameData->oldPos.rana);
        cancellaOggettoDaMatrice(gameData, rana, rana, S_RANA);
        return 0;
    }

    /* PROIETTILI RANA  */
    
    ThreadControlBlock *tcb_proiettili = gameData->allTCB->tcb_proiettili;
    for(int i=0; i< MAXNPROIETTILI; i++){
        tcb_thread = &(tcb_proiettili[i]);
        if(joinThreadMorto(tcb_thread, semaforo_tcb) == 0){
            gameData->pids.pidProiettili[i]=0;
            gameData->contatori.contProiettili = (gameData->contatori.contProiettili-1)%MAXNPROIETTILI;
            PipeData *proiettile = &(gameData->oldPos.proiettili[i]);
            cancellaOggettoDaMatrice(gameData,proiettile,gameData->oldPos.proiettili,S_PROIETTILE);
        }
    }
    /**/

    /*  PROIETILE NEMICO    */
    ThreadControlBlock *tcb_proiettili_nemici = gameData->allTCB->tcb_proiettili_nemici;
    for(int i=0; i<MAXNPROIETTILINEMICI; i++){
        tcb_thread = &(tcb_proiettili_nemici[i]);
        if(joinThreadMorto(tcb_thread, semaforo_tcb) == 0){
            gameData->pids.pidProiettiliNemici[i]=0;  // ??
            gameData->contatori.contProiettiliN = (gameData->contatori.contProiettiliN-1)%MAXNPROIETTILINEMICI;
            PipeData *proiettileNemico = &(gameData->oldPos.proiettiliNemici[i]);
            cancellaOggettoDaMatrice(gameData, proiettileNemico, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
        }
    }


    /*  NEMICO_PIANTA    */
    ThreadControlBlock *tcb_nemici = gameData->allTCB->tcb_piante;
    for(int i=0; i<MAXNNEMICI; i++){
        tcb_thread = &(tcb_nemici[i]);
        if(joinThreadMorto(tcb_thread, semaforo_tcb)==0){
             gameData->pids.pidNemici[i]=0;  // ??
             gameData->contatori.contNemici = (gameData->contatori.contNemici-1)%MAXNNEMICI;
             PipeData *nemico = &(gameData->oldPos.nemici[i]);
             cancellaOggettoDaMatrice(gameData, nemico, gameData->oldPos.nemici, S_PIANTA);
        }
    }
    
    /*  COCCODRILLO */
    ThreadControlBlock *tcb_coccodrilli = gameData->allTCB->tcb_coccodrilli;
    for(int i=0; i<MAXNCOCCODRILLI; i++){
        tcb_thread = &(tcb_coccodrilli[i]);
        if(joinThreadMorto(tcb_thread, semaforo_tcb)==0){
            gameData->pids.pidCoccodrilli[i]=0;  // ??
            
            PipeData *coccodrillo = &(gameData->oldPos.coccodrilli[i]);
            switch (coccodrillo->type)
            {
            case 'C':
            /* code */
                cancellaOggettoDaMatrice(gameData, coccodrillo, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C);
                break;
            case 'c':
            /* code */
                cancellaOggettoDaMatrice(gameData, coccodrillo, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C);
                break;
            default:
                break;
            }
            gameData->contatori.contCoccodrilli = (gameData->contatori.contCoccodrilli -1)%MAXNCOCCODRILLI;
            gameData->flussi[gameData->controlloCoccodrilli[gameData->oldPos.coccodrilli[i].id].fila].n_coccodrilli_attivi--;

        }
    }


    /*  TEMPO   */
    ThreadControlBlock * tcb_tempo = &(gameData->allTCB->tcb_tempo);
    if(joinThreadMorto(tcb_tempo, semaforo_tcb)==0){
        
        //avviaTempoThread();   come resettare la manche ??

        //beep();
    }



    return -1;
}


/** Imposta tutti i thread a target=true */
void terminaTuttiThread(GameData* gameData , struct Semaphore* allSem){
    ThreadControlBlock *tcb_thread = NULL;
    sem_t *semaforoTCB = &(allSem->tcb_mutex); 

    /*  RANA    */
    tcb_thread = gameData->allTCB->tcb_rana;
    impostaThreadTarget(tcb_thread, semaforoTCB);

    /* PROIETTILI */
    tcb_thread = gameData->allTCB->tcb_proiettili;
    for(int i=0; i< MAXNPROIETTILI; i++){
        impostaThreadTarget( &(tcb_thread[i]), semaforoTCB);
    }

    /*  NEMICO_PIANTA   */
    tcb_thread = gameData->allTCB->tcb_piante;
    for(int i=0; i< MAXNNEMICI; i++){
        impostaThreadTarget( &(tcb_thread[i]), semaforoTCB);
    }

    /* PROIETTILI NEMICI */
    tcb_thread = gameData->allTCB->tcb_proiettili_nemici;
    for(int i=0; i< MAXNPROIETTILINEMICI; i++){
        impostaThreadTarget( &(tcb_thread[i]), semaforoTCB);
    }

    /* COCOCDRILLI  */
    tcb_thread = gameData->allTCB->tcb_coccodrilli;
    for(int i=0; i< MAXNCOCCODRILLI; i++){
        impostaThreadTarget( &(tcb_thread[i]), semaforoTCB);
    }


    /*  TEMPO    */
    //tcb_thread = &gameData->allTCB->tcb_tempo;
    //impostaThreadTarget(tcb_thread, semaforoTCB);



}



/**
 * Imposta tutti i thread a target per farli terminare
 * Resetta conteggio tempo e HUD del tempo
 * N.B. Neccessita di (Params*) per avere i semafori per impostare i thread a "target"
 */
void resetManche(Params *p)
{
    GameData *gameData = p->gameData;
    struct Semaphore* allSem = p->semafori;
    
    // conta i thread attivi al momento (eccetto Disegna,Tempo) (+1 per la Rana)
    //int thread_attivi = 1 + gameData->contatori.contCoccodrilli + gameData->contatori.contNemici + gameData->contatori.contProiettili + gameData->contatori.contProiettiliN;
    
    terminaTuttiThread(gameData, allSem);  // Imposta tutti i thread a target per farli terminare

    printInitTempo(p->gameData); 
    gameData->gameInfo.tempo.start = time(NULL);	//resetta tempo
    gameData->gameInfo.tempoIsChanged = true;
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
    *(old_pos) = *(pipeData);
    return;
}

bool isGameOver(GameData* gameData){
    return gameData->gameInfo.vite==0 || gameData->gameInfo.manche==MAXNTANE;
}

bool isWin(GameData* gameData){
    return gameData->gameInfo.manche==MAXNTANE;
}

/**
 * Controlla che il movimento della Rana sul coccodrillo sia permesso
 */
bool isFrogMoveLecit(int newX, int newY, RanaAbsPos ranaPos, PipeData pipeData)
{
	if (ranaPos.on_coccodrillo && pipeData.x != 0)
	{
		// la rana è su un coccodrillo e sto cercando di muovermi lungo il coccodrillo
		int new_offset = ranaPos.offset_on_coccodrillo + pipeData.x;
		if (new_offset <= 0 || new_offset > COCCODRILLO_W - RANA_W - 1)
		{
			return false;
		}
	}
	return newX <= LASTGAMECOL - RANA_W + 1 && newX >= FIRSTGAMECOL && newY > FIRSTGAMEROW + RANA_H && newY < LASTGAMEROW;
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

int fromFilaToRow(int fila)
{
	int row = 0;
	switch (fila)
	{
	case 1:
		row = FILA_UNO;
		break;
	case 2:
		row = FILA_DUE;
		break;
	case 3:
		row = FILA_TRE;
		break;
	case 4:
		row = FILA_QUATTRO;
		break;
	case 5:
		row = FILA_CINQUE;
		break;
	case 6:
		row = FILA_SEI;
		break;
	case 7:
		row = FILA_SETTE;
		break;
	case 8:
		row = FILA_OTTO;
		break;
	default:
		break;
	}
	return row;
}


void debugBlock()
{
	nodelay(stdscr, FALSE); // riabilita l'input bloccante
	getch();				// Aspetta che l'utente prema un tasto qualsiasi

	nodelay(stdscr, TRUE); // riabilita l'input non bloccante
}