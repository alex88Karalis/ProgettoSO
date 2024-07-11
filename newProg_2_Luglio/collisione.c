#include "collisione.h"
/**
 * Rileva se c'è stata una collisione, il tipo di collisione, quali oggetti sono coinvolti
 * @param gameData Puntatore alla struttura con tutti i dati di gioco
 * @return Collisione - Struttura con Il tipo di collisione e i dati utili degli oggetti coinvolti 
 */
Collisione detectCollisione(GameData* gameData){
    Collisione collisione;
    collisione.tipoCollisione=NO_COLLISIONE;
    Schermo* schermo=&(gameData->schermo); // lo schermo di gioco
    
    // switch sul carattere in pipe
    switch(gameData->pipeData.type){
        // rana
        case 'X':   // Rana
        {
            // calcolo posizone assoluta della rana in potenza
            // ovvero sommo a abspos la posizione relativa in pipe
            RanaAbsPos ranaPos;
            ranaPos.x=gameData->ranaAbsPos.x + gameData->pipeData.x;
            ranaPos.y = gameData->ranaAbsPos.y + gameData->pipeData.y;

            // se il movimento della rana non è lecito non c'è aggiornamento ne collisione
            if(!isFrogMoveLecit(ranaPos.x,ranaPos.y)){
                return collisione;
            }

            // se la rana si è spostata lungo un coccodrillo devo modificare solo l'offset della rana
            if(gameData->pipeData.x != 0 && gameData->ranaAbsPos.on_coccodrillo){
                
                int offset= gameData->ranaAbsPos.offset_on_coccodrillo + gameData->pipeData.x;
                // se il movimento lungo il coccodrillo è lecito ovvero non sfora:
                if(offset>=0 && offset<=9){
                    // aggiorno offset
                    gameData->ranaAbsPos.offset_on_coccodrillo += gameData->pipeData.x;
                }
                else{
                    // non c'è aggiornamento ne collisione
                    return collisione;
                }
                
            }

            for (int row = ranaPos.y; row < ranaPos.y + RANA_H; row++)
            {
                for(int col = ranaPos.x; col < ranaPos.x + RANA_W; col++){
                    
                    switch(schermo->screenMatrix[row][col].tipo){
                        case FIUME_OBJ:
                            collisione.tipoCollisione=RANA_FIUME;
                            collisione.id_oggetto_attivo=gameData->pipeData.id;
                            collisione.oggetto_attivo= RANA_OBJ;
                            collisione.oggetto_passivo=FIUME_OBJ;
                            collisione.id_oggetto_passivo=0;
                            return collisione;
                            break;
                        case TANA_OPEN_OBJ:
                            collisione.tipoCollisione=RANA_TANA_APERTA;
                            collisione.id_oggetto_attivo=gameData->pipeData.id;
                            collisione.oggetto_attivo= RANA_OBJ;
                            collisione.oggetto_passivo=TANA_OPEN_OBJ;
                            collisione.id_oggetto_passivo=schermo->screenMatrix[row][col].id;
                            return collisione;
                            break;
                        case TANA_CLOSE_OBJ:
                            collisione.tipoCollisione=RANA_TANA_CHIUSA;
                            collisione.id_oggetto_attivo=gameData->pipeData.id;
                            collisione.oggetto_attivo= RANA_OBJ;
                            collisione.oggetto_passivo=TANA_CLOSE_OBJ;
                            collisione.id_oggetto_passivo=schermo->screenMatrix[row][col].id;
                            return collisione;
                            break;

                        case COCCODRILLO_BUONO_OBJ:
                            collisione.tipoCollisione = RANA_COCCODRILLO_BUONO;
                            collisione.id_oggetto_attivo = gameData->pipeData.id;
                            collisione.oggetto_attivo = RANA_OBJ;
                            collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                            collisione.oggetto_passivo = COCCODRILLO_BUONO_OBJ;
                            collisione.hit_point_x=col;
                            collisione.hit_point_y=row;
                            return collisione;
                            break;

                        case PROIETTILE_NEMICO_OBJ:
                            /* Rileva la collisione della Rana che passa sul proiettileNemico rimasto a terra
                                - possibile implemento per una feature aggiuntiva al gameplay
                            
                            collisione.tipoCollisione=PROIETTILE_NEMICO_RANA;
                            collisione.id_oggetto_attivo=gameData->pipeData.id;
                            collisione.oggetto_attivo= RANA_OBJ;
                            collisione.oggetto_passivo=PN_OBJ;
                            collisione.id_oggetto_passivo=schermo->screenMatrix[row][col].id;
                            return collisione;
                            /**/
                            break;
                        default:
                            break;
                    }
                }
            }
            break;
        }
        
        case 'P':   // proiettile Rana
        {
            PipeData proiettile = gameData->pipeData;
            for (int row = proiettile.y; row < proiettile.y + PROIETTILE_H; row++)
            {
                for(int col = proiettile.x; col < proiettile.x + PROIETTILE_W; col++)
                {
                    switch(schermo->screenMatrix[row][col].tipo){
                        
                        case NEMICO_OBJ:
                            collisione.tipoCollisione=PROIETTILE_RANA_NEMICO;
                            collisione.id_oggetto_attivo=gameData->pipeData.id;
                            collisione.oggetto_attivo= PROIETTILE_OBJ;
                            collisione.oggetto_passivo=NEMICO_OBJ;
                            collisione.id_oggetto_passivo=schermo->screenMatrix[row][col].id;
                            return collisione;
                            break;
                        
                        case PROIETTILE_NEMICO_OBJ:
                            collisione.tipoCollisione=PROIETTILE_RANA_PROIETTILE_NEMICO;
                            collisione.id_oggetto_attivo=gameData->pipeData.id;
                            collisione.oggetto_attivo= PROIETTILE_OBJ;
                            collisione.oggetto_passivo=PROIETTILE_NEMICO_OBJ;
                            collisione.id_oggetto_passivo=schermo->screenMatrix[row][col].id;
                            return collisione;
                            break;
                        
                        case COCCODRILLO_CATTIVO_OBJ:
                            collisione.tipoCollisione = PROIETTILE_COCCODRILLO_CATTIVO;
                            collisione.id_oggetto_attivo = proiettile.id;
                            collisione.oggetto_attivo = PROIETTILE_OBJ;
                            collisione.oggetto_passivo = COCCODRILLO_CATTIVO_OBJ;
                            collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                            collisione.hit_point_x=col;
                            collisione.hit_point_y=row;
                            break;

                        case COCCODRILLO_BUONO_OBJ:
                            collisione.tipoCollisione = PROIETTILE_COCCODRILLO_BUONO;
                            collisione.id_oggetto_attivo = proiettile.id;
                            collisione.oggetto_attivo = PROIETTILE_OBJ;
                            collisione.oggetto_passivo = COCCODRILLO_BUONO_OBJ;
                            collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                            collisione.hit_point_x=col;
                            collisione.hit_point_y=row;
                            break;
                        default:
                            break;
                    }
                }
            }
            break;
        }
         
        case 'p':   // proiettile Nemico
        {
            PipeData proiettileNemico = (gameData->pipeData);
            for (int row = proiettileNemico.y; row < proiettileNemico.y + PROIETTILE_H; row++)
            {
                for(int col = proiettileNemico.x; col < proiettileNemico.x + PROIETTILE_W; col++)
                {
                    switch(schermo->screenMatrix[row][col].tipo){
                        
                        case RANA_OBJ:  // proiettileNemico ha colpito la Rana
                            collisione.tipoCollisione=PROIETTILE_NEMICO_RANA;
                            collisione.id_oggetto_attivo=gameData->pipeData.id;
                            collisione.oggetto_attivo= PROIETTILE_NEMICO_OBJ;
                            collisione.oggetto_passivo=RANA_OBJ;
                            collisione.id_oggetto_passivo=schermo->screenMatrix[row][col].id;
                            return collisione;
                            break;

                        case PROIETTILE_OBJ:    //piettileNemico ha colpito proiettileRana
                        // oggetto attivo = proiettileNemico , oggetto passivo = proiettileRana
                            collisione.tipoCollisione = PROIETTILE_NEMICO_PROIETTILE_RANA;
                            collisione.id_oggetto_attivo = gameData->pipeData.id;
                            collisione.oggetto_attivo = PROIETTILE_NEMICO_OBJ ;
                            collisione.oggetto_passivo = PROIETTILE_OBJ;
                            collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id; 
                            return collisione;
                            break;
                        default:
                            break;
                    }
                }
            }
            break;
        }
       
        case 'C':   // Coccodrilli
        case 'c':
        {
            PipeData coccodrilloData = gameData->pipeData;
            if (!(gameData->controlloCoccodrilli[coccodrilloData.id].is_buono)) // se coccodrillo è cattivo
            {
                for (int row = coccodrilloData.y; row < coccodrilloData.y + COCCODRILLO_H; row++)
                {
                    for (int col = coccodrilloData.x; col < coccodrilloData.x + COCCODRILLO_W; col++)
                    {

                        switch (schermo->screenMatrix[row][col].tipo) 
                        {
                        case PROIETTILE_OBJ:        // collide con proiettileRana
                        /*  oggetto attivo = coccodrillo, oggetto passivo = proiettile  */
                            collisione.tipoCollisione = PROIETTILE_COCCODRILLO_CATTIVO;
                            collisione.id_oggetto_attivo = coccodrilloData.id;
                            collisione.oggetto_attivo = COCCODRILLO_CATTIVO_OBJ;
                            collisione.oggetto_passivo = PROIETTILE_OBJ;
                            collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                            collisione.hit_point_x=col;
                            collisione.hit_point_y=row;
                            return collisione;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            else    //se il coccodrillo è buono
            {   
                 for (int row = coccodrilloData.y; row < coccodrilloData.y + COCCODRILLO_H; row++)
                {
                    for (int col = coccodrilloData.x; col < coccodrilloData.x + COCCODRILLO_W; col++)
                    {
                        switch (schermo->screenMatrix[row][col].tipo)   // ha colliso con...?
                        {
                        case RANA_OBJ:  // RANA
                            /* oggetto attivo = coccodrillo, oggetto passivo = rana */
                            collisione.tipoCollisione = RANA_COCCODRILLO_BUONO;
                            collisione.id_oggetto_attivo = gameData->pipeData.id;
                            collisione.oggetto_attivo = COCCODRILLO_BUONO_OBJ ;
                            collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                            collisione.oggetto_passivo = RANA_OBJ;
                            collisione.hit_point_x=col;
                            collisione.hit_point_y=row;
                            return collisione;
                            break;
                        default:
                            break;
                        }
                        
                    }
                }

                
                
            }
            
        }
        default:
            break;
    }
    
    return collisione;
}

void handleCollisione(Params* thread_args, GameData* gameData, Collisione collisione)
{
    Semaphore* semafori = thread_args->semafori;    // Riferimento a struttura con tutti i semafori
    sem_t* semaforoWindow = &(thread_args->semafori->window_mutex);
    sem_t* semaforoTCB = &(thread_args->semafori->tcb_mutex);
    switch(collisione.tipoCollisione){
		case RANA_FIUME:    // Rana è caduta nel fiume OK   
        {
			// stampo la rana sopra il fiume
			int newPosAbsRanaX = gameData->pipeData.x + gameData->ranaAbsPos.x;
			int newPosAbsRanaY = gameData->pipeData.y + gameData->ranaAbsPos.y;
			gameData->pipeData.x = newPosAbsRanaX;
			gameData->pipeData.y = newPosAbsRanaY;
            
			// normale aggiornamento
			aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
			gameData->ranaAbsPos.x = gameData->pipeData.x;
			gameData->ranaAbsPos.y = gameData->pipeData.y; 
			stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
			
            sem_wait(semaforoWindow);
            refresh(); // Aggiorna la finestra
            sem_post(semaforoWindow);
			usleep(500000);

			// riproduco suono plof
			// tolgo una vita alla rana
			// schermata vite--
			// faccio ripartire la rana
			
            // dico alla Rana di terminare
            int err= impostaThreadTarget(gameData->allTCB->tcb_rana, &thread_args->semafori->tcb_mutex);
            // controllo
            if (err) { perror("Imposta Rana Target FAILED "); }
            
			if(gameData->gameInfo.vite>0 ){ // decremento vite
			    gameData->gameInfo.vite--;
                gameData->gameInfo.viteIsChanged = true;
            }   
            
			aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
			break;
        }

        case PROIETTILE_NEMICO_RANA: // Proiettile_Nemico ha colpito la Rana OK
        {
            /* Far terminare la Rana, Far terminare il Proiettile_Nemico */
            //ThreadControlBlock *tcb_proiettileNemico = cercaThreadByTid(gameData->allTCB->tcb_proiettili_nemici, proiettile.thread_id, &semafori->tcb_mutex, MAXNPROIETTILINEMICI);
            
            PipeData proiettile = gameData->pipeData;
            ThreadControlBlock *tcb_proiettileNemico = &(gameData->allTCB->tcb_proiettili_nemici[proiettile.id]);
            ThreadControlBlock *tcb_rana =  gameData->allTCB->tcb_rana;

            if(isThreadTarget(tcb_proiettileNemico,&semafori->tcb_mutex)){ break; }
            if(isThreadTarget(tcb_rana,&semafori->tcb_mutex)){break;}

            if(impostaThreadTarget(tcb_proiettileNemico,&semafori->tcb_mutex)!=0) // fa terminare il proiettile nemico
            {   
                perror("ERRORE imposta thread Target");
                break;
            }else{
                beep();
            }

            if(impostaThreadTarget(tcb_rana, &semafori->tcb_mutex)!=0) // fa terminare la RANA
            {
                perror("ERR: Imposta Rana Target Fallito");
                break;
            }else{
                if(gameData->gameInfo.vite>0 ){ // decremento vite
			        gameData->gameInfo.vite--;
                    gameData->gameInfo.viteIsChanged = true;
                }   
            }

            break;
        }
        case PROIETTILE_RANA_NEMICO: // Proiettile_Rana ha colpito il Nemico
        {
            beep();
            /* Il proiettile è l'oggetto attivo , il nemico_pianta è l'oggetto passivo della collisione */
            //PipeData proiettile = gameData->pipeData;

            if(gameData->pipeData.id != collisione.id_oggetto_attivo)
            {
                perror("ID proiettileRana non coincide");
            }
            //PipeData *nemico = &(gameData->oldPos.nemici[collisione.id_oggetto_passivo]);
            ThreadControlBlock *tcb_nemico = &(gameData->allTCB->tcb_piante[collisione.id_oggetto_passivo]);
            //ThreadControlBlock *tcb_proiettile = &(gameData->allTCB->tcb_proiettili[proiettile.id]); 
            ThreadControlBlock *tcb_proiettile = &(gameData->allTCB->tcb_proiettili[collisione.id_oggetto_attivo]); 

            /* controlli di routine , evitano doppie collisioni */
            if(isThreadTarget(tcb_nemico,&(semafori->tcb_mutex))){ break; }
            if(isThreadTarget(tcb_proiettile,&(semafori->tcb_mutex)))
            {   //?? 
                cancellaOggettoDaMatrice(gameData,&(gameData->oldPos.proiettili[collisione.id_oggetto_attivo]),gameData->oldPos.proiettili,S_PROIETTILE);
                break; 
            }

            /* Imposto i thread proiettile e nemico_pianta per terminare */
            if(0 != impostaThreadTarget(tcb_nemico,&(semafori->tcb_mutex)))
            {
                perror("ERRORE imposta thread Target");
                break;
            }
            if(0 != impostaThreadTarget(tcb_proiettile,&(semafori->tcb_mutex)))
            {
                perror("ERRORE imposta thread Target");
                break;
            }

            gameData->gameInfo.punteggio+=5;
            gameData->gameInfo.punteggioIsChanged=true;
            //beep();
            break;
        }
        case PROIETTILE_RANA_PROIETTILE_NEMICO: // Proiettile_Rana ha colpito Proiettile_Nemico
        {   // oggetto attivo = proiettileRana. oggetto passivo = proiettileNemico
            // su gamData->pipeData è scritto ProiettileRana

            ThreadControlBlock *tcb_proiettileRana = &(gameData->allTCB->tcb_proiettili[collisione.id_oggetto_attivo]);
            ThreadControlBlock *tcb_proiettileNemico = &(gameData->allTCB->tcb_proiettili_nemici[collisione.id_oggetto_passivo]);
            
            pthread_t tid_proiettile = leggiTid(tcb_proiettileRana,semaforoTCB);
            if(tid_proiettile == 0) // il TCB del proiettile è gia stato resettato
            {
                cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
                //beep(); 
                break;
            }
            
            if(isThreadTarget(tcb_proiettileRana,semaforoTCB)){
                if(isThreadTerminated(tcb_proiettileRana,semaforoTCB))//ultima scrittura
                {
                    cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
                    //beep(); 
                    break;
                } 
            }
            
            if(isThreadTarget(tcb_proiettileNemico,semaforoTCB)){
                if(isThreadTerminated(tcb_proiettileNemico,semaforoTCB))//ultima scrittura
                {
                    PipeData *proiettileNemico = & (gameData->oldPos.proiettiliNemici[collisione.id_oggetto_passivo]);
                    cancellaOggettoDaMatrice(gameData, proiettileNemico, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);

                    //cancellaOggetto(gameData, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
                    //beep(); 
                    break;
                } 
            }
            /**/
            impostaThreadTarget(tcb_proiettileRana,semaforoTCB);
            impostaThreadTarget(tcb_proiettileNemico,semaforoTCB);

            beep();
            break;
        }
        
        case PROIETTILE_COCCODRILLO_CATTIVO:
        {
            // il proiettile muore e il coccodrillo diventa buono
            // l'oggetto attivo è il proiettile
            if (collisione.oggetto_attivo == PROIETTILE_OBJ)
            {
                // uccide il processo proiettile
                ThreadControlBlock *tcb_proiettile = &(gameData->allTCB->tcb_proiettili[collisione.id_oggetto_attivo]);
                pthread_t tid_proiettile = leggiTid( tcb_proiettile, semaforoTCB);

                if(tid_proiettile==0)   // collisione rilevata da thread gia chiuso e tcb resettato
                {
                    cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
                    break;
                }
                
                if(isThreadTarget(tcb_proiettile, semaforoTCB)){
                    if(isThreadTerminated(tcb_proiettile, semaforoTCB))
                    { 
                        cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
                    }
                    break; 
                }

                impostaThreadTarget(tcb_proiettile,semaforoTCB);
                    
                    //uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_attivo); 
                
                // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
                    //cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
                    //gameData->contatori.contProiettili--;


                // il coccodrillo diventa buono
                gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].is_buono = true;

                // il coccodrillo esce da tutte le fasi in cui si trova
                gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].offset_deep = 0;
                gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].is_fase_pre_immersione = false;
                gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].is_fase_immersione = false;
                gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].is_going_deep = false;
                gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].is_going_up = false;
                gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].passi_in_immersione = 0;
                gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].passi_in_pre_immersione = 0;
                gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].passi_deep = 0;
            }
            else
            {
                /*
                // l'oggetto attivo è il tronco
                // faccio caso collisione apparte?

                uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_passivo); // uccide il processo proiettile
                // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
                cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
                gameData->contatori.contProiettili--;

                // il coccodrillo diventa buono
                gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].is_buono = true;
                // il coccodrillo esce da tutte le fasi in cui si trova
                gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].offset_deep = 0;
                gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].is_fase_pre_immersione = false;
                gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].is_fase_immersione = false;
                gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].is_going_deep = false;
                gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].is_going_up = false;
                gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].passi_in_immersione = 0;
                gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].passi_in_pre_immersione = 0;
                gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].passi_deep = 0;
                /**/
            }
            break;
        }

        case PROIETTILE_COCCODRILLO_BUONO:  // proiettile Rana collide con Coccodrillo Buono
        {
            /*  distruggo il proiettile e basta  */
            if(collisione.oggetto_attivo==PROIETTILE_OBJ)
            {
                ThreadControlBlock *tcb_proiettile = &(gameData->allTCB->tcb_proiettili[collisione.id_oggetto_attivo]);
                pthread_t tid_proiettile = leggiTid(tcb_proiettile, semaforoTCB);
                if(tid_proiettile==0){ 
                    // cancellando la sprite evita altre collisioni
                    cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
                    break;
                }
                if(isThreadTarget(tcb_proiettile, semaforoTCB)){
                    if(isThreadTerminated(tcb_proiettile, semaforoTCB)){
                        break;
                    }
                }
                impostaThreadTarget(tcb_proiettile, semaforoTCB); 
            
            }else   // oggetto attivo = coccodrillo
            {   
                /*  ce n'è bisogno ?. 
                    devo distruggere solo il proiettile, al coccodrillo non succede nulla. 
                */

            }
            break;
        }

        case PROIETTILE_NEMICO_PROIETTILE_RANA: // proiettileNemico ha colpito proiettileRana
        {   // oggetto attivo = proiettileNemico. oggetto passivo = proiettileRana
            // su gameData->pipData è scritto il proiettileNemico !!

            ThreadControlBlock *tcb_proiettileRana = &(gameData->allTCB->tcb_proiettili[collisione.id_oggetto_passivo]);
            ThreadControlBlock *tcb_proiettileNemico = &(gameData->allTCB->tcb_proiettili_nemici[collisione.id_oggetto_attivo]);


            if(isThreadTarget(tcb_proiettileNemico,semaforoTCB))            // già impostato per terminare
            {
                if(isThreadTerminated(tcb_proiettileNemico,semaforoTCB))    // ha fatto l' ultima scrittura
                {
                    cancellaOggetto(gameData, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
                    //beep(); 
                    break;
                } 
            }

            if(isThreadTarget(tcb_proiettileRana,semaforoTCB)){
                if(isThreadTerminated(tcb_proiettileRana,semaforoTCB))      //ultima scrittura
                {   
                    PipeData *proiettile = &(gameData->oldPos.proiettili[collisione.id_oggetto_passivo]);
                    cancellaOggettoDaMatrice(gameData, proiettile ,gameData->oldPos.proiettili, S_PROIETTILE);
                    //beep(); 
                    break;
                } 
            }
            impostaThreadTarget(tcb_proiettileNemico,semaforoTCB);
            impostaThreadTarget(tcb_proiettileRana,semaforoTCB);

            beep();
            break;
        }
        
        case RANA_COCCODRILLO_BUONO:
        {
            if(collisione.oggetto_attivo==RANA_OBJ){    // oggetto attivo=Rana. oggetto passivo=Coccodrillo
                // stampo il coccodrillo buono
                if(gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].direction==1){
                    aggiornaOggettoNew(gameData,gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo],gameData->oldPos.coccodrilli,S_COCCODRILLO_DX);
                }
                else{
                    aggiornaOggettoNew(gameData,gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo],gameData->oldPos.coccodrilli,S_COCCODRILLO_SX);
                }

                // stampo sopra il coccodrillo buono la rana
                // aggiorno abs pos rana
                gameData->ranaAbsPos.x = collisione.hit_point_x;
                gameData->ranaAbsPos.y = collisione.hit_point_y;
                gameData->ranaAbsPos.on_coccodrillo=true;
                gameData->ranaAbsPos.id_coccodrillo= collisione.id_oggetto_passivo;
                // aggiorno offset rana su coccodrillo
                gameData->ranaAbsPos.offset_on_coccodrillo= collisione.hit_point_x- gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo].x;
                // stampo e aggiorno rana
                PipeData ranaData;
                ranaData.x= gameData->ranaAbsPos.x;
                ranaData.type='X';
                ranaData.y=gameData->ranaAbsPos.y;
                ranaData.id= 0;
                aggiornaOggettoNew(gameData,ranaData,&(gameData->oldPos.rana),S_RANA);

            }else{  
            
                // l'oggetto attivo è il coccodrillo buono
                // stampo il coccodrillo buono
                if(gameData->controlloCoccodrilli[gameData->pipeData.id].direction==1){
                    aggiornaOggetto(gameData,gameData->oldPos.coccodrilli,S_COCCODRILLO_DX);
                }
                else{
                    aggiornaOggetto(gameData,gameData->oldPos.coccodrilli,S_COCCODRILLO_SX);
                }
                // aggiorno abs pos rana
                gameData->ranaAbsPos.x = collisione.hit_point_x;
                gameData->ranaAbsPos.y = collisione.hit_point_y;
                gameData->ranaAbsPos.on_coccodrillo=true;
                gameData->ranaAbsPos.id_coccodrillo= collisione.id_oggetto_attivo;
                // aggiorno offset rana su coccodrillo
                gameData->ranaAbsPos.offset_on_coccodrillo = collisione.hit_point_x - gameData->oldPos.coccodrilli[collisione.id_oggetto_attivo].x ;
                // stampo e aggiorno rana
                PipeData ranaData;
                ranaData.x= gameData->ranaAbsPos.x;
                ranaData.type='X';
                ranaData.y=gameData->ranaAbsPos.y;
                ranaData.id= 0;
                // stampo sopra il coccodrillo buono la rana
                aggiornaOggettoNew(gameData,ranaData, &(gameData->oldPos.rana), S_RANA);

            /**/
            }
            break;
        }
        
        default:
			break;
	}
    return;
}