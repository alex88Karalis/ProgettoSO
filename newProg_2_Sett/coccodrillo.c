#include "coccodrillo.h"
//#include "../hdr/coccodrillo.h"

/*  PROCESSI    */


pid_t avviaCoccodrillo(int *pipe_fd, PipeData* coccodrillo_init, int id)
{

    pid_t coccodrillo_pid = fork();

    if (coccodrillo_pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }
    else if (coccodrillo_pid == 0)
    {
        // Processo coccodrillo
        close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
        
        coccodrillo(pipe_fd, coccodrillo_init, id);
        exit(0);
    }
    else
    {
        return coccodrillo_pid;
    }
}

void coccodrillo(int *pipe_fd, PipeData *coccodrillo_init, int id)
{
    PipeData coccodrillo;
    coccodrillo.x = coccodrillo_init->x; // le coordinate iniziali del coccodrillo sono quelle dell' oggetto che ha sparato
    coccodrillo.y = coccodrillo_init->y;
    coccodrillo.type = coccodrillo_init->type;
    coccodrillo.id = id;
    int dirX = 1;

    // Seme unico per ogni processo
    unsigned int seed = time(NULL) ^ (getpid() << 16); 

    // la direzione deve essere calcolata in base alla y iniziale
    switch (coccodrillo_init->y)
    {
    case FILA_UNO:
    case FILA_TRE:
    case FILA_CINQUE:
    case FILA_SETTE:
        // il coccodrillo si muove verso destra percio' la direzione e' 1
        dirX = 1;
        break;
    case FILA_DUE:
    case FILA_QUATTRO:
    case FILA_SEI:
    case FILA_OTTO:
        // il coccodrillo si muove verso sinistra percio' la direzione e' -1
        dirX = -1;
        break;
    default:
        break;
    }

    // numero randomico tra min e max compresi
        int randomico = generaRandom_r(0 , 200000*15,&seed);
        // piccola usleep
        usleep(randomico);
    
    while (1)
    {
        coccodrillo.x += dirX;
        write(pipe_fd[1], &coccodrillo, sizeof(PipeData));
        usleep(200000); // Aspetta un po' prima di generare nuove coordinate
    }
}

void uccidiCoccodrillo(pid_t *pids_coccodrilli, int id_coccodrillo)
{
    if ((id_coccodrillo != -1) && (pids_coccodrilli[id_coccodrillo] != 0))
    {
        kill(pids_coccodrilli[id_coccodrillo], SIGKILL);
        int err = waitpid(pids_coccodrilli[id_coccodrillo], NULL, 0);
        if (err == -1)
        {
            perror("Errore nella waitpid");
            exit(1);
        }
        pids_coccodrilli[id_coccodrillo] = 0;
    }
    return;
}


/*  THREAD */

pthread_t avviaCoccodrilloThread(Params_coccodrilli *thread_args, int id)
{

    pthread_t tid_coccodrillo ;
	thread_args->param->id = id;  //Passo l'id valido del cccoccodrillo agli argomenti del thread
    if( pthread_create(&tid_coccodrillo, NULL, &coccodrilloThread, (void*)thread_args)!= 0)
    {
        perror("ERR: Avvio Coccodrillo Fallito!");
		_exit(3);
    }
    // scrivo il suo threadID direttamente nei pids 
    Params* p = thread_args->param;
    
    //p->gameData->pids.pidCoccodrilli[id] = tid_coccodrillo;
    return tid_coccodrillo;
}// end avviaCOccodrilloThread

void *coccodrilloThread(void *param){
    
    Params_coccodrilli *p_coccodrillo = (Params_coccodrilli*) param; // recupero ptr a parametri_coccodrillo
    Params *p = (Params*)p_coccodrillo->param;                      // recupero ptr ai parametri thread generali.
    
    sem_t *semaforoTCB = &(p->semafori->tcb_mutex);
	pthread_t my_tid = pthread_self();
	ThreadControlBlock my_tcb = {my_tid,false,false};
	copiaTCB(&(p->gameData->allTCB->tcb_coccodrilli[p->id]), my_tcb, semaforoTCB);

    ThreadControlBlock *coccodrilloTCB = &(p->gameData->allTCB->tcb_coccodrilli[p->id]);

    PipeData coccodrillo_init = p_coccodrillo->coccodrillo_init; // recupero coordinate iniziali

    Flusso flusso;
    switch (coccodrillo_init.y)     // recupero il flusso di questo coccodrillo
    {
    case FILA_UNO:
        flusso = p->gameData->flussi[0];
        break;
    case FILA_DUE:
        flusso = p->gameData->flussi[1];
        break;
    case FILA_TRE:
        flusso = p->gameData->flussi[2];
        break;
    case FILA_QUATTRO:
        flusso = p->gameData->flussi[3];
        break;
    case FILA_CINQUE:
        flusso = p->gameData->flussi[4];
        break;
    case FILA_SEI:
        flusso = p->gameData->flussi[5];
        break;
    case FILA_SETTE:
        flusso = p->gameData->flussi[6];
        break;
    case FILA_OTTO:
        flusso = p->gameData->flussi[7];
        break;
    default:
        break;
    }


    PipeData coccodrillo;
    coccodrillo.x = coccodrillo_init.x; // le coordinate iniziali del coccodrillo
    coccodrillo.y = coccodrillo_init.y;
    coccodrillo.type = coccodrillo_init.type;
    coccodrillo.id = p->id;
    coccodrillo.thread_id = my_tid;
    //int dirX = 1;

    int direction, vel; // direzione e velocità passati al processo dall'esterno.
    vel = flusso.vel;

    direction = (coccodrillo_init.type == 'C') ? 1 : -1 ; // se init.type==C vai da sx a dx, altrimenti vai da dx a sx

    //int dirX = direction;
    
    int velocity;

    switch(vel){
        case 0:
        velocity = FLUSSO_LENTO;
        break;
        case 1:
        velocity =FLUSSO_NORM;
        break;
        case 2:
        velocity= FLUSSO_VELOCE;
        break;
    }

    // Seme unico per ogni processo
    unsigned int seed = time(NULL) ^ (pthread_self() << 16); 
    // la direzione deve essere calcolata in base alla y iniziale
/*
    switch (coccodrillo_init.y)
    {
    case FILA_UNO:
    case FILA_TRE:
    case FILA_CINQUE:
    case FILA_SETTE:
        // il coccodrillo si muove verso destra percio' la direzione e' 1
        dirX = 1;
        break;
    case FILA_DUE:
    case FILA_QUATTRO:
    case FILA_SEI:
    case FILA_OTTO:
        // il coccodrillo si muove verso sinistra percio' la direzione e' -1
        dirX = -1;
        break;
    default:
        break;
    }
/**/

    // numero randomico tra min e max compresi
    int randomico = generaRandom_r(0 , 200000*15,&seed);
   
    usleep(randomico);  // piccola usleep prima di partire
    
    while (1)
    {
        if(isThreadTarget(coccodrilloTCB,semaforoTCB)){
            break;
        }
        coccodrillo.x += direction;
        scriviSuBuffer(p, coccodrillo, coccodrilloTCB, false);
        
        usleep(velocity); // Aspetta un po' prima di generare nuove coordinate
    }
    
    //scriviSuBuffer(p, coccodrillo, coccodrilloTCB, true);
    /*  DA PROVARE/**/ 
    my_tcb.thread_id = pthread_self();
    my_tcb.is_target = true;
    my_tcb.is_terminated = true; 
    scriviSuTCB(p,coccodrilloTCB, my_tcb);
    /**/
   
    pthread_exit(NULL);
}// end coccodrilloThread


