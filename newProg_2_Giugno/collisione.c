#include "collisione.h"

Collisione detectCollisione(GameData* gameData){
    Collisione collisione;
    collisione.tipoCollisione=NO_COLLISIONE;
    Schermo* schermo=&(gameData->schermo); // lo schermo di gioco
    
    // switch sul carattere in pipe
    switch(gameData->pipeData.type){
        // rana
        case 'X':
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
                        default:
                            break;
                    }

                }
            }
            break;
        default:
            break;
    }
    
    return collisione;
}

void handleCollisione(Params* thread_args, GameData* gameData, Collisione collisione){
    switch(collisione.tipoCollisione){
		case RANA_FIUME:

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
			refresh(); // Aggiorna la finestra
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
		default:
			break;
	}
    return;
}