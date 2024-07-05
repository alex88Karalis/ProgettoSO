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
        case 'p':   // proiettile Nemico
        {
            PipeData proiettileNemico = (gameData->pipeData);
            for (int row = proiettileNemico.y; row < proiettileNemico.y + PROIETTILE_H; row++)
            {
                for(int col = proiettileNemico.x; col < proiettileNemico.x + PROIETTILE_W; col++)
                {
                    switch(schermo->screenMatrix[row][col].tipo){
                        
                        case RANA_OBJ:
                            collisione.tipoCollisione=PROIETTILE_NEMICO_RANA;
                            collisione.id_oggetto_attivo=gameData->pipeData.id;
                            collisione.oggetto_attivo= PROIETTILE_NEMICO_OBJ;
                            collisione.oggetto_passivo=RANA_OBJ;
                            collisione.id_oggetto_passivo=schermo->screenMatrix[row][col].id;
                            return collisione;
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
                        default:
                            break;
                    }
                }
            }
            break;
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
                /* aggiorna proiettile_Nemico e aggiorna contatore proiettili_Nemici */
			    cancellaOggetto(gameData, &(gameData->oldPos.proiettiliNemici), S_PROIETTILE_NEMICO);
                gameData->contatori.contProiettiliN--;
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
            PipeData proiettile = gameData->pipeData;
            PipeData *nemico = &(gameData->oldPos.nemici[collisione.id_oggetto_passivo]);
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
        case PROIETTILE_RANA_PROIETTILE_NEMICO:
        {
            ThreadControlBlock *tcb_proiettileRana = &(gameData->allTCB->tcb_proiettili[collisione.id_oggetto_attivo]);
            ThreadControlBlock *tcb_proiettileNemico = &(gameData->allTCB->tcb_proiettili_nemici[collisione.id_oggetto_passivo]);
            if(isThreadTarget(tcb_proiettileRana,semaforoTCB)){ beep(); break;}

            impostaThreadTarget(tcb_proiettileRana,semaforoTCB);

            beep();
            break;
        }
        default:
			break;
	}
    return;
}