#include "../hdr/collisione.h"

Collisione detectCollisione(GameData *gameData)
{
    Collisione collisione;

    setCollision(&collisione, NO_COLLISIONE, NO_OBJ, NOID, NO_OBJ, NOID, NOPOS, NOPOS);

    Schermo *schermo = &(gameData->schermo); // lo schermo di gioco

    // switch sul carattere in pipe
    switch (gameData->pipeData.type)
    {
    case 'X': // rana
    {
        // calcolo posizone assoluta della rana in potenza
        // ovvero sommo a abspos la posizione relativa in pipe
        RanaAbsPos ranaPos;
        ranaPos.x = gameData->ranaAbsPos.x + gameData->pipeData.x;
        ranaPos.y = gameData->ranaAbsPos.y + gameData->pipeData.y;

        // se il movimento della rana non è lecito non c'è aggiornamento ne collisione
        if (!isFrogMoveLecit(ranaPos.x, ranaPos.y, gameData->ranaAbsPos, gameData->pipeData) ||
            (gameData->ranaAbsPos.on_coccodrillo && gameData->pipeData.x != 0))
        {
            return collisione;
        }

        for (int row = ranaPos.y; row < ranaPos.y + RANA_H; row++)
        {
            for (int col = ranaPos.x; col < ranaPos.x + RANA_W; col++)
            {
                switch (schermo->screenMatrix[row][col].tipo)
                {
                case FIUME_OBJ:
                    setCollision(&collisione, RANA_FIUME, RANA_OBJ, gameData->pipeData.id, FIUME_OBJ, 0, col, row);
                    return collisione;
                case LAVA_OBJ:
                    setCollision(&collisione, RANA_LAVA, RANA_OBJ, gameData->pipeData.id, LAVA_OBJ, 0, col, row);
                    return collisione;
                case TANA_OPEN_OBJ:
                    setCollision(&collisione, RANA_TANA_APERTA, RANA_OBJ, gameData->pipeData.id, TANA_OPEN_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    break;
                case TANA_CLOSE_OBJ:
                    setCollision(&collisione, RANA_TANA_CHIUSA, RANA_OBJ, gameData->pipeData.id, TANA_CLOSE_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    return collisione;
                case COCCODRILLO_BUONO_OBJ:
                    setCollision(&collisione, RANA_COCCODRILLO_BUONO, RANA_OBJ, gameData->pipeData.id, COCCODRILLO_BUONO_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    break; // la rana deve stare tutta sul coccodrillo per questo no return ma break
                case PN_OBJ:
                    setCollision(&collisione, PROIETTILENEMICO_RANA, RANA_OBJ, gameData->pipeData.id, PN_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    return collisione;
                case N_OBJ:
                    setCollision(&collisione, NEMICO_RANA, RANA_OBJ, gameData->pipeData.id, N_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    return collisione;
                case COCCODRILLO_CATTIVO_OBJ:
                    setCollision(&collisione, RANA_COCCODRILLO_CATTIVO, RANA_OBJ, gameData->pipeData.id, COCCODRILLO_CATTIVO_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    break; // la rana deve stare tutta sul coccodrillo per questo no return ma break
                case MARCIAPIEDE_OBJ:
                    setCollision(&collisione, RANA_MARCIAPIEDE, RANA_OBJ, gameData->pipeData.id, MARCIAPIEDE_OBJ, 0, col, row);
                    return collisione;
                case ARGINE_OBJ:
                    setCollision(&collisione, RANA_ARGINE, RANA_OBJ, gameData->pipeData.id, ARGINE_OBJ, 0, col, row);
                    break;
                default:
                    break;
                }
            }
        }

        // todo forse questo va messo anche per altri tipi di collisione
        if (collisione.tipoCollisione == RANA_COCCODRILLO_BUONO || collisione.tipoCollisione == RANA_COCCODRILLO_CATTIVO)
        {
            // modifico il punto di hit
            collisione.hit_point_x -= 2;
            collisione.hit_point_y -= 1;
        }
        break;
    }
    case 'P': // proiettile rana
    {
        PipeData proiettileData = gameData->pipeData;

        for (int row = proiettileData.y; row < proiettileData.y + PROIETTILE_H; row++)
        {
            for (int col = proiettileData.x; col < proiettileData.x + PROIETTILE_W; col++)
            {
                switch (schermo->screenMatrix[row][col].tipo)
                {
                case COCCODRILLO_CATTIVO_OBJ:
                    setCollision(&collisione, PROIETTILE_COCCODRILLO_CATTIVO, P_OBJ, proiettileData.id, COCCODRILLO_CATTIVO_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    return collisione;
                case N_OBJ:
                    setCollision(&collisione, PROIETTILE_NEMICO, P_OBJ, proiettileData.id, N_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    return collisione;
                case PN_OBJ:
                    setCollision(&collisione, PROIETTILE_PROIETTILENEMICO, P_OBJ, proiettileData.id, PN_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    return collisione;
                case COCCODRILLO_BUONO_OBJ:
                    setCollision(&collisione, PROIETTILE_COCCODRILLO_BUONO, P_OBJ, proiettileData.id, COCCODRILLO_BUONO_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    return collisione;
                default:
                    break;
                }
            }
        }
        break;
    }
    case 'p': // proiettile nemico
    {
        PipeData proiettileData = gameData->pipeData;

        for (int row = proiettileData.y; row < proiettileData.y + PROIETTILE_H; row++)
        {
            for (int col = proiettileData.x; col < proiettileData.x + PROIETTILE_W; col++)
            {
                switch (schermo->screenMatrix[row][col].tipo)
                {
                case RANA_OBJ:
                    setCollision(&collisione, PROIETTILENEMICO_RANA, PN_OBJ, proiettileData.id, RANA_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    return collisione;
                case P_OBJ:
                    setCollision(&collisione, PROIETTILE_PROIETTILENEMICO, PN_OBJ, proiettileData.id, P_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    return collisione;
                default:
                    break;
                }
            }
        }
        break;
    }
    case 'C': // coccodrillo a destra
    case 'c': // coccodrillo a sinistra
    {
        PipeData coccodrilloData = gameData->pipeData;

        if (!(gameData->controlloCoccodrilli[coccodrilloData.id].is_buono))
        {
            for (int row = coccodrilloData.y; row < coccodrilloData.y + COCCODRILLO_H; row++)
            {
                for (int col = coccodrilloData.x; col < coccodrilloData.x + COCCODRILLO_W; col++)
                {
                    switch (schermo->screenMatrix[row][col].tipo)
                    {
                    case P_OBJ:
                        setCollision(&collisione, PROIETTILE_COCCODRILLO_CATTIVO, COCCODRILLO_CATTIVO_OBJ, coccodrilloData.id, P_OBJ, schermo->screenMatrix[row][col].id, col, row);
                        return collisione;
                    default:
                        break;
                    }
                }
            }
        }
        else
        {
            for (int row = coccodrilloData.y; row < coccodrilloData.y + COCCODRILLO_H; row++)
            {
                for (int col = coccodrilloData.x; col < coccodrilloData.x + COCCODRILLO_W; col++)
                {

                    switch (schermo->screenMatrix[row][col].tipo)
                    {
                    case P_OBJ:
                        setCollision(&collisione, PROIETTILE_COCCODRILLO_BUONO, COCCODRILLO_BUONO_OBJ, coccodrilloData.id, P_OBJ, schermo->screenMatrix[row][col].id, col, row);
                        return collisione;
                    default:
                        break;
                    }
                }
            }
        }
        break;
    }
    case 'n': // pianta
    {
        PipeData nemicoData = gameData->pipeData;

        for (int row = nemicoData.y; row < nemicoData.y + PIANTA_H; row++)
        {
            for (int col = nemicoData.x; col < nemicoData.x + PIANTA_W; col++)
            {
                switch (schermo->screenMatrix[row][col].tipo)
                {
                case RANA_OBJ:
                    setCollision(&collisione, NEMICO_RANA, N_OBJ, nemicoData.id, RANA_OBJ, schermo->screenMatrix[row][col].id, col, row);
                    return collisione;
                default:
                    break;
                }
            }
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return collisione;
}

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

void handleCollisione(GameData *gameData, Collisione collisione)
{
    switch (collisione.tipoCollisione)
    {
    case RANA_FIUME:
    {
        // stampo la rana sopra il fiume
        printRana2(gameData);

        refresh();
        usleep(100000);

        // imposto rana is dead a true
        gameData->gameInfo.ranaIsDead = true;
        break;
    }
    case RANA_LAVA:
    {
        // stampo la rana sopra la lava
        printRana2(gameData);

        refresh();
        usleep(100000);

        // imposto rana is dead a true
        gameData->gameInfo.ranaIsDead = true;
        break;
    }
    case RANA_TANA_APERTA:
    {
        // stampo la rana sopra la tana aperta
        printRana2(gameData);

        refresh();
        usleep(100000);

        // stampo tana chiusa
        stampaTanaChiusa(gameData->tane[collisione.id_oggetto_passivo], gameData);
        gameData->gameInfo.punteggio += 10;

        gameData->gameInfo.mancheWin = true;
        break;
    }
    case RANA_TANA_CHIUSA:
    {
        // stampo la rana sopra la tana chiusa
        printRana2(gameData);

        refresh();
        usleep(100000);
        // imposto rana is dead a true
        gameData->gameInfo.ranaIsDead = true;
        break;
    }
    case PROIETTILE_COCCODRILLO_CATTIVO:
    {
        // il proiettile muore e il coccodrillo diventa buono
        // l'oggetto attivo è il proiettile
        if (collisione.oggetto_attivo == P_OBJ)
        {
            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_attivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);

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
            // l'oggetto attivo è il tronco
            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_passivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);

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
        }
        gameData->contatori.contProiettili--;
        gameData->gameInfo.punteggio += 2;
        break;
    }
    case RANA_COCCODRILLO_BUONO:
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
            setPipeData(&ranaData, 'X', 0, gameData->ranaAbsPos.x, gameData->ranaAbsPos.y);
            aggiornaOggettoNew(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);
        }
        else
        {
            // l'oggetto attivo e il coccodrillo buono
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
            setPipeData(&ranaData, 'X', 0, gameData->ranaAbsPos.x, gameData->ranaAbsPos.y);
            // stampo sopra il coccodrillo buono la rana
            aggiornaOggettoNew(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);
        }
        gameData->gameInfo.punteggio += 2;
        break;
    }
    case PROIETTILE_NEMICO:
    {
        if (collisione.oggetto_attivo == P_OBJ)
        {
            // uccisione del proiettile amico che ha colpito il nemico
            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_attivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
            gameData->contatori.contProiettili--;

            // uccisione del nemico (pianta)
            uccidiNemico(gameData->pids.pidNemici, collisione.id_oggetto_passivo);
            // cancella oggetto nemico
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.nemici[collisione.id_oggetto_passivo], gameData->oldPos.nemici, S_PIANTA);
            gameData->contatori.contNemici--;
            gameData->gameInfo.punteggio += 5;
        }

        break;
    }
    case PROIETTILENEMICO_RANA:
    {
        if (collisione.oggetto_attivo == RANA_OBJ)
        {
            // l'oggetto attivo è la rana
            printRana2(gameData);

            refresh();
            usleep(100000);

            // imposto rana is dead a true
            gameData->gameInfo.ranaIsDead = true;

            // uccido il proiettile nemico
            uccidiProiettileNemico(gameData->pids.pidProiettiliNemici, collisione.id_oggetto_passivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.proiettiliNemici[collisione.id_oggetto_passivo], gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
        }
        else
        {
            // l'oggetto attivo è il proiettile nemico
            // uccisione del proiettile nemico
            uccidiProiettileNemico(gameData->pids.pidProiettiliNemici, collisione.id_oggetto_attivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.proiettiliNemici[collisione.id_oggetto_attivo], gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);

            // imposto rana is dead a true
            gameData->gameInfo.ranaIsDead = true;
        }
        gameData->contatori.contProiettiliN--;
        break;
    }
    case PROIETTILE_PROIETTILENEMICO:
    {
        if (collisione.oggetto_attivo == P_OBJ)
        {
            // il proiettile amivo è l'oggetto attivo
            // uccido il proiettile amico
            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_attivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);

            // uccido il proiettile nemico
            uccidiProiettileNemico(gameData->pids.pidProiettiliNemici, collisione.id_oggetto_passivo); // uccide il processo proiettile
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.proiettiliNemici[collisione.id_oggetto_passivo], gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
        }
        else
        {
            // il proiettile nemico è l'oggetto attivo
            // uccido il proiettile nemico
            uccidiProiettileNemico(gameData->pids.pidProiettiliNemici, collisione.id_oggetto_attivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.proiettiliNemici[collisione.id_oggetto_attivo], gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);

            // uccido il proiettile amico
            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_passivo);
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.proiettili[collisione.id_oggetto_passivo], gameData->oldPos.proiettili, S_PROIETTILE);
        }
        gameData->contatori.contProiettili--;
        gameData->contatori.contProiettiliN--;
        gameData->gameInfo.punteggio += 1;
        break;
    }
    case NEMICO_RANA:
    {
        if (collisione.oggetto_attivo == RANA_OBJ)
        {
            // la rana è l'oggetto attivo uccido la rana
            gameData->gameInfo.ranaIsDead = true;

            // reprint del nemico
            stampaSpriteInMatrice(&(gameData->oldPos.nemici[collisione.id_oggetto_passivo]), &(gameData->sprites[S_PIANTA]), gameData);
        }
        else
        {
            // la pianta è l'oggetto attivo uccido la pianta ma non la rana
            uccidiNemico(gameData->pids.pidNemici, collisione.id_oggetto_attivo);
            gameData->oldPos.nemici[collisione.id_oggetto_attivo].id = NOID;
            gameData->gameInfo.punteggio += 5;
        }
        break;
    }
    case RANA_COCCODRILLO_CATTIVO:
    {
        if (collisione.oggetto_attivo == RANA_OBJ)
        {
            // stampo il coccodrillo cattivo
            if (gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].direction == 1)
            {
                aggiornaOggettoNew(gameData, gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo], gameData->oldPos.coccodrilli, S_COCCODRILLO_DX_C);
            }
            else
            {
                aggiornaOggettoNew(gameData, gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo], gameData->oldPos.coccodrilli, S_COCCODRILLO_SX_C);
            }

            // stampo sopra il coccodrillo cattivo la rana
            // aggiorno abs pos rana
            gameData->ranaAbsPos.x = collisione.hit_point_x;
            gameData->ranaAbsPos.y = collisione.hit_point_y;
            gameData->ranaAbsPos.on_coccodrillo = true;
            gameData->ranaAbsPos.id_coccodrillo = collisione.id_oggetto_passivo;
            // aggiorno offset rana su coccodrillo
            gameData->ranaAbsPos.offset_on_coccodrillo = collisione.hit_point_x - gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo].x;
            // stampo e aggiorno rana
            PipeData ranaData;
            setPipeData(&ranaData, 'X', 0, gameData->ranaAbsPos.x, gameData->ranaAbsPos.y);
            aggiornaOggettoNew(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);
        }
        else
        {
            // l'oggetto attivo e il coccodrillo buono
            // stampo il coccodrillo buono
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
            setPipeData(&ranaData, 'X', 0, gameData->ranaAbsPos.x, gameData->ranaAbsPos.y);
            // stampo sopra il coccodrillo buono la rana
            aggiornaOggettoNew(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);
        }
        gameData->gameInfo.punteggio += 2;
        break;
    }
    case RANA_MARCIAPIEDE:
    {
        if (gameData->ranaAbsPos.on_coccodrillo)
        {
            gameData->ranaAbsPos.on_coccodrillo = false;
            gameData->ranaAbsPos.id_coccodrillo = -1;
            gameData->ranaAbsPos.offset_on_coccodrillo = 0;
        }
        // stampo la rana sopra il coccodrillo
        printRana2(gameData);

        break;
    }
    case RANA_ARGINE:
    {
        if (gameData->ranaAbsPos.on_coccodrillo)
        {
            gameData->ranaAbsPos.on_coccodrillo = false;
            gameData->ranaAbsPos.id_coccodrillo = -1;
            gameData->ranaAbsPos.offset_on_coccodrillo = 0;
        }
        // stampo la rana sopra il coccodrillo
        printRana2(gameData);

        break;
    }
    case PROIETTILE_COCCODRILLO_BUONO:
    {
        // devo uccidere il proiettile amico
        if (collisione.id_oggetto_attivo == P_OBJ)
        {
            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_attivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
        }
        else
        {
            // l'oggetto attivo è il coccodrillo
            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_passivo);
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.proiettili[collisione.id_oggetto_passivo], gameData->oldPos.proiettili, S_PROIETTILE);
        }
        gameData->contatori.contProiettili--;
        break;
    }
    default:
    {
        break;
    }
    }
    return;
}
// print della rana in posizione
void printRana2(GameData *gameData)
{
    int newPosAbsRanaX = gameData->pipeData.x + gameData->ranaAbsPos.x;
    int newPosAbsRanaY = gameData->pipeData.y + gameData->ranaAbsPos.y;
    gameData->pipeData.x = newPosAbsRanaX;
    gameData->pipeData.y = newPosAbsRanaY;
    aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
    gameData->ranaAbsPos.x = gameData->pipeData.x;
    gameData->ranaAbsPos.y = gameData->pipeData.y;
    stampaMatrice(gameData->schermo.screenMatrix);
    return;
}