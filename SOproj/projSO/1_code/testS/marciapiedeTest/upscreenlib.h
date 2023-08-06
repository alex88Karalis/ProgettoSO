// copie colori
#define MARCIAPIEDE 1
#define SFONDO 2
#define PRATO 3
#define FIUME 4
#define TANE 5
#define HUD 6
#define TRONCHI 7

// dim. schermo
#define NUM_ROWS 37
#define NUM_COLS 100	
#define NUM_TANE 5

// dim. tane

typedef struct {
	char sprite[2][9]; // 2 righe - 8 char + terminatore di stringa 
} SpriteTronco;




void disegnaFiume(int height, int width);
void disegnaTronco(int row, int col);
void disegnaTronchi(int width);
void disegnaTane(int width);
void disegnaHUD(int width);
