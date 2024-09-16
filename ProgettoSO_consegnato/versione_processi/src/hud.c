#include "../hdr/hud.h"

void printVite(GameData *GameData)
{
    int vite = GameData->gameInfo.vite;
    Schermo *schermo = &(GameData->schermo);
    char rana[RANA_H][RANA_W] = {
        {'\\', '.', '/'},
        {'/', '-', '\\'}};
    // pulizia sezione vite
    clearScreenArea(schermo, VITEROWSTART, VITESTARTCOL, VITE_W, RANA_H);

    // stampa sezione vite
    int row = 0, col = 0;
    for (int n_vite = 0; n_vite < vite; n_vite++)
    {
        for (int rel_row = 0; rel_row < RANA_H; rel_row++)
        {
            row = VITEROWSTART + rel_row;
            for (int rel_col = 0; rel_col < RANA_W; rel_col++)
            {
                col = VITESTARTCOL + rel_col + (n_vite * (RANA_W + 1));
                setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, rana[rel_row][rel_col], 0, RANA_COL, true);
            }
        }
    }
    return;
}

void printInitTempo(GameData *gameData)
{
    Schermo *schermo = &(gameData->schermo);
    for (int row = TEMPOROWSTART; row < TEMPOROWSTART + TEMPO_H; row++)
    {
        for (int col = TEMPOSTARTCOL; col < TEMPOSTARTCOL + TEMPO_W; col++)
        {
            setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, TEMPO_COL, true);
        }
    }
    return;
}

void printTempo(GameData *gameData)
{
    Schermo *schermo = &(gameData->schermo);
    int secondi = gameData->gameInfo.secondi_di_gioco;
    int maxSeconds = 0; // nuermo massimo di secondi per manche a seconda del livello
    switch (gameData->gameInfo.livello)
    {
    case 1:
        maxSeconds = TEMPOLVL1;
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

    int tempoInHudPassato = ((double)(TEMPO_W) / (double)maxSeconds) * (double)secondi;
    if (secondi <= maxSeconds)
    {
        for (int row = TEMPOROWSTART; row < TEMPOROWSTART + TEMPO_H; row++)
        {
            for (int col = TEMPOSTARTCOL; col < TEMPOSTARTCOL + tempoInHudPassato; col++)
            {
                setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, SFONDO_COL, true);
            }
        }
    }
    return;
}

void printDefaultHudSup(GameData *gameData)
{
    // print default per livello e manche
    char defLvlManche[HUDSUP_H][LVLMANCHEDEF_W] = {
        {'@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', ' ', ' ', ' ', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', ' ', ' ', ' ', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'@', '@', '@', '@', ' ', ' ', '@', '@', ' ', ' ', '@', '@', '@', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '@'}};
    Schermo *schermo = &(gameData->schermo);
    int row = 0, col = 0, color;
    for (int rel_row = 0; rel_row < HUDSUP_H; rel_row++)
    {
        row = rel_row + HUDSUPLETTERROWSTART;
        for (int rel_col = 0; rel_col < LVLMANCHEDEF_W; rel_col++)
        {
            col = rel_col + HUDLETTERSUPCOLSX;
            color = defLvlManche[rel_row][rel_col] == '@' ? LVL_COL : SFONDO_COL;
            setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, color, true);
        }
    }
    // print default per score
    char defScore[HUDSUP_H][SCOREDEF_W] = {
        {'@', '@', '@', '@', ' ', '@', '@', '@', '@', ' ', '@', '@', '@', '@', ' ', '@', '@', '@', ' ', ' ', '@', '@', '@', '@', ' ', ' '},
        {'@', ' ', ' ', ' ', ' ', '@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', '@', ' ', ' ', '@', ' ', ' ', ' ', ' ', '@'},
        {'@', '@', '@', '@', ' ', '@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', '@', '@', ' ', ' ', '@', '@', '@', ' ', ' ', ' '},
        {' ', ' ', ' ', '@', ' ', '@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', '@', ' ', ' ', '@', ' ', ' ', ' ', ' ', '@'},
        {'@', '@', '@', '@', ' ', '@', '@', '@', '@', ' ', '@', '@', '@', '@', ' ', '@', ' ', ' ', '@', ' ', '@', '@', '@', '@', ' ', ' '}};
    row = 0, col = 0;
    for (int rel_row = 0; rel_row < HUDSUP_H; rel_row++)
    {
        row = rel_row + HUDSUPLETTERROWSTART;
        for (int rel_col = 0; rel_col < SCOREDEF_W; rel_col++)
        {
            col = rel_col + SCOREDEFCOLSTART;
            color = defScore[rel_row][rel_col] == '@' ? LVL_COL : SFONDO_COL;
            setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, color, true);
        }
    }
    return;
}

void printLvl(GameData *gameData)
{
    char(*digits)[CIFRA_H][CIFRA_W] = getDigits();
    int level = gameData->gameInfo.livello;
    char(*digit)[CIFRA_W] = digits[level]; // Seleziona la cifra corrispondente

    Schermo *schermo = &(gameData->schermo);

    clearScreenArea(schermo, HUDSUPLETTERROWSTART, LVLSTARTCOL, CIFRA_W, CIFRA_H);

    printDigit(schermo, digit, HUDSUPLETTERROWSTART, LVLSTARTCOL, LVL_COL); // print del livello
    return;
}

void printManche(GameData *gameData)
{
    Schermo *schermo = &(gameData->schermo);
    int manche = gameData->gameInfo.manche;
    char(*digits)[CIFRA_H][CIFRA_W] = getDigits();
    char(*digit)[CIFRA_W] = digits[manche]; // Seleziona la cifra corrispondente

    // Pulizia preliminare
    clearScreenArea(schermo, HUDSUPLETTERROWSTART, MANCHESTARTCOL, CIFRA_W, CIFRA_H);

    printDigit(schermo, digit, HUDSUPLETTERROWSTART, MANCHESTARTCOL, LVL_COL);

    return;
}

void printScore(GameData *gameData)
{
    Schermo *schermo = &(gameData->schermo);
    char(*digits)[CIFRA_H][CIFRA_W] = getDigits();
    int centinaia = (gameData->gameInfo.punteggio / 100) % 9;
    int decine = (gameData->gameInfo.punteggio / 10) % 9;
    int unita = gameData->gameInfo.punteggio % 9;

    // Pulizia dello score
    clearScreenArea(schermo, HUDSUPLETTERROWSTART, SCORESTARTCOL, SCORE_W, CIFRA_H);

    // stampa dello score
    printDigit(schermo, digits[centinaia], HUDSUPLETTERROWSTART, CENTSTARTCOL, LVL_COL);
    printDigit(schermo, digits[decine], HUDSUPLETTERROWSTART, DECSTARTCOL, LVL_COL);
    printDigit(schermo, digits[unita], HUDSUPLETTERROWSTART, UNITSTARTCOL, LVL_COL);
    return;
}

void aggiornaHud(GameData *gameData)
{
    if (gameData->gameInfo.livelloIsChanged)
    {
        printLvl(gameData);
        gameData->gameInfo.livelloIsChanged = false;
    }
    if (gameData->gameInfo.mancheIsChanged)
    {
        printManche(gameData);
        gameData->gameInfo.mancheIsChanged = false;
    }
    if (gameData->gameInfo.punteggioIsChanged)
    {
        printScore(gameData);
        gameData->gameInfo.punteggioIsChanged = false;
    }
    if (gameData->gameInfo.tempoIsChanged)
    {
        printTempo(gameData);
        gameData->gameInfo.tempoIsChanged = false;
    }
    if (gameData->gameInfo.viteIsChanged)
    {
        printVite(gameData);
        gameData->gameInfo.viteIsChanged = false;
    }
    return;
}

void printSchermataWin(int totalScore)
{
    clear(); // pulisco schermo
    refresh();

    // stampa box
    stampaBox(0);

    printTitolo(5, (LASTGAMECOL - DIMTITOLOCOLS) / 2);

    // stampa logo
    printLogo(13, (LASTGAMECOL - DIMLOGOCOLS) / 2);

    printWin(26, (LASTGAMECOL - DIMWINCOLS) / 2);

    nodelay(stdscr, FALSE); // riabilita l'input bloccante

    mvprintw(33,(LASTGAMECOL - 16)/2,"Your score: %d",totalScore);
    // stampa la scritta premi un tasto per continuare
    mvprintw(35, (LASTGAMECOL - 33) / 2, "Premi un tasto per continuare...");
    getch();
    refresh();
    nodelay(stdscr, TRUE); // riabilita l'input non bloccante
    clear();
    refresh();
}

void printSchermataGameOver()
{
    clear(); // pulisco schermo
    refresh();

    // stampa box
    stampaBox(0);

    printTitolo(5, (LASTGAMECOL - DIMTITOLOCOLS) / 2);

    // stampa logo
    printLogo(13, (LASTGAMECOL - DIMLOGOCOLS) / 2);

    printGameOver(26, (LASTGAMECOL - DIMGAMEOVERCOLS) / 2);

    nodelay(stdscr, FALSE); // riabilita l'input bloccante

    // stampa la scritta premi un tasto per continuare
    mvprintw(35, (LASTGAMECOL - 33) / 2, "Premi un tasto per continuare...");
    getch();
    refresh();
    nodelay(stdscr, TRUE); // riabilita l'input non bloccante
    clear();
    refresh();
}

void printSchermataManche()
{
    clear(); // pulisco schermo
    refresh();

    // stampa box
    stampaBox(0);

    printTitolo(5, (LASTGAMECOL - DIMTITOLOCOLS) / 2);

    // stampa logo
    printLogo(13, (LASTGAMECOL - DIMLOGOCOLS) / 2);

    printMancheWin(26, (LASTGAMECOL - DIMMANCHECOLS) / 2);

    nodelay(stdscr, FALSE); // riabilita l'input bloccante

    // stampa la scritta premi un tasto per continuare
    mvprintw(35, (LASTGAMECOL - 33) / 2, "Premi un tasto per continuare...");
    getch();
    refresh();
    nodelay(stdscr, TRUE); // riabilita l'input non bloccante
    clear();
    refresh();
}

void printViteMinus()
{
    clear(); // pulisco schermo
    refresh();

    // stampa box
    stampaBox(0);

    printTitolo(5, (LASTGAMECOL - DIMTITOLOCOLS) / 2);

    // stampa logo
    printLogo(13, (LASTGAMECOL - DIMLOGOCOLS) / 2);

    printLose(26, (LASTGAMECOL - DIMLOSECOLS) / 2);

    nodelay(stdscr, FALSE); // riabilita l'input bloccante

    // stampa la scritta premi un tasto per continuare
    mvprintw(35, (LASTGAMECOL - 33) / 2, "Premi un tasto per continuare...");
    getch();
    refresh();
    nodelay(stdscr, TRUE); // riabilita l'input non bloccante
    clear();
    refresh();
}

void printLogo(int start_row, int start_col)
{
    char logoImg[DIMLOGOROWS][DIMLOGOCOLS] = {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', '_', ' ', ' ', ' ', '_', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', '(', '.', ')', '_', '(', '.', ')', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', '_', ' ', '(', ' ', ' ', ' ', '_', ' ', ' ', ' ', ')', ' ', '_', ' ', ' ', ' '},
        {' ', ' ', '/', ' ', '\\', '/', '\'', '-', '-', '-', '-', '-', '\'', '\\', '/', ' ', '\\', ' ', ' '},
        {'_', '_', '\\', ' ', '(', ' ', '(', ' ', ' ', ' ', ' ', ' ', ')', ' ', ')', ' ', '/', '_', '_'},
        {')', ' ', ' ', ' ', '/', '\\', ' ', '\\', '.', '_', '.', '/', ' ', '/', '\\', ' ', ' ', ' ', '('},
        {' ', ')', '_', '/', ' ', '/', '|', '\\', ' ', ' ', ' ', '/', '|', '\\', ' ', '\\', '_', '(', ' '}};

    // stampa logo
    for (int row = start_row; row < start_row + DIMLOGOROWS; row++)
    {
        for (int col = start_col; col < start_col + DIMLOGOCOLS; col++)
        {
            attron(COLOR_PAIR(RANA_COL));
            mvaddch(row, col, logoImg[row - start_row][col - start_col]);
            attroff(COLOR_PAIR(RANA_COL));
        }
    }
    return;
}

void printTitolo(int start_row, int start_col)
{
    char titoloMatrix[DIMTITOLOROWS][DIMTITOLOCOLS] = {
        " _____                   _____                ",
        "|  ___| __ ___   __ _   |  ___| __ ___   __ _ ",
        "| |_ | '__/ _ \\ / _` |  | |_ | '__/ _ \\ / _` |",
        "|  _|| | | (_) | (_| |  |  _|| | | (_) | (_| |",
        "|_|  |_|  \\___/ \\__,  | |_|  |_|  \\___/ \\__, |",
        "                |___/                   |___/ "};

    // stampa titolo
    for (int row = start_row; row < start_row + DIMTITOLOROWS; row++)
    {
        for (int col = start_col; col < start_col + DIMTITOLOCOLS; col++)
        {
            attron(COLOR_PAIR(RANA_COL));
            mvaddch(row, col, titoloMatrix[row - start_row][col - start_col]);
            attroff(COLOR_PAIR(RANA_COL));
        }
    }
    return;
}

void printLose(int start_row, int start_col)
{
    char titoloLose[DIMLOSEROWS][DIMLOSECOLS] = {
        " __   __            _                   ",
        " \\ \\ / /__  _   _  | |    ___  ___  ___ ",
        "  \\ V / _ \\| | | | | |   / _ \\/ __|/ _ \\",
        "   | | (_) | |_| | | |__| (_) \\__ \\  __/",
        "   |_|\\___/ \\__,_| |_____\\___/|___/\\___|",
        "                                        "};

    // stampa loose
    for (int row = start_row; row < start_row + DIMLOSEROWS; row++)
    {
        for (int col = start_col; col < start_col + DIMLOSECOLS; col++)
        {
            attron(COLOR_PAIR(RANA_COL));
            mvaddch(row, col, titoloLose[row - start_row][col - start_col]);
            attroff(COLOR_PAIR(RANA_COL));
        }
    }
}

void printWin(int start_row, int start_col)
{

    char titoloWin[DIMWINROWS][DIMWINCOLS] = {
        " __   __           __        ___       ",
        " \\ \\ / /__  _   _  \\ \\      / (_)_ __  ",
        "  \\ V / _ \\| | | |  \\ \\ /\\ / /| | '_ \\ ",
        "   | | (_) | |_| |   \\ V  V / | | | | |",
        "   |_|\\___/ \\__,_|    \\_/\\_/  |_|_| |_|",
    };

    for (int row = start_row; row < start_row + DIMWINROWS; row++)
    {
        for (int col = start_col; col < start_col + DIMWINCOLS; col++)
        {
            attron(COLOR_PAIR(RANA_COL));
            mvaddch(row, col, titoloWin[row - start_row][col - start_col]);
            attroff(COLOR_PAIR(RANA_COL));
        }
    }
    return;
}

void printMancheWin(int start_row, int start_col)
{
    char titoloMancheWin[DIMMANCHEROWS][DIMMANCHECOLS] = {
        "  __  __                  _           __        _____       ",
        " |  \\/  | __ _ _ __   ___| |__   ___  \\ \\      / /_ _|_ __  ",
        " | |\\/| |/ _` | '_ \\ / __| '_ \\ / _ \\  \\ \\ /\\ / / | || '_ \\ ",
        " | |  | | (_| | | | | (__| | | |  __/   \\ V  V /  | || | | |",
        " |_|  |_|\\__,_|_| |_|\\___|_| |_|\\___|    \\_/\\_/  |___|_| |_|"};
    for (int row = start_row; row < start_row + DIMMANCHEROWS; row++)
    {
        for (int col = start_col; col < start_col + DIMMANCHECOLS; col++)
        {
            attron(COLOR_PAIR(RANA_COL));
            mvaddch(row, col, titoloMancheWin[row - start_row][col - start_col]);
            attroff(COLOR_PAIR(RANA_COL));
        }
    }
    return;
}

void printGameOver(int start_row, int start_col)
{
    char titoloGameOver[DIMGAMEOVERROWS][DIMGAMEOVERCOLS] = {
        "   ____                         ___                 ",
        "  / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __ ",
        " | |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__|",
        " | |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |   ",
        "  \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|   "};
    for (int row = start_row; row < start_row + DIMGAMEOVERROWS; row++)
    {
        for (int col = start_col; col < start_col + DIMGAMEOVERCOLS; col++)
        {
            attron(COLOR_PAIR(RANA_COL));
            mvaddch(row, col, titoloGameOver[row - start_row][col - start_col]);
            attroff(COLOR_PAIR(RANA_COL));
        }
    }
    return;
}

void printDigit(Schermo *schermo, char digit[CIFRA_H][CIFRA_W], int startRow, int startCol, int color)
{
    for (int rel_row = 0; rel_row < CIFRA_H; rel_row++)
    {
        int row = rel_row + startRow;
        for (int rel_col = 0; rel_col < CIFRA_W; rel_col++)
        {
            int col = rel_col + startCol;
            int cellColor = (digit[rel_row][rel_col] == '@') ? color : SFONDO_COL;
            setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, cellColor, true);
        }
    }
}

void clearScreenArea(Schermo *schermo, int startRow, int startCol, int width, int height)
{
    for (int rel_row = 0; rel_row < height; rel_row++)
    {
        int row = rel_row + startRow;
        for (int rel_col = 0; rel_col < width; rel_col++)
        {
            int col = rel_col + startCol;
            setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, SFONDO_COL, true);
        }
    }
}

char (*getDigits())[CIFRA_H][CIFRA_W]
{
    static char cifre[NUM_CIFRE][CIFRA_H][CIFRA_W] = {
        {// Zero
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', '@'},
         {'@', ' ', ' ', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'}},
        {// Uno
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'}},
        {// Due
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', ' '},
         {'@', '@', '@', '@'}},
        {// Tre
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {' ', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {'@', '@', '@', '@'}},
        {// Quattro
         {'@', ' ', ' ', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'}},
        {// Cinque
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', ' '},
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {'@', '@', '@', '@'}},
        {// Sei
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', ' '},
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'}},
        {// Sette
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'}},
        {// Otto
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'}},
        {// Nove
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {'@', '@', '@', '@'}}};

    return cifre;
}