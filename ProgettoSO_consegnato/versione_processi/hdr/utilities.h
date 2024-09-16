#ifndef UTILITIES_H
#define UTILITIES_H
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include "./sound.h"

// macro aggiuntive
#define NOPID 0 // indica che non esiste un pid associato
#define NOID -1 // indica che non esiste un id associato
#define NOPOS -1 // indica che non esiste una posizione assegnata
#define NO_OBJ -1 // indica che non esiste un oggetto associato

// colori aggiuntivi
#define COLOR_GRAY 8

// velocità di aggiornamento
#define SUPERFAST 20000 // la velocita di aggiornamento piu veloce

// velocità flussi 
#define FLUSSO_LENTO  600000
#define FLUSSO_NORM   600000
#define FLUSSO_VELOCE 200000

// dimensioni totali schermo
#define WIDTH 105 // larghezza massima dello schermo
#define HEIGHT 40 // altezza massima dello schermo

// altezze oggetti di gioco
#define BORDER_H 1 // altezza del bordo
#define HUDSUP_H 7 // altezza della sezione hud superiore
#define FULLTANA_H 1 // altezza della riga piena di tana
#define TANA_H 4 // altezza delle tane
#define ARGINE_H 4 // altezza dell'argine
#define FIUME_H 16 // altezza del fiume
#define MARCIAPIEDE_H 2 // altezza del marciapiede
#define HUDINF_H 4 // altezza della sezione hud inferiore
#define RANA_H 2 // altezza della rana
#define COCCODRILLO_H 2 // altezza del coccodrillo
#define PROIETTILE_H 2 // altezza del proiettile
#define LETTER_H 5 // altezza lettering hud parte superiore
#define CIFRA_H 5 // altezza di una cifra
#define TEMPO_H 2 // altezza hud per il tempo
#define PIANTA_H 2 // altezza della pianta
#define HUDVITEMENO_H 8

// larghezze oggetti di gioco
#define RANA_W 3 // larghezza della rana
#define PROIETTILE_W 1 // larghezza del proiettile
#define TANA_W 9 // larghezza della tana
#define COCCODRILLO_W 9 // larghezza del coccodrillo
#define NOTANA_W 13 // larghezza del tratto tra una tana e l'altra
#define LVLMANCHEDEF_W 23 // larrghezza scritta di default per lvl e manche
#define SCOREDEF_W 26 // larghezza scritta di default per score
#define CIFRA_W 4 // larghezza di una cifra
#define SCORE_W 13 // larghezza dello score(numeri)
#define VITE_W 11 // larghezza massima hud vite
#define TEMPO_W LASTGAMECOL - TEMPOSTARTCOL // larghezza hud per il tempo
#define PIANTA_W 3 // larghezza della pianta
#define HUDVITEMENO_W 27

// righe start e end degli elementi di gioco
#define BORDERROWSUP 0 // riga superiore di bordo
#define BORDERROWINF HEIGHT - 1 // riga inferiore di bordo

#define HUDSUPROWSTART 1 // prima riga dell'hud parte superiore
#define HUDSUPROWEND HUDSUPROWSTART + HUDSUP_H - 1 // ultima riga dell' hud parte superiore

#define HUDSUPLETTERROWSTART HUDSUPROWSTART + 1 // prima riga del lettering hud sup
#define HUDSUPLETTERROWEND HUDSUPLETTERROWSTART + HUDSUPLETTER_H -1 // ultima riga lettering hud sup

#define FIRSTGAMEROW HUDSUPROWEND + 1 // prima riga dello schermo di gioco 
#define LASTGAMEROW HUDINFROWSTART - 1 // ultima riga dello schermo di gioco 

#define FULLTANAROWSTART HUDSUPROWEND + 1 // prima riga di full tana
#define FULLTANAROWEND FULLTANAROWSTART + FULLTANA_H - 1 // ultima riga di full tana

#define TANAROWSTART FULLTANAROWEND + 1 // prima riga di tana 
#define TANAROWEND TANAROWSTART + TANA_H - 1 // ultima riga di tana

#define ARGINEROWSTART TANAROWEND + 1 // prima riga di argine
#define ARGINEROWEND ARGINEROWSTART + ARGINE_H - 1 // ultima riga di argine

#define FIUMEROWSTART ARGINEROWEND + 1 // prima riga di fiume
#define FIUMEROWEND FIUMEROWSTART + FIUME_H - 1 // ultima riga di fiume

#define MARCIAPIEDEROWSTART FIUMEROWEND + 1 // prima riga di marciapiede
#define MARCIAPIEDEROWEND MARCIAPIEDEROWSTART + MARCIAPIEDE_H - 1 // ultima riga di marciapiede

#define HUDINFROWSTART MARCIAPIEDEROWEND + 1// prima riga dell' hud inferiore
#define HUDINFROWEND HUDINFROWSTART + HUDINF_H - 1 // ultima riga dell' hud inferiore

#define VITEROWSTART HUDINFROWSTART + 1 // prima riga dell'hud delle vite

#define TEMPOROWSTART HUDINFROWSTART + 1 // prima riga dell'hud dell' tempo

#define VITEMENOROWSTART 12

// colonne sx e dx massime degli elementi di gioco
#define BORDERCOLSX 0 // posizione colonna sinistra del bordo
#define BORDERCOLDX 104 // posizione colonna destra del bordo

#define FIRSTGAMECOL BORDERCOLSX + 1 // prima colonna utile di gioco 
#define LASTGAMECOL BORDERCOLDX - 1 // ultima colonna utile di gioco 

#define HUDLETTERSUPCOLSX FIRSTGAMECOL + 1 // prima colonna del lettering hud sup
#define SCOREDEFCOLSTART 60 // prima colonna della scritta di default per lo score
#define LVLSTARTCOL 19 // prima colonna della cifra che indica il livello
#define MANCHESTARTCOL 26 // prima colonna della cifra che indica la manche
#define SCORESTARTCOL 87 // prima colonna della cifra dello score
#define CENTSTARTCOL 87 // prima colonna della cifra delle centinaia dello score
#define DECSTARTCOL 92 // prima colonna della cifra delle decine dello score 
#define UNITSTARTCOL 97 // prima colonna della cifra delle unità dello score
#define VITESTARTCOL FIRSTGAMECOL + 1 // prima colonna dell'hud delle vite
#define TEMPOSTARTCOL 50 // prima colonna dell' hud del tempo
#define PLANT0STARTCOL 15 // prima colonna della prima pianta
#define PLANT1STARTCOL PLANT0STARTCOL + DISTRAPIANTE // prima colonna della seconda pianta
#define PLANT2STARTCOL PLANT1STARTCOL + DISTRAPIANTE // prima colonna della terza pianta
#define PLANT3STARTCOL PLANT2STARTCOL + DISTRAPIANTE // prima colonna della quarta pianta
#define INITCOCCODRILLOSXCOL LASTGAMECOL - COCCODRILLO_W // colonna di partenza per i coccodrilli che si muovono verso sinistra

// colori
#define SFONDO_COL 0 // colore sfondo
#define RANA_COL 1 // colore rana
#define COCCODRILLOBUONO_COL 2 // colore coccodrillo buono
#define COCCODRILLOCATTIVO_COL 3 // colore coccodrillo cattivo
#define PIANTA_COL 4 // colore piante
#define PROIETTILE_COL 5 // colore proiettile
#define MARCIAPIEDE_COL 6 // colore marciapiede
#define FIUME_COL 7 // colore fiume
#define ARGINE_COL 8 // colore argine
#define TANE_COL 9 // colore tane
#define MENUBOX_COL 10 // colore della menu box
#define BORDER_COL 11 // colore del bordo
#define LAVA_COL 13 // colore lava
#define LVL_COL 14 // colore dell'hud per il livello
#define TEMPO_COL 15 // colore hud per il tempo
#define LAMPEGGIA 16 // colore coccodrillo quando lampeggia

// massimo numero di oggetti per tipo
#define MAXNCOCCODRILLI 24
#define MAXNPROIETTILI 3
#define MAXNNEMICI 4
#define MAXNPROIETTILINEMICI 4
#define MAXNTANE 5
#define MAXCOCCODRILLIXFLUSSO 4
#define NUMFLUSSI 8

// quantita oggetti
#define N_SPRITES 13
#define NUM_CIFRE 10

// tempo
#define TEMPOLVL1 240 // numero di secondi per manche per il primo livello
#define TEMPOLVL2 340 // numero di secondi per manche per il secondo livello
#define TEMPOLVL3 440 // numero di secondi per manche per il terzo livello

#define TEMPO_SPAWN_COCCODRILLI 7 // tempo di spawn dei coccodrilli

// distanze tra oggetti di gioco
#define DISTTRATANE 22 // distanza tra una tana e l'altra
#define DISTRAPIANTE 22 // distanza tra una pianta e l'altra

// file fiume per coccorilli
#define FILA_UNO FIUMEROWSTART 
#define FILA_DUE FILA_UNO + 2
#define FILA_TRE FILA_DUE + 2
#define FILA_QUATTRO FILA_TRE + 2
#define FILA_CINQUE FILA_QUATTRO + 2
#define FILA_SEI FILA_CINQUE + 2
#define FILA_SETTE FILA_SEI + 2
#define FILA_OTTO FILA_SETTE + 2

// sezione enumerazioni

// rappresenta il tipo di sprite
// elenco: S_RANA, S_PROIETTILE, S_TANA_APERTA, S_TANA_CHIUSA, S_PIANTA, S_PROIETTILE_NEMICO
typedef enum{
	S_RANA, 
	S_PROIETTILE, 
	S_TANA_APERTA, 
	S_TANA_CHIUSA,
	S_PIANTA,
	S_PROIETTILE_NEMICO,
	S_COCCODRILLO_SX,
	S_COCCODRILLO_DX,
	S_COCCODRILLO_SX_C,
	S_COCCODRILLO_DX_C,
	S_COCCODRILLO_DX_L,
	S_COCCODRILLO_SX_L
}TipoSprite;

// rappresenta il tipo di collisione
// elenco: NO_COLLISIONE, RANA_FIUME, RANA_TANA_APERTA, RANA_TANA_CHIUSA
typedef enum{
	NO_COLLISIONE,
	RANA_FIUME,
	RANA_TANA_APERTA,
	RANA_TANA_CHIUSA,
	PROIETTILE_COCCODRILLO_CATTIVO,
	RANA_COCCODRILLO_BUONO,
	PROIETTILE_NEMICO,
	PROIETTILENEMICO_RANA,
	PROIETTILE_PROIETTILENEMICO,
	NEMICO_RANA,
	RANA_LAVA,
	RANA_COCCODRILLO_CATTIVO,
	RANA_MARCIAPIEDE,
	RANA_ARGINE,
	PROIETTILE_COCCODRILLO_BUONO,
}TipoCollisione;

// enumerazione per il tipo di oggetto su schermo
// - elenco: TANA_OPEN_OBJ, TANA_CLOSE_OBJ, FIUME_OBJ, ARGINE_OBJ,
// MARCIAPIEDE_OBJ, COCCODRILLO_CATTIVO_OBJ, COCCODRILLO_BUONO_OBJ, PN_OBJ,
// N_OBJ, P_OBJ, RANA_OBJ, SFONDO_OBJ, LAVA_OBJ
typedef enum{
	TANA_OPEN_OBJ, 
	TANA_CLOSE_OBJ,
	FIUME_OBJ,
	ARGINE_OBJ,
	MARCIAPIEDE_OBJ,
	COCCODRILLO_CATTIVO_OBJ,
	COCCODRILLO_BUONO_OBJ,
	PN_OBJ,
	N_OBJ,
	P_OBJ,
	RANA_OBJ,
	SFONDO_OBJ,
	LAVA_OBJ,
	HUD_OBJ
}TipoObj;

// enumerazione che indica lo stato dell'oggetto tana
// elenco : TANA_OPENED, TANA_CLOSED 
typedef enum{
	TANA_OPENED, 
	TANA_CLOSED 
}TanaStatus;

// Struttura dati per pipe
// - x : posizione su asse x
// - y : posizione su asse y
// - type : carattere che indica il tipo di oggetto di gioco
// - id : codice identificativo dell' oggetto 
typedef struct{
    int x;
    int y;
    char type;
    int id;
}PipeData;

// struttura dati per le posizioni al passo precedente degli oggetti in gioco
// - rana : vecchia posizione della rana
// - coccodrilli : array delle vecchie posizioni dei coccodrilli
// - nemici : array delle vecchie posizioni dei nemici
// - proiettili : array delle vecchie posizioni dei proiettili amici
// - proiettiliNemici : array delle vecchie posizioni dei proiettili nemici
typedef struct{
	PipeData rana;
	PipeData coccodrilli[MAXNCOCCODRILLI];
	PipeData nemici[MAXNNEMICI];
	PipeData proiettili[MAXNPROIETTILI];
	PipeData proiettiliNemici[MAXNPROIETTILINEMICI];
}OldPos;

// struttura dati per singola cella della matrice che rappresenta lo schermo di gioco
// - ch : carattere della cella da stampare
// - color : colore della cella
// - is_changed : indica se la cella è stata modificata dall'ultimo refresh
// - id : indica l'id dell'oggetto di gioco che sta sulla cella
// - tipo : indica il tipo di oggetto di gioco che sta sulla cella
typedef struct{
	char ch;
	int color;
	bool is_changed;
	int id;
	TipoObj tipo;
}ScreenCell;

// struttura dati che rappresenta l'oggetto di gioco tana
// - info : rappresenta posizione e id della tana di tipo pipeData
// - status : è lo stato della tana (aperta/chiusa) di tipo TanaStatus
typedef struct {
	PipeData info;
	TanaStatus status;
}Tana;

// struttura per singolo sprite:
// - max_row : numero massimo di righe dello sprite
// - max_col : numero massimo di colonne dello sprite
// - sprite : puntatore a matrice di caratteri (sprite)
// - color : intero che rappresenta il colore dello sprite
typedef struct{
	int max_row;
	int max_col;
	char **sprite;
	int color;
	TipoObj tipo;
}Sprite;

// struttura per le info di gioco
// - tempo : per gestione del tempo
// - vite : le vite rimanenti della rana
// - punteggio : il punteggio dell'utente
// - livello : il livello del gioco
// - mache : il numero di manche attuale all'interno del livello
typedef struct {
	int secondi_di_gioco;
	bool tempoIsChanged;
	int vite;
	bool viteIsChanged;
	int punteggio;
	bool punteggioIsChanged;
	int livello;
	bool livelloIsChanged;
	int manche;
	bool mancheIsChanged;
	bool ranaIsDead;
	bool mancheWin;
} GameInfo;		

// struttura che contiene tutti i pid dei processi usati
// - pidRana : il pid del processo che si occupa del movimento della rana
// - pidProiettili : array dei pid dei processi proiettile amico
// - pidProiettiliNemici : array dei pid dei processi proiettile nemico
// - pidNemici : array dei pid dei processi nemici
// - pidCoccodrilli : array dei pid dei processi coccodrillo
typedef struct{
	pid_t pidRana;
	pid_t pidTempo;
	pid_t pidMusica;
	pid_t soundPlayer;
	pid_t pidProiettili[MAXNPROIETTILI];
	pid_t pidProiettiliNemici[MAXNPROIETTILINEMICI];
	pid_t pidNemici[MAXNNEMICI];
	pid_t pidCoccodrilli[MAXNCOCCODRILLI];
}Pids;

// struttura che rappresenta lo schermo di gioco
// - screenMatrix : rappresenta lo schermo
// - staticScreenMatrix : rappresenta solo gli oggetti statici di gioco, per ripristino
typedef struct{
	ScreenCell screenMatrix[HEIGHT][WIDTH]; // matrice che rappresenta lo schermo
	ScreenCell staticScreenMatrix[HEIGHT][WIDTH]; // matrice degli elementi statici dello schermo
}Schermo;

// struttura che contiene i contatori degli oggetti in gioco
// - contProiettili : il numero di proiettili amici in gioco
// - contProietiiliN : il numero di proietiili nemici in gioco
// - contNemici : il numero di nemici in gioco
// - contCoccodrilli : il nuemro di coccodrilli in gioco
typedef struct{
	int contProiettili;
	int contProiettiliN;
	int contNemici;
	int contCoccodrilli;
}Cont;

// struttura per la posizione assouluta della rana all'interno della matrice schermo
// - x : coordinata x
// - y : coordinata y
// - on_coccodrillo : booleano indica se la rana e su un coccodrillo
// - id_coccodrillo : l'id del coccodrillo sul quale è la rana
// - offset_on_coccodrillo : offeset posizione rana sul coccodrillo
typedef struct{
	int x;
	int y;
	bool on_coccodrillo;
	int id_coccodrillo;
	int offset_on_coccodrillo;
}RanaAbsPos;

typedef struct{
	int id;
	bool is_buono;
	bool is_fase_pre_immersione;
	bool is_fase_immersione;
	bool is_going_deep;
	bool is_going_up;
	bool is_deep;
	bool lampeggia;

	int offset_deep;
	int direction;
	int passi;
	int passi_in_immersione;
	int passi_in_pre_immersione;
	int passi_deep;
	int fila;
}CocodrileControl;

typedef struct{
	int direction;
	int vel;
	int n_coccodrilli_attivi;
}Flusso;

// struttura dati generale del gioco, contiene tutti i dati significativi per la gestione
// - pipe : il file descriptor della pipe
// - pipeData : per leggere la pipe
// - ranaAbsPos : posizione assoluta della rana all'interno della matrice
// - sprites : tutti gli sprite usati(array)
// - schermo : schermo e shermo degli elementi statici
// - pids : contiene tutti i pid dei processi
// - contatori : contatori per gli oggetti in gioco
// - tane : tutti gli oggetti Tana (array)
// - gameInfo : informazioni sulla partita in corso es vite score manche etc.
// - oldPos : contiene le posizioni al passo precedente di tutti gli oggetti in gioco
typedef struct{
	int* pipe; // la pipe
	int* pipe_suoni; // la pipe per i suoni di gioco
	PipeData pipeData; // struttura per leggere la pipe
	RanaAbsPos ranaAbsPos; // struttura per posizione assoluta della rana
	Sprite sprites[N_SPRITES]; // le sprite degli oggetti
	Schermo schermo; // lo schermo
	Pids pids; // tutti i pid dei processi figli
	Cont contatori; // contatori per oggetti
	Tana tane[MAXNTANE]; // oggetti tana
	GameInfo gameInfo; // informaizioni sul gioco lvl, manche score vite
	OldPos oldPos; // dati oggetti al passo precedente
	CocodrileControl controlloCoccodrilli[MAXNCOCCODRILLI]; // dati per oggetti coccodrilli
	Flusso flussi[NUMFLUSSI];
	unsigned int randomSeed;
}GameData;



// Struttura che rappresenta una collisione tra due oggetti in gioco
// - tipoCollisione : il tipo della collisione, indica i due oggetti coinvolti
// - oggetto_attivo : il tipo di oggetto che ha provocato la collisione
// - id_oggetto_attivo : l'id dell'oggetto che ha provocato la collisione
// - oggetto_passivo : il tipo di oggetto che ha subito la collisione
// - id_oggetto_attivo : l'id dell'oggetto che ha subito la collisione
typedef struct{
	TipoCollisione tipoCollisione;
	TipoObj oggetto_attivo; // oggetto che si muove e che provoca collisione
	int id_oggetto_attivo;
	TipoObj oggetto_passivo; // oggetto che sta fermo e subisce la collisione
	int id_oggetto_passivo;
	int hit_point_x;
	int hit_point_y;
}Collisione;

// inizializza ncurses ed i colori utilizzati
void inizializzaNcurses();

/** @brief crea una pipe per la comunicazione
 * @param pipe_fd array di interi di dimensione 2
*/ 
void creaPipe(int pipe_fd[2]);

/** @brief imposta i colori
*/
void inizializzaColorazione();

/** @brief termina il processo dato
 * @param target_pid : il pid del processo da terminare
*/
void uccidiProcesso(pid_t target_pid);

/** @brief targettizza il pid del processo da terminare e lo termina
 * @param array_pid : array sul quale è presente il target
 * @param id_processo : indice del target 
*/
void targetUccidiProcesso( pid_t* array_pid, int id_processo);

/** @brief dato un array di pid individua una posizione vuota ed utile
 * @param array_pid : l'array di pid da analizzare
 * @param lunghezza : la lunghezza di array_pid
*/
int id_disponibile(pid_t *array_pid, int lunghezza);

/** @brief aggiorna la posizione al passo precedente con la nuova posizione
 * @param old_pos : posizione al passo precedente
 * @param pipeData : posizione attuale
*/
void aggiornaOldPos(PipeData *old_pos,PipeData *pipeData);

/** @retval true se il gioco è terminato*/
bool isGameOver(GameData* gameData);

/** @retval true se il giocatore ha vinto il livello*/
bool isWin(GameData* gameData);

/** @brief stampa a schermo il bordo dell gioco*/
void stampaBox();
bool isFrogMoveLecit(int newX, int newY,RanaAbsPos ranaPos,PipeData pipeData);
int generaRandom_r(int min, int max, unsigned int *seed);
int fromFilaToRow(int fila);
void setScreenCell(ScreenCell* screenCell,TipoObj tipo,char ch,int id,int color,bool changed);
void setPipeData(PipeData* pipeData, char type, int id, int x,int y);
#endif // UTILITIES_H