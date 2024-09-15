#include "collisione.h"
/**
 * Rileva se c'è stata una collisione, il tipo di collisione, quali oggetti sono coinvolti
 * @param gameData Puntatore alla struttura con tutti i dati di gioco
 * @return Collisione - Struttura con Il tipo di collisione e i dati utili degli oggetti coinvolti
 */
Collisione detectCollisione(GameData *gameData)
{
    Collisione collisione;
    collisione.tipoCollisione = NO_COLLISIONE;
    Schermo *schermo = &(gameData->schermo); // lo schermo di gioco

    // switch sul carattere in pipe
    switch (gameData->pipeData.type)
    {
    // rana
    case 'X': // Rana
    {
        // calcolo posizone assoluta della rana in potenza
        // ovvero sommo a abspos la posizione relativa in pipe
        RanaAbsPos ranaPos;
        ranaPos.x = gameData->ranaAbsPos.x + gameData->pipeData.x;
        ranaPos.y = gameData->ranaAbsPos.y + gameData->pipeData.y;

        // se il movimento della rana non è lecito non c'è aggiornamento ne collisione
        if (!isFrogMoveLecit(ranaPos.x, ranaPos.y, gameData->ranaAbsPos, gameData->pipeData))
        {
            return collisione;
        }

        // se la rana si è spostata lungo un coccodrillo devo modificare solo l'offset della rana
        if (gameData->pipeData.x != 0 && gameData->ranaAbsPos.on_coccodrillo)
        {

            int offset = gameData->ranaAbsPos.offset_on_coccodrillo + gameData->pipeData.x;
            // se il movimento lungo il coccodrillo è lecito ovvero non sfora:
            if (offset >= 0 && offset <= 9)
            {
                // aggiorno offset
                gameData->ranaAbsPos.offset_on_coccodrillo += gameData->pipeData.x;
            }
            else
            {
                // non c'è aggiornamento ne collisione
                return collisione;
            }
        }

        // controllo collisione sulla sprite della Rana
        for (int row = ranaPos.y; row < ranaPos.y + RANA_H; row++)
        {
            for (int col = ranaPos.x; col < ranaPos.x + RANA_W; col++)
            {

                switch (schermo->screenMatrix[row][col].tipo)
                {
                case FIUME_OBJ:
                {
                    setCollision(&collisione,RANA_FIUME,RANA_OBJ,gameData->pipeData.id,FIUME_OBJ,0,col,row);
                    return collisione;
                    break;
                }

                case LAVA_OBJ:
                {
                    setCollision(&collisione,RANA_LAVA,RANA_OBJ,gameData->pipeData.id,LAVA_OBJ,0,col,row);
                    return collisione;
                    break;
                }

                case TANA_OPEN_OBJ:
                {
                    setCollision(&collisione,RANA_TANA_APERTA,RANA_OBJ,gameData->pipeData.id,TANA_OPEN_OBJ,schermo->screenMatrix[row][col].id,col,row);
                    break;
                }
                case TANA_CLOSE_OBJ:
                {
                    setCollision(&collisione,RANA_TANA_CHIUSA,RANA_OBJ,gameData->pipeData.id,TANA_CLOSE_OBJ,schermo->screenMatrix[row][col].id,col,row);
                    return collisione;
                    break;
                }

                case COCCODRILLO_BUONO_OBJ:
                {
                    setCollision(&collisione,RANA_COCCODRILLO_BUONO,RANA_OBJ,gameData->pipeData.id,COCCODRILLO_BUONO_OBJ,schermo->screenMatrix[row][col].id,col,row);
                    break;
                }

                case COCCODRILLO_CATTIVO_OBJ:
                {
                    setCollision(&collisione,RANA_COCCODRILLO_CATTIVO,RANA_OBJ,gameData->pipeData.id,COCCODRILLO_CATTIVO_OBJ,schermo->screenMatrix[row][col].id,col,row);
                    break;
                }

                case PROIETTILE_NEMICO_OBJ:
                {
                    setCollision(&collisione,PROIETTILE_NEMICO_RANA, RANA_OBJ,gameData->pipeData.id,PROIETTILE_NEMICO_OBJ,schermo->screenMatrix[row][col].id,col,row);
                    return collisione;
                    break;
                }

                case MARCIAPIEDE_OBJ:
                {
                    if (gameData->ranaAbsPos.on_coccodrillo) // stai saltando da coccodrillo a marciapiede
                    {
                        setCollision(&collisione,RANA_MARCIAPIEDE,RANA_OBJ,gameData->pipeData.id,MARCIAPIEDE_OBJ,schermo->screenMatrix[row][col].id,col,row);
                        return collisione;
                    }
                    return collisione; // ritorna NO_COLLISIONE
                    break;
                }

                case ARGINE_OBJ:
                {
                    if (gameData->ranaAbsPos.on_coccodrillo) // stai saltando da coccodrillo a argine
                    {
                        setCollision(&collisione,RANA_ARGINE,RANA_OBJ,gameData->pipeData.id,ARGINE_OBJ,schermo->screenMatrix[row][col].id,col,row);
                        return collisione;
                    }
                    return collisione;
                    break;
                }

                case NEMICO_OBJ: // collisione Rana-NemicoPianta
                {
                    setCollision(&collisione,RANA_NEMICO,RANA_OBJ,gameData->pipeData.id,NEMICO_OBJ,schermo->screenMatrix[row][col].id,col,row);
                    return collisione;
                    break;
                }
                default:
                    break;
                }
            }
        }

        // todo forse questo va messo anche per altri tipi di collisione
        if (collisione.tipoCollisione == RANA_COCCODRILLO_BUONO || collisione.tipoCollisione == RANA_COCCODRILLO_CATTIVO || collisione.tipoCollisione== RANA_TANA_APERTA)
        {
            // modifico il punto di hit
            collisione.hit_point_x -= 2;
            collisione.hit_point_y -= 1;
        }
        return collisione;
        break;
    }

    case 'P': // proiettile Rana
    {
        PipeData proiettile = gameData->pipeData;
        for (int row = proiettile.y; row < proiettile.y + PROIETTILE_H; row++)
        {
            for (int col = proiettile.x; col < proiettile.x + PROIETTILE_W; col++)
            {
                switch (schermo->screenMatrix[row][col].tipo)
                {

                case NEMICO_OBJ:
                setCollision(&collisione,PROIETTILE_RANA_NEMICO,PROIETTILE_OBJ,gameData->pipeData.id,NEMICO_OBJ,schermo->screenMatrix[row][col].id,col,row);
                    return collisione;
                    break;
                case PROIETTILE_NEMICO_OBJ:
                    setCollision(&collisione,PROIETTILE_RANA_PROIETTILE_NEMICO,PROIETTILE_OBJ,gameData->pipeData.id,PROIETTILE_NEMICO_OBJ,schermo->screenMatrix[row][col].id,col,row);
                    return collisione;
                    break;
                case COCCODRILLO_CATTIVO_OBJ:
                    setCollision(&collisione,PROIETTILE_COCCODRILLO_CATTIVO,PROIETTILE_OBJ,proiettile.id,COCCODRILLO_CATTIVO_OBJ,schermo->screenMatrix[row][col].id,col,row);
                    break;
                case COCCODRILLO_BUONO_OBJ:
                    setCollision(&collisione,PROIETTILE_COCCODRILLO_BUONO,PROIETTILE_OBJ,proiettile.id,COCCODRILLO_BUONO_OBJ,schermo->screenMatrix[row][col].id,col,row);
                    break;
                default:
                    break;
                }
            }
        }
        break;
    }

    case 'p': // proiettile Nemico
    {
        PipeData proiettileNemico = (gameData->pipeData);
        for (int row = proiettileNemico.y; row < proiettileNemico.y + PROIETTILE_H; row++)
        {
            for (int col = proiettileNemico.x; col < proiettileNemico.x + PROIETTILE_W; col++)
            {
                switch (schermo->screenMatrix[row][col].tipo)
                {

                case RANA_OBJ: // proiettileNemico ha colpito la Rana
                    setCollision(&collisione,PROIETTILE_NEMICO_RANA,PROIETTILE_NEMICO_OBJ,gameData->pipeData.id,RANA_OBJ, schermo->screenMatrix[row][col].id,col,row);
                    return collisione;
                    break;
                case PROIETTILE_OBJ: // piettileNemico ha colpito proiettileRana
                    setCollision(&collisione, PROIETTILE_NEMICO_PROIETTILE_RANA, PROIETTILE_NEMICO_OBJ, gameData->pipeData.id, PROIETTILE_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    return collisione;
                    break;
                default:
                    break;
                }
            }
        }
        break;
    }

    case 'C': // Coccodrilli da sx a dx
    case 'c': // da dx a sx
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
                    case PROIETTILE_OBJ: // collide con proiettileRana
                        setCollision(&collisione, PROIETTILE_COCCODRILLO_CATTIVO, COCCODRILLO_CATTIVO_OBJ, coccodrilloData.id, PROIETTILE_OBJ, schermo->screenMatrix[row][col].id, col, row); 
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
/**
 * Imposta correttamente il tipo di collisione
 * @param collisione Struttura in cui inserire tutti i dati della collisione
 * @param tipo Tipo di collisione
 * @param attivo Tipo di oggetto attivo, quello che thriggera la collisione
 * @param id_attivo id dell'oggetto attivo
 * @param passivo Tipo oggetto passivo 
 * @param id_passivo id oggetto passivo
 * @param hit_x coordinata x del punto di collisione
 * @param hit_y coordinata y del punto di collisione
 * 
 */
void setCollision(Collisione *collisione, TipoCollisione tipo, TipoObj attivo, int id_attivo, TipoObj passivo, int id_passivo, int hit_x, int hit_y)
{
    collisione->tipoCollisione = tipo;
    collisione->oggetto_attivo = attivo;
    collisione->oggetto_passivo = passivo;
    collisione->id_oggetto_attivo = id_attivo;
    collisione->id_oggetto_passivo = id_passivo;
    collisione->hit_point_x = hit_x;
    collisione->hit_point_y = hit_y;
}

/**
 * Prende un oggetto Collisione e la gestisce
 * @param thread_args Struttura per i parametri da passare ai thread 
 * @param gameData Struttura con i principali dati di gioco 
 * @param collisione La collisione da gestire
 */
void handleCollisione(Params *thread_args, GameData *gameData, Collisione collisione)
{
    Semaphore *semafori = thread_args->semafori; // Riferimento a struttura con tutti i semafori
    sem_t *semaforoWindow = &(thread_args->semafori->window_mutex);
    sem_t *semaforoTCB = &(thread_args->semafori->tcb_mutex);
    switch (collisione.tipoCollisione)
    {
    case RANA_FIUME: // Rana è caduta nel fiume OK
    {
        ThreadControlBlock *tcb_rana = gameData->allTCB->tcb_rana;
        if (isThreadTarget(tcb_rana, semaforoTCB)) // se la Rana è gia target allora esci
        {
            break;
        } 
        // dico alla Rana di terminare
        int err = impostaThreadTarget(gameData->allTCB->tcb_rana, semaforoTCB);
        if (err)
        {
            perror("Imposta Rana Target FAILED ");
            break;
        }
        // stampo la rana sopra il fiume
        PipeData newPosAbsRana;
        newPosAbsRana.x = gameData->pipeData.x + gameData->ranaAbsPos.x;
        newPosAbsRana.y = gameData->pipeData.y + gameData->ranaAbsPos.y;
        
        // normale aggiornamento
        aggiornaOggettoNew_2(gameData, newPosAbsRana, &(gameData->oldPos.rana), S_RANA);
        gameData->ranaAbsPos.x = newPosAbsRana.x;
        gameData->ranaAbsPos.y = newPosAbsRana.y;

        sem_wait(semaforoWindow);
        stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
        refresh();                                     // Aggiorna la finestra
        sem_post(semaforoWindow);
        usleep(500000);
        
        if (gameData->gameInfo.vite > 0)
        { // decremento vite
            gameData->gameInfo.vite--;
            gameData->gameInfo.viteIsChanged = true;
        }

        break;
    }

    case RANA_LAVA:
    {
        ThreadControlBlock *tcb_rana = gameData->allTCB->tcb_rana;
        if (isThreadTarget(tcb_rana, semaforoTCB)) // se la Rana è gia target allora esci
        {
            break;
        } 
        // dico alla Rana di terminare
        int err = impostaThreadTarget(gameData->allTCB->tcb_rana, semaforoTCB);
        if (err)
        {
            perror("Imposta Rana Target FAILED ");
            break;
        }

        // stampo la rana sopra la lava
        PipeData newPosAbsRana;
        newPosAbsRana.x = gameData->pipeData.x + gameData->ranaAbsPos.x;
        newPosAbsRana.y = gameData->pipeData.y + gameData->ranaAbsPos.y;

        // normale aggiornamento
        aggiornaOggettoNew_2(gameData, newPosAbsRana, &(gameData->oldPos.rana), S_RANA);

        gameData->ranaAbsPos.x = newPosAbsRana.x;
        gameData->ranaAbsPos.y = newPosAbsRana.y;

        sem_wait(semaforoWindow);
        stampaMatrice(gameData->schermo.screenMatrix); // stampa sprite
        refresh();                                     // Aggiorna la finestra
        sem_post(semaforoWindow);
        usleep(500000);
        /**/

        if (gameData->gameInfo.vite > 0)
        { // decremento vite
            gameData->gameInfo.vite--;
            gameData->gameInfo.viteIsChanged = true;
        }

        break;
    }

    case RANA_TANA_APERTA:
    {
        
        ThreadControlBlock *tcb_rana = gameData->allTCB->tcb_rana; // recupera ThreadControlBlock della Rana
        if (isThreadTarget(tcb_rana, semaforoTCB)) // se la Rana è gia target allora esci
        {
            break;
        } 
        // dico alla Rana di terminare
        int err = impostaThreadTarget(gameData->allTCB->tcb_rana, semaforoTCB);
        if (err)
        {
            perror("Imposta Rana Target FAILED ");
            break;
        }
        // stampo la rana sopra la tana aperta
        PipeData newPosAbsRana;
        newPosAbsRana.x = gameData->pipeData.x + gameData->ranaAbsPos.x;
        newPosAbsRana.y = gameData->pipeData.y + gameData->ranaAbsPos.y;

        // normale aggiornamento
        aggiornaOggettoNew_2(gameData, newPosAbsRana, &(gameData->oldPos.rana), S_RANA);
        gameData->ranaAbsPos.x = newPosAbsRana.x;
        gameData->ranaAbsPos.y = newPosAbsRana.y;

        sem_wait(semaforoWindow);
        stampaMatrice(gameData->schermo.screenMatrix); // stampa sprite
        refresh();                                     // Aggiorna la finestra
        sem_post(semaforoWindow);
        usleep(500000);

        gameData->ranaAbsPos.on_coccodrillo = false; // resetta info su Rana e Coccodrillo
        gameData->ranaAbsPos.id_coccodrillo = -1;

        // modifico dati manche e punteggio
        if (false == gameData->gameInfo.mancheIsChanged) 
        {
            gameData->gameInfo.manche++;
            gameData->gameInfo.mancheIsChanged = true;
            gameData->gameInfo.punteggio = gameData->gameInfo.punteggio + 10;
            gameData->gameInfo.punteggioIsChanged = true;
        }
        else
        {
            break;
        }

        // stampo tana chiusa
        stampaTanaChiusa(gameData->tane[collisione.id_oggetto_passivo], gameData);
        gameData->tane[collisione.id_oggetto_passivo].status = TANA_CLOSED;
        break;
    }

    case RANA_TANA_CHIUSA:
    {
        ThreadControlBlock *tcb_rana = gameData->allTCB->tcb_rana; // recupera ThreadControlBlock della Rana
        if (isThreadTarget(tcb_rana, semaforoTCB)) // se la Rana è gia target allora esci
        {
            break;
        } 
        // dico alla Rana di terminare
        int err = impostaThreadTarget(gameData->allTCB->tcb_rana, semaforoTCB);
        if (err)
        {
            perror("Imposta Rana Target FAILED ");
            break;
        }

        // stampo la rana sopra la lava
        PipeData newPosAbsRana;
        newPosAbsRana.x = gameData->pipeData.x + gameData->ranaAbsPos.x;
        newPosAbsRana.y = gameData->pipeData.y + gameData->ranaAbsPos.y;

        // normale aggiornamento
        aggiornaOggettoNew_2(gameData, newPosAbsRana, &(gameData->oldPos.rana), S_RANA);

        gameData->ranaAbsPos.x = newPosAbsRana.x;
        gameData->ranaAbsPos.y = newPosAbsRana.y;

        sem_wait(semaforoWindow);
        stampaMatrice(gameData->schermo.screenMatrix); // stampa sprite
        refresh();                                     // Aggiorna la finestra
        sem_post(semaforoWindow);
        usleep(500000);
        /**/

        if (gameData->gameInfo.vite > 0)
        { // decremento vite
            gameData->gameInfo.vite--;
            gameData->gameInfo.viteIsChanged = true;
        }

        break;
    }

    case PROIETTILE_NEMICO_RANA: // Proiettile_Nemico ha colpito la Rana OK
    {

        if (collisione.oggetto_attivo == RANA_OBJ) // oggetto attivo = RANA
        {
            // recupero ThreadControlBlock della Rana e del ProiettileNemico
            ThreadControlBlock *tcb_rana = gameData->allTCB->tcb_rana; 
            ThreadControlBlock *tcb_proiettileNemico = &(gameData->allTCB->tcb_proiettili_nemici[collisione.id_oggetto_passivo]);
            pthread_t tid_rana = leggiTid(tcb_rana, semaforoTCB);
            pthread_t tid_proiettileNemico = leggiTid(tcb_proiettileNemico, semaforoTCB);
            // controllo di validità sui tid
            if (tid_rana == 0)
            {
                break;
            }
            if (tid_proiettileNemico == 0)
            {
                break;
            }
            // se uno dei due è già target, c'è gia una collisione ed esci
            if (isThreadTarget(tcb_proiettileNemico, semaforoTCB))
            {
                break;
            }
            if (isThreadTarget(tcb_rana, semaforoTCB))
            {
                break;
            }
            if (impostaThreadTarget(tcb_proiettileNemico, &semafori->tcb_mutex) != 0) // fa terminare il proiettile nemico
            {
                perror("ERRORE imposta thread Target");
                break;
            }
            

            if (impostaThreadTarget(tcb_rana, &semafori->tcb_mutex) != 0) // fa terminare la RANA
            {
                perror("ERR: Imposta Rana Target Fallito");
                break;
            }
            else
            {
                if (gameData->gameInfo.vite > 0)
                { // decremento vite e manche
                    gameData->gameInfo.vite--;
                    gameData->gameInfo.viteIsChanged = true;
                    gameData->gameInfo.manche--;
                    gameData->gameInfo.mancheIsChanged = true;
                }
            }
        }
        else // oggetto attivo = PRoiettile_Nemico
        {
            /* Fa terminare la Rana, 
                Fa terminare il Proiettile_Nemico 
                Decrementa vite e manche    
            */
            // Recupero i ThreadControlBlock di Rana e ProiettileNemico
            ThreadControlBlock *tcb_proiettileNemico = &(gameData->allTCB->tcb_proiettili_nemici[collisione.id_oggetto_attivo]);
            ThreadControlBlock *tcb_rana = gameData->allTCB->tcb_rana;

            pthread_t tid_rana = leggiTid(tcb_rana, semaforoTCB);
            pthread_t tid_proiettileNemico = leggiTid(tcb_proiettileNemico, semaforoTCB);
            // controllo sui tid. se hai letto dei thread gia chiusi, esci
            if (tid_rana == 0)
            {
                break;
            }
            if (tid_proiettileNemico == 0)
            {
                break;
            }
            // se uno dei due è già target la collisione c'è già stata ed esci
            if (isThreadTarget(tcb_proiettileNemico, &semafori->tcb_mutex))
            {
                break;
            }
            if (isThreadTarget(tcb_rana, &semafori->tcb_mutex))
            {
                break;
            }

            if (impostaThreadTarget(tcb_proiettileNemico, &semafori->tcb_mutex) != 0) // fa terminare il proiettile nemico
            {
                perror("ERRORE imposta thread Target");
                break;
            }
            
            if (impostaThreadTarget(tcb_rana, &semafori->tcb_mutex) != 0) // fa terminare la RANA
            {
                perror("ERR: Imposta Rana Target Fallito");
                break;
            }
            else
            {
                cancellaOggettoDaMatrice(gameData, &gameData->oldPos.rana, &gameData->oldPos.rana, S_RANA); // cancella sprite RANA

                if (gameData->gameInfo.vite > 0)
                { // decremento vite
                    gameData->gameInfo.vite--;
                    gameData->gameInfo.viteIsChanged = true;
                }
                /**/
            }
        }

        break;
    }
    case PROIETTILE_RANA_NEMICO: // Proiettile_Rana ha colpito il Nemico
    {
        /* Il proiettile è l'oggetto attivo , il nemico_pianta è l'oggetto passivo della collisione */
        // PipeData proiettile = gameData->pipeData;

        if (gameData->pipeData.id != collisione.id_oggetto_attivo)
        {
            perror("ID proiettileRana non coincide");
        }
        ThreadControlBlock *tcb_nemico = &(gameData->allTCB->tcb_piante[collisione.id_oggetto_passivo]);
        ThreadControlBlock *tcb_proiettile = &(gameData->allTCB->tcb_proiettili[collisione.id_oggetto_attivo]);

        /* controlli di routine , evitano doppie collisioni */
        if (isThreadTarget(tcb_nemico, &(semafori->tcb_mutex)))
        {
            PipeData *nemico = &(gameData->oldPos.nemici[collisione.id_oggetto_passivo]);
            cancellaOggettoDaMatrice(gameData, nemico, gameData->oldPos.nemici, S_PIANTA);
            break;
        }
        if (isThreadTarget(tcb_proiettile, &(semafori->tcb_mutex)))
        { 
            break;
        }

        /* Imposto i thread proiettile e nemico_pianta per terminare */
        if (0 != impostaThreadTarget(tcb_nemico, &(semafori->tcb_mutex)))
        {
            perror("ERRORE imposta thread Target");
            break;
        }
        if (0 != impostaThreadTarget(tcb_proiettile, &(semafori->tcb_mutex)))
        {
            perror("ERRORE imposta thread Target");
            break;
        }

        gameData->gameInfo.punteggio += 5;
        gameData->gameInfo.punteggioIsChanged = true;
        break;
    }
    case PROIETTILE_RANA_PROIETTILE_NEMICO: // Proiettile_Rana ha colpito Proiettile_Nemico
    {                                       
        // oggetto attivo = proiettileRana. oggetto passivo = proiettileNemico
        // su gameData->pipeData è scritto ProiettileRana

        ThreadControlBlock *tcb_proiettileRana = &(gameData->allTCB->tcb_proiettili[collisione.id_oggetto_attivo]);
        ThreadControlBlock *tcb_proiettileNemico = &(gameData->allTCB->tcb_proiettili_nemici[collisione.id_oggetto_passivo]);

        pthread_t tid_proiettile = leggiTid(tcb_proiettileRana, semaforoTCB);
        if (tid_proiettile == 0) // il TCB del proiettile è gia stato resettato
        {
            cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
            break;
        }

        if (isThreadTarget(tcb_proiettileRana, semaforoTCB))
        {
            if (isThreadTerminated(tcb_proiettileRana, semaforoTCB)) // ultima scrittura
            {
                cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
                break;
            }
        }

        if (isThreadTarget(tcb_proiettileNemico, semaforoTCB))
        {
            if (isThreadTerminated(tcb_proiettileNemico, semaforoTCB)) // ultima scrittura
            {
                PipeData *proiettileNemico = &(gameData->oldPos.proiettiliNemici[collisione.id_oggetto_passivo]);
                cancellaOggettoDaMatrice(gameData, proiettileNemico, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);

                break;
            }
        }
        
        // dico ai thread di terminare
        impostaThreadTarget(tcb_proiettileRana, semaforoTCB);
        impostaThreadTarget(tcb_proiettileNemico, semaforoTCB);

        break;
    }

    case PROIETTILE_COCCODRILLO_CATTIVO: // Proiettile_Rana ha colpito Coccodrillo Cattivo
    {
        // il proiettile muore e il coccodrillo diventa buono
        // l'oggetto attivo è il proiettile
        if (collisione.oggetto_attivo == PROIETTILE_OBJ)
        {
            // uccide il processo proiettile
            ThreadControlBlock *tcb_proiettile = &(gameData->allTCB->tcb_proiettili[collisione.id_oggetto_attivo]);
            pthread_t tid_proiettile = leggiTid(tcb_proiettile, semaforoTCB);

            if (tid_proiettile == 0) // collisione rilevata da thread gia chiuso e tcb resettato
            {
                cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE); //cancella sprite
                break;
            }

            if (isThreadTarget(tcb_proiettile, semaforoTCB))
            {
                if (isThreadTerminated(tcb_proiettile, semaforoTCB))
                {
                    cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE); //cancella sprite
                }
                break;
            }

            impostaThreadTarget(tcb_proiettile, semaforoTCB);

            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            
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
        break;
    }

    case PROIETTILE_COCCODRILLO_BUONO: // proiettile Rana collide con Coccodrillo Buono
    {
        /*  distruggo il proiettile e basta  */
        if (collisione.oggetto_attivo == PROIETTILE_OBJ)
        {
            ThreadControlBlock *tcb_proiettile = &(gameData->allTCB->tcb_proiettili[collisione.id_oggetto_attivo]);
            pthread_t tid_proiettile = leggiTid(tcb_proiettile, semaforoTCB);
            if (tid_proiettile == 0)
            {
                // cancellando la sprite evita altre collisioni
                cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
                break;
            }
            if (isThreadTarget(tcb_proiettile, semaforoTCB))
            {
                if (isThreadTerminated(tcb_proiettile, semaforoTCB))
                {
                    break;
                }
            }
            impostaThreadTarget(tcb_proiettile, semaforoTCB); // dice al thread di terminare
        }
        
        break;
    }

    case PROIETTILE_NEMICO_PROIETTILE_RANA: // proiettileNemico ha colpito proiettileRana
    {                                       
        // oggetto attivo = proiettileNemico. oggetto passivo = proiettileRana
        // su gameData->pipData è scritto il proiettileNemico !!

        // recupero ThreadControlBlock del ProiettileRana e ProiettileNemico
        ThreadControlBlock *tcb_proiettileRana = &(gameData->allTCB->tcb_proiettili[collisione.id_oggetto_passivo]);
        ThreadControlBlock *tcb_proiettileNemico = &(gameData->allTCB->tcb_proiettili_nemici[collisione.id_oggetto_attivo]);

        if (isThreadTarget(tcb_proiettileNemico, semaforoTCB)) // già impostato per terminare
        {
            if (isThreadTerminated(tcb_proiettileNemico, semaforoTCB)) // ha fatto l' ultima scrittura
            {
                cancellaOggetto(gameData, gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);// cancella sprite proiettileNemico
                break;
            }
        }

        if (isThreadTarget(tcb_proiettileRana, semaforoTCB)) // già impostato per terminare
        {
            if (isThreadTerminated(tcb_proiettileRana, semaforoTCB)) // ultima scrittura
            {
                PipeData *proiettile = &(gameData->oldPos.proiettili[collisione.id_oggetto_passivo]); // recupera PipeData del proiettileRana
                cancellaOggettoDaMatrice(gameData, proiettile, gameData->oldPos.proiettili, S_PROIETTILE); // cancella sprite
                break;
            }
        }
        // dice hai thread di terminare
        impostaThreadTarget(tcb_proiettileNemico, semaforoTCB);
        impostaThreadTarget(tcb_proiettileRana, semaforoTCB);

        break;
    }

    case RANA_COCCODRILLO_BUONO: // Rana è sul Coccodrillo Buono
    {
        if (collisione.oggetto_attivo == RANA_OBJ)
        {
            // stampo il coccodrillo buono
            if (gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].direction == 1)
            {
                aggiornaOggettoNew(gameData, gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo], gameData->oldPos.coccodrilli, S_COCCODRILLO_DX);
            }
            else
            {
                aggiornaOggettoNew(gameData, gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo], gameData->oldPos.coccodrilli, S_COCCODRILLO_SX);
            }

            // stampo sopra il coccodrillo buono la rana
            // aggiorno abs pos rana
            gameData->ranaAbsPos.x = collisione.hit_point_x;
            gameData->ranaAbsPos.y = collisione.hit_point_y;
            gameData->ranaAbsPos.on_coccodrillo = true;
            gameData->ranaAbsPos.id_coccodrillo = collisione.id_oggetto_passivo;
            // aggiorno offset rana su coccodrillo
            gameData->ranaAbsPos.offset_on_coccodrillo = collisione.hit_point_x - gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo].x;
            // stampo e aggiorno rana
            PipeData ranaData;
            ranaData.x = gameData->ranaAbsPos.x;
            ranaData.type = 'X';
            ranaData.y = gameData->ranaAbsPos.y;
            ranaData.id = 0;
            aggiornaOggettoNew(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);
        }
        else
        {
            // l'oggetto attivo è il coccodrillo buono
            // stampo il coccodrillo buono
            if (gameData->controlloCoccodrilli[gameData->pipeData.id].direction == 1)
            {
                aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX);
            }
            else
            {
                aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX);
            }
            // aggiorno abs pos rana
            gameData->ranaAbsPos.x = collisione.hit_point_x;
            gameData->ranaAbsPos.y = collisione.hit_point_y;
            gameData->ranaAbsPos.on_coccodrillo = true;
            gameData->ranaAbsPos.id_coccodrillo = collisione.id_oggetto_attivo;
            // aggiorno offset rana su coccodrillo
            gameData->ranaAbsPos.offset_on_coccodrillo = collisione.hit_point_x - gameData->oldPos.coccodrilli[collisione.id_oggetto_attivo].x;
            // stampo e aggiorno rana
            PipeData ranaData;
            ranaData.x = gameData->ranaAbsPos.x;
            ranaData.type = 'X';
            ranaData.y = gameData->ranaAbsPos.y;
            ranaData.id = 0;
            // stampo sopra il coccodrillo buono la rana
            aggiornaOggettoNew(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);

            /**/
        }
        break;
    }

    case RANA_COCCODRILLO_CATTIVO: // Rana è sul Coccodrillo Cattivo
    {
        if (collisione.oggetto_attivo == RANA_OBJ)
        { // oggetto attivo=Rana. oggetto passivo=Coccodrillo
            // stampo il coccodrillo cattivo
            if (gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].direction == 1)
            {
                aggiornaOggettoNew(gameData, gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo], gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C);
            }
            else
            {
                aggiornaOggettoNew(gameData, gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo], gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C);
            }

            // stampo sopra il coccodrillo buono la rana
            // aggiorno abs pos rana
            gameData->ranaAbsPos.x = collisione.hit_point_x;
            gameData->ranaAbsPos.y = collisione.hit_point_y;
            gameData->ranaAbsPos.on_coccodrillo = true;
            gameData->ranaAbsPos.id_coccodrillo = collisione.id_oggetto_passivo;
            // aggiorno offset rana su coccodrillo
            gameData->ranaAbsPos.offset_on_coccodrillo = collisione.hit_point_x - gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo].x;
            // stampo e aggiorno rana
            PipeData ranaData;
            ranaData.x = gameData->ranaAbsPos.x;
            ranaData.type = 'X';
            ranaData.y = gameData->ranaAbsPos.y;
            ranaData.id = 0;
            aggiornaOggettoNew_2(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);
        }
        else
        {

            // l'oggetto attivo=coccodrillo cattivo
            // stampo il coccodrillo cattivo
            if (gameData->controlloCoccodrilli[gameData->pipeData.id].direction == 1)
            {
                aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C);
            }
            else
            {
                aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C);
            }
            // aggiorno abs pos rana
            gameData->ranaAbsPos.x = collisione.hit_point_x;
            gameData->ranaAbsPos.y = collisione.hit_point_y;
            gameData->ranaAbsPos.on_coccodrillo = true;
            gameData->ranaAbsPos.id_coccodrillo = collisione.id_oggetto_attivo;
            // aggiorno offset rana su coccodrillo
            gameData->ranaAbsPos.offset_on_coccodrillo = collisione.hit_point_x - gameData->oldPos.coccodrilli[collisione.id_oggetto_attivo].x;
            // stampo e aggiorno rana
            PipeData ranaData;
            ranaData.x = gameData->ranaAbsPos.x;
            ranaData.type = 'X';
            ranaData.y = gameData->ranaAbsPos.y;
            ranaData.id = 0;
            // stampo la rana sopra il coccodrillo cattivo
            aggiornaOggettoNew_2(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);

            /**/
        }
        break;
    }

    case RANA_MARCIAPIEDE:
    {
        // aggiorno abs pos rana, pipeData dovrebbe essere la RANA
        gameData->ranaAbsPos.x = gameData->ranaAbsPos.x + gameData->pipeData.x;
        gameData->ranaAbsPos.y = gameData->ranaAbsPos.y + gameData->pipeData.y;
        gameData->ranaAbsPos.on_coccodrillo = false;
        gameData->ranaAbsPos.id_coccodrillo = -1;
        // aggiorno offset rana su coccodrillo
        gameData->ranaAbsPos.offset_on_coccodrillo = -1;
        // stampo e aggiorno rana
        PipeData ranaData;
        ranaData.x = gameData->ranaAbsPos.x;
        ranaData.y = gameData->ranaAbsPos.y;
        ranaData.type = 'X';
        ranaData.id = 0;
        aggiornaOggettoNew_2(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);
        beep();
        break;
    }

    case RANA_ARGINE:
    {
        // aggiorno abs pos rana, pipeData dovrebbe essere la RANA
        gameData->ranaAbsPos.x = gameData->ranaAbsPos.x + gameData->pipeData.x;
        gameData->ranaAbsPos.y = gameData->ranaAbsPos.y + gameData->pipeData.y;
        gameData->ranaAbsPos.on_coccodrillo = false;
        gameData->ranaAbsPos.id_coccodrillo = -1;
        // aggiorno offset rana su coccodrillo a valore non valido
        gameData->ranaAbsPos.offset_on_coccodrillo = -1;
        // stampo e aggiorno rana
        PipeData ranaData;
        ranaData.x = gameData->ranaAbsPos.x;
        ranaData.y = gameData->ranaAbsPos.y;
        ranaData.type = 'X';
        ranaData.id = 0;
        aggiornaOggettoNew_2(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);
        break;
    }

    case RANA_NEMICO: // Rana collide con NemicoPianta
    {
        /* La Rana muore e perde la manche */
        if (collisione.oggetto_attivo == RANA_OBJ) // oggetto attivo = RANA
        {
            /*  Manca teminazione thread Nemico ?
             */
            ThreadControlBlock *tcb_rana = gameData->allTCB->tcb_rana;
            pthread_t tid_rana = leggiTid(tcb_rana, semaforoTCB);
            if (tid_rana == 0)
            {
                break;
            }
            if (isThreadTarget(tcb_rana, semaforoTCB))
            {
                break;
            }
            if (impostaThreadTarget(tcb_rana, &semafori->tcb_mutex) != 0) // fa terminare la RANA
            {
                perror("ERR: Imposta Rana Target Fallito");
                break;
            }
            else
            {
                if (gameData->gameInfo.vite > 0)
                { // decremento vite
                    gameData->gameInfo.vite--;
                    gameData->gameInfo.viteIsChanged = true;
                    gameData->gameInfo.manche--;
                    gameData->gameInfo.mancheIsChanged = true;
                }
            }

        }
        break;
    }

    default:
        break;
    }
    return;
}