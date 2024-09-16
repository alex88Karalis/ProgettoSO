#ifndef HUD_H
#define HUD_H
#include "./utilities.h"

#define DIMLOGOROWS 7
#define DIMLOGOCOLS 19

#define DIMTITOLOROWS 6
#define DIMTITOLOCOLS 46

#define DIMLOSEROWS 6
#define DIMLOSECOLS 40

#define DIMWINROWS 5
#define DIMWINCOLS 39

#define DIMMANCHEROWS 5
#define DIMMANCHECOLS 60

#define DIMGAMEOVERROWS 5
#define DIMGAMEOVERCOLS 52

/***/
void printVite(GameData *GameData);

/***/
void printInitTempo(GameData *gameData);

/***/
void printTempo(GameData *gameData);

/***/
void printDefaultHudSup(GameData *gameData);

/***/
void printLvl(GameData *gameData);

/***/
void printManche(GameData *gameData);

/***/
void printScore(GameData *gameData);

/***/
void aggiornaHud(GameData *gameData);

void printDigit(Schermo *schermo, char digit[CIFRA_H][CIFRA_W], int startRow, int startCol, int color);

void clearScreenArea(Schermo *schermo, int startRow, int startCol, int width, int height);

char (*getDigits())[CIFRA_H][CIFRA_W];

void printLogo(int start_row, int start_col);

void printTitolo(int start_row, int start_col);

void printLose(int start_row, int start_col);

void printMancheWin(int start_row, int start_col);

void printWin(int start_row, int start_col);

void printGameOver(int start_row, int start_col);

void printSchermataWin(int totalScore);

void printSchermataGameOver();

void printSchermataManche();

void printViteMinus();
#endif
