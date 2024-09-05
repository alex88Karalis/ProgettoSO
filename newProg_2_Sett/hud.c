
#include "hud.h"

void printVite(GameData* GameData){
    int vite = GameData->gameInfo.vite;
    Schermo* schermo = &(GameData->schermo);
    char rana[RANA_H][RANA_W]={
        {'\\','.','/'},
        {'/','-','\\'}
    };
    // pulizia sezione vite
    for(int row=VITEROWSTART;row<VITEROWSTART+RANA_H;row++){
        for(int col=VITESTARTCOL;col<VITESTARTCOL+VITE_W;col++){
            schermo->screenMatrix[row][col].ch = ' ';
			schermo->screenMatrix[row][col].color = SFONDO_COL;
			schermo->screenMatrix[row][col].id = 0;
			schermo->screenMatrix[row][col].tipo = HUD_OBJ;
            schermo->screenMatrix[row][col].is_changed=true;
        }
    }
    // stampa sezione vite
    int row=0,col=0;
    for(int n_vite=0;n_vite<vite;n_vite++){
        for(int rel_row=0;rel_row<RANA_H;rel_row++){
            row=VITEROWSTART+rel_row;
            for(int rel_col=0;rel_col<RANA_W;rel_col++){
                col=VITESTARTCOL+rel_col+(n_vite*(RANA_W+1));
                schermo->screenMatrix[row][col].ch = rana[rel_row][rel_col];
			    schermo->screenMatrix[row][col].color = RANA_COL;
			    schermo->screenMatrix[row][col].id = 0;
			    schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                schermo->screenMatrix[row][col].is_changed=true;
            }
        }
    }
    return;
}

void printInitTempo(GameData* gameData){
    Schermo* schermo = &(gameData->schermo);
    for(int row=TEMPOROWSTART;row < TEMPOROWSTART+TEMPO_H; row++){
        for(int col=TEMPOSTARTCOL;col<TEMPOSTARTCOL+TEMPO_W; col++){
            schermo->screenMatrix[row][col].ch = ' ';
			schermo->screenMatrix[row][col].color = TEMPO_COL;
			schermo->screenMatrix[row][col].id = 0;
			schermo->screenMatrix[row][col].tipo = HUD_OBJ;
        }
    }
    return;
}

void printTempo(GameData* gameData){
    Schermo* schermo = &(gameData->schermo);
    int secondi= gameData->gameInfo.tempo.secondi;
    int maxSeconds = 0; // nuermo massimo di secondi per manche a seconda del livello
    switch (gameData->gameInfo.livello)
    {
    case 1:
        maxSeconds= TEMPOLVL1;
        break;
    case 2:
        maxSeconds = TEMPOLVL2;
        break;
    case 3:
        maxSeconds = TEMPOLVL3;
        break;
    default:
        break;
    }

    int tempoInHudPassato = ((double)(TEMPO_W) / (double)maxSeconds ) * (double)secondi;
    if(secondi<=maxSeconds){
        for(int row=TEMPOROWSTART;row < TEMPOROWSTART+TEMPO_H; row++){
            for(int col=TEMPOSTARTCOL;col<TEMPOSTARTCOL+tempoInHudPassato; col++){
                schermo->screenMatrix[row][col].ch = ' ';
                schermo->screenMatrix[row][col].color = SFONDO_COL;
                schermo->screenMatrix[row][col].id = 0;
                schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                schermo->screenMatrix[row][col].is_changed = true;
            }
        }
    }
    return;
}

void printDefaultHudSup(GameData* gameData){
    // print default per livello e manche
    char defLvlManche[HUDSUP_H][LVLMANCHEDEF_W]={
        {'@',' ',' ',' ',' ','@',' ',' ','@',' ','@',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
        {'@',' ',' ',' ',' ','@',' ',' ','@',' ','@',' ',' ',' ',' ','@',' ',' ',' ',' ',' ',' ',' '},
        {'@',' ',' ',' ',' ','@',' ',' ','@',' ','@',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
        {'@',' ',' ',' ',' ','@',' ',' ','@',' ','@',' ',' ',' ',' ','@',' ',' ',' ',' ',' ',' ',' '},
        {'@','@','@','@',' ',' ','@','@',' ',' ','@','@','@','@',' ',' ',' ',' ',' ',' ',' ',' ','@'}
    };
    Schermo* schermo = &(gameData->schermo);
    int row=0,col=0;
    for(int rel_row=0;rel_row< HUDSUP_H;rel_row++){
        row = rel_row + HUDSUPLETTERROWSTART;
        for(int rel_col=0;rel_col<LVLMANCHEDEF_W;rel_col++){
            col = rel_col + HUDLETTERSUPCOLSX;
		    schermo->screenMatrix[row][col].ch = ' ';
			schermo->screenMatrix[row][col].color = defLvlManche[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			schermo->screenMatrix[row][col].id = 0;
			schermo->screenMatrix[row][col].tipo = HUD_OBJ;
        }
    }
    // print default per score
    char defScore[HUDSUP_H][SCOREDEF_W]={
        {'@','@','@','@',' ','@','@','@','@',' ','@','@','@','@',' ','@','@','@',' ',' ','@','@','@','@',' ',' '},
        {'@',' ',' ',' ',' ','@',' ',' ',' ',' ','@',' ',' ','@',' ','@',' ','@',' ',' ','@',' ',' ',' ',' ','@'},
        {'@','@','@','@',' ','@',' ',' ',' ',' ','@',' ',' ','@',' ','@','@','@',' ',' ','@','@','@',' ',' ',' '},
        {' ',' ',' ','@',' ','@',' ',' ',' ',' ','@',' ',' ','@',' ','@',' ','@',' ',' ','@',' ',' ',' ',' ','@'},
        {'@','@','@','@',' ','@','@','@','@',' ','@','@','@','@',' ','@',' ',' ','@',' ','@','@','@','@',' ',' '}
    };
    row=0,col=0;
    for(int rel_row=0;rel_row< HUDSUP_H;rel_row++){
        row = rel_row + HUDSUPLETTERROWSTART;
        for(int rel_col=0;rel_col<SCOREDEF_W;rel_col++){
            col = rel_col + SCOREDEFCOLSTART;
		    schermo->screenMatrix[row][col].ch = ' ';
			schermo->screenMatrix[row][col].color = defScore[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			schermo->screenMatrix[row][col].id = 0;
			schermo->screenMatrix[row][col].tipo = HUD_OBJ;
        }
    }
    
    return;
}

void printLvl(GameData* gameData){
    char uno[CIFRA_H][CIFRA_W]={
        {' ',' ','@','@'},
        {' ','@',' ','@'},
        {' ',' ',' ','@'},
        {' ',' ',' ','@'},
        {' ',' ',' ','@'}
    };
    char due[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {' ',' ',' ','@'},
        {' ','@','@',' '},
        {'@',' ',' ',' '},
        {'@','@','@','@'}
    };
    char tre[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {' ',' ',' ','@'},
        {' ','@','@','@'},
        {' ',' ',' ','@'},
        {'@','@','@','@'}
    };
    
    Schermo* schermo = &(gameData->schermo);
    // pulizia preliminare
    int row= HUDSUPLETTERROWSTART;
    int col= LVLSTARTCOL;
    for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
        row = rel_row + HUDSUPLETTERROWSTART;
        for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
            col= rel_col + LVLSTARTCOL;
            schermo->screenMatrix[row][col].ch = ' ';
			schermo->screenMatrix[row][col].color = SFONDO_COL;
			schermo->screenMatrix[row][col].id = 0;
			schermo->screenMatrix[row][col].tipo = HUD_OBJ;
            schermo->screenMatrix[row][col].is_changed=true;
        }
    }
    // print del livello
    row= HUDSUPLETTERROWSTART;
    col= LVLSTARTCOL;
    switch(gameData->gameInfo.livello){
        case 1:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + LVLSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = uno[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 2:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + LVLSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = due[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 3:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + LVLSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = tre[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        default:
            break;
    }
    return;
}

void printManche(GameData* gameData){
    char uno[CIFRA_H][CIFRA_W]={
        {' ',' ','@','@'},
        {' ','@',' ','@'},
        {' ',' ',' ','@'},
        {' ',' ',' ','@'},
        {' ',' ',' ','@'}
    };
    char due[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {' ',' ',' ','@'},
        {' ','@','@',' '},
        {'@',' ',' ',' '},
        {'@','@','@','@'}
    };
    char tre[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {' ',' ',' ','@'},
        {' ','@','@','@'},
        {' ',' ',' ','@'},
        {'@','@','@','@'}
    };
    Schermo* schermo = &(gameData->schermo);
    // pulizia preliminare
    int row= HUDSUPLETTERROWSTART;
    int col= MANCHESTARTCOL;
    for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
        row = rel_row + HUDSUPLETTERROWSTART;
        for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
            col= rel_col + MANCHESTARTCOL;
            schermo->screenMatrix[row][col].ch = ' ';
			schermo->screenMatrix[row][col].color = SFONDO_COL;
			schermo->screenMatrix[row][col].id = 0;
			schermo->screenMatrix[row][col].tipo = HUD_OBJ;
            schermo->screenMatrix[row][col].is_changed=true;
        }
    }
    // print del livello
    row= HUDSUPLETTERROWSTART;
    col= MANCHESTARTCOL;
    switch(gameData->gameInfo.manche){
        case 1:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + MANCHESTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = uno[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 2:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + MANCHESTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = due[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 3:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + MANCHESTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = tre[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        default:
            break;
    }
    return;
}

void printScore(GameData* gameData){
    Schermo* schermo = &(gameData->schermo);
    int centinaia=(gameData->gameInfo.punteggio/100)%9;
    int decine=(gameData->gameInfo.punteggio/10)%9;
    int unita = gameData->gameInfo.punteggio%9;
    char zero[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {'@',' ',' ','@'},
        {'@',' ',' ','@'},
        {'@',' ',' ','@'},
        {'@','@','@','@'}
    };
    char uno[CIFRA_H][CIFRA_W]={
        {' ',' ','@','@'},
        {' ','@',' ','@'},
        {' ',' ',' ','@'},
        {' ',' ',' ','@'},
        {' ',' ',' ','@'}
    };
    char due[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {' ',' ',' ','@'},
        {' ','@','@',' '},
        {'@',' ',' ',' '},
        {'@','@','@','@'}
    };
    char tre[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {' ',' ',' ','@'},
        {' ','@','@','@'},
        {' ',' ',' ','@'},
        {'@','@','@','@'}
    };
    char quattro[CIFRA_H][CIFRA_W]={
        {'@',' ',' ','@'},
        {'@',' ',' ','@'},
        {'@','@','@','@'},
        {' ',' ',' ','@'},
        {' ',' ',' ','@'}
    };
    char cinque[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {'@',' ',' ',' '},
        {'@','@','@','@'},
        {' ',' ',' ','@'},
        {'@','@','@','@'}
    };
    char sei[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {'@',' ',' ',' '},
        {'@','@','@','@'},
        {'@',' ',' ','@'},
        {'@','@','@','@'}
    };
    char sette[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {'@',' ',' ','@'},
        {' ',' ',' ','@'},
        {' ',' ',' ','@'},
        {' ',' ',' ','@'}
    };
    char otto[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {'@',' ',' ','@'},
        {'@','@','@','@'},
        {'@',' ',' ','@'},
        {'@','@','@','@'}
    };
    char nove[CIFRA_H][CIFRA_W]={
        {'@','@','@','@'},
        {'@',' ',' ','@'},
        {'@','@','@','@'},
        {' ',' ',' ','@'},
        {'@','@','@','@'}
    };
    // pulizia dello score
    int row = HUDSUPLETTERROWSTART;
    int col = SCORESTARTCOL;
    for(int rel_row=0; rel_row < CIFRA_H;rel_row++){
        row = rel_row + HUDSUPLETTERROWSTART;
        for(int rel_col=0; rel_col< SCORE_W;rel_col++){
            col= rel_col + SCORESTARTCOL;
            schermo->screenMatrix[row][col].ch = ' ';
			schermo->screenMatrix[row][col].color = SFONDO_COL;
			schermo->screenMatrix[row][col].id = 0;
			schermo->screenMatrix[row][col].tipo = HUD_OBJ;
            schermo->screenMatrix[row][col].is_changed=true;
        }
    }
    // stampa dello score
    row = HUDSUPLETTERROWSTART;
    col = CENTSTARTCOL;
    switch(centinaia){
        case 0:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + CENTSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = zero[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 1:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + CENTSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = uno[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 2:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + CENTSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = due[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 3:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + CENTSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = tre[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 4:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + CENTSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = quattro[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 5:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + CENTSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = cinque[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 6:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + CENTSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = sei[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 7:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + CENTSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = sette[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 8:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + CENTSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = otto[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 9:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + CENTSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = nove[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        default:
            break;
    }
    row = HUDSUPLETTERROWSTART;
    col = DECSTARTCOL;
    switch(decine){
        case 0:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + DECSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = zero[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 1:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + DECSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = uno[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 2:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + DECSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = due[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 3:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + DECSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = tre[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 4:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + DECSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = quattro[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 5:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + DECSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = cinque[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 6:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + DECSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = sei[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 7:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + DECSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = sette[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 8:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + DECSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = otto[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 9:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + DECSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = nove[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        default:
            break;
    }
    row = HUDSUPLETTERROWSTART;
    col = UNITSTARTCOL;
    switch(unita){
        case 0:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + UNITSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = zero[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 1:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + UNITSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = uno[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 2:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + UNITSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = due[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 3:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + UNITSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = tre[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 4:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + UNITSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = quattro[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 5:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + UNITSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = cinque[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 6:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + UNITSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = sei[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 7:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + UNITSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = sette[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 8:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + UNITSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = otto[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        case 9:
            for(int rel_row=0; rel_row<CIFRA_H;rel_row++){
                row = rel_row + HUDSUPLETTERROWSTART;
                for(int rel_col=0; rel_col<CIFRA_W;rel_col++){
                    col= rel_col + UNITSTARTCOL;
                    schermo->screenMatrix[row][col].ch = ' ';
			        schermo->screenMatrix[row][col].color = nove[rel_row][rel_col]=='@' ? LVL_COL : SFONDO_COL;
			        schermo->screenMatrix[row][col].id = 0;
			        schermo->screenMatrix[row][col].tipo = HUD_OBJ;
                    schermo->screenMatrix[row][col].is_changed=true;
                }
            }
            break;
        default:
            break;
    }

    return;
}

void aggiornaHud(GameData* gameData){
    if(gameData->gameInfo.livelloIsChanged){
        printLvl(gameData);
        gameData->gameInfo.livelloIsChanged=false;
    }
    if(gameData->gameInfo.mancheIsChanged){
        printManche(gameData);
        gameData->gameInfo.mancheIsChanged=false;
    }
    if(gameData->gameInfo.punteggioIsChanged){
        printScore(gameData);
        gameData->gameInfo.punteggioIsChanged=false;
    }
    if(gameData->gameInfo.tempoIsChanged){
        printTempo(gameData);
        gameData->gameInfo.tempoIsChanged=false;
    }
    if(gameData->gameInfo.viteIsChanged){
        printVite(gameData);
        gameData->gameInfo.viteIsChanged=false;
    }
    return;
}