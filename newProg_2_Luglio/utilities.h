#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <semaphore.h>
#include <pthread.h>
#include "sound.h"
//#include <stddef.h>  // in più??
//#include "tempo.h"
//#include "schermo.h"


// colori aggiuntivi
#define COLOR_GRAY 8

// velocità di aggiornamento
#define SUPERFAST 20000 // la velocita di aggiornamento piu veloce

// velocità flussi 
#define FLUSSO_LENTO 220000
#define FLUSSO_NORM 170000
#define FLUSSO_VELOCE 140000


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
#define PIANTA_H 2 // altezza della pianta
#define LETTER_H 5 // altezza lettering hud parte superiore
#define CIFRA_H 5 // altezza di una cifra
#define TEMPO_H 2 // altezza hud per il tempo

// larghezze oggetti di gioco
#define RANA_W 3 // larghezza della rana
#define PROIETTILE_W 1 // larghezza del proiettile
#define TANA_W 9 // larghezza della tana
#define COCCODRILLO_W 9 // larghezza del coccodrillo
#define PIANTA_W 3 // larghezza della pianta
#define NOTANA_W 13 // larghezza del tratto tra una tana e l'altra
#define LVLMANCHEDEF_W 23 // larrghezza scritta di default per lvl e manche
#define SCOREDEF_W 26 // larghezza scritta di default per score
#define CIFRA_W 4 // larghezza di una cifra
#define SCORE_W 13 // larghezza dello score(numeri)
#define VITE_W 11 // larghezza massima hud vite
#define TEMPO_W LASTGAMECOL - TEMPOSTARTCOL // larghezza hud per il tempo

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
#define MAXNCOCCODRILLI 8
#define MAXNPROIETTILI 3
#define MAXNNEMICI 4
#define MAXNPROIETTILINEMICI 4
#define MAXNTANE 5

// quantita oggetti
#define N_SPRITES 13

// tempo
#define TEMPOLVL1 60 // numero di secondi per manche per il primo livello
#define TEMPOLVL2 90 // numero di secondi per manche per il secondo livello
#define TEMPOLVL3 120 // numero di secondi per manche per il terzo livello

#define TEMPO_SPAWN_COCCODRILLI 3 // tempo di spawn dei coccodrilli

// altro
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


/** rappresenta il tipo di sprite
 * 	- S_RANA, 
	- S_PROIETTILE, 
	- S_TANA_APERTA, 
	- S_TANA_CHIUSA,
	- S_PIANTA,
	- S_PROIETTILE_NEMICO,
	- S_COCCODRILLO_SX,
	- S_COCCODRILLO_DX,
	- S_COCCODRILLO_SX_C  
	- S_COCCODRILLO_DX_C 
	- S_COCCODRILLO_DX_L
	- S_COCCODRILLO_SX_L
*/
typedef enum{
	S_RANA, 
	S_PROIETTILE, 
	S_TANA_APERTA, 
	S_TANA_CHIUSA,
	S_PIANTA,
	S_PROIETTILE_NEMICO,
	S_COCCODRILLO_SX,
	S_COCCODRILLO_DX,
	S_COCCODRILLO_SX_C, // coccodrilli cattivi
	S_COCCODRILLO_DX_C,
	S_COCCODRILLO_DX_L,	// coccodrilli lampeggianti
	S_COCCODRILLO_SX_L
}TipoSprite;

/**
 * Rappresenta il tipo di collisione:
 * - NO_COLLISIONE
 * - RANA_FIUME
 * - RANA_TANA_APERTA
 * - RANA_TANA_CHIUSA
 * - PROIETTILE_NEMICO_RANA
 * - PROIETTILE_RANA_NEMICO
 * - PROIETTILE_RANA_PROIETTILE_NEMICO,
 * - PROIETTILE_NEMICO_PROIETTILE_RANA,
 * - PROIETTILE_COCCODRILLO_CATTIVO,
 * - RANA_COCCODRILLO_BUONO
 */
typedef enum{
	NO_COLLISIONE,
	RANA_FIUME,
	RANA_TANA_APERTA,
	RANA_TANA_CHIUSA,
	PROIETTILE_NEMICO_RANA,
	PROIETTILE_RANA_NEMICO,
	PROIETTILE_RANA_PROIETTILE_NEMICO,
	PROIETTILE_NEMICO_PROIETTILE_RANA,
	PROIETTILE_COCCODRILLO_CATTIVO,
	PROIETTILE_COCCODRILLO_BUONO,
	RANA_COCCODRILLO_BUONO
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
	PROIETTILE_NEMICO_OBJ,					// Proiettile Nemico
	NEMICO_OBJ,					// Nemico
	PROIETTILE_OBJ,					// Proiettile (della rana)
	RANA_OBJ,
	SFONDO_OBJ,
	LAVA_OBJ,
	HUD_OBJ
}TipoObj;

// Struttura dati per pipe
// - x : posizione su asse x
// - y : posizione su asse y
// - type : carattere che indica il tipo di oggetto di gioco
// - id : codice identificativo dell' oggetto 
// - thread_id : codice identificativo dell' thread 
typedef struct{
    int x;
    int y;
    char type;
    int id;
	pthread_t thread_id;
}PipeData;


// Struttura per conoscere lo stato del thread
// - thread_id : identificativo del thread
// - is_target : indica se il thread deve terminare
// - is_terminated : indica se il thread ha terminato
typedef struct ThreadControlBlock {
	pthread_t thread_id;
	bool  is_target;
	bool is_terminated;
}ThreadControlBlock;


// Struttura generale con riferimenti ai TCB di tutti i thread
// - *tcb_rana
// - tcb_proiettili [_]
// - tcb_coccodrilli[_]
// - tcb_piante[_]
// - tcb_proiettili_nemici[_]
// - tcb_disegna : TCB di drawThread.
// - tcb_tempo : per il thread TEMPO
typedef struct AllTCB{
	ThreadControlBlock *tcb_rana;
	ThreadControlBlock tcb_proiettili[MAXNPROIETTILI];
	ThreadControlBlock tcb_coccodrilli[MAXNCOCCODRILLI];
	ThreadControlBlock tcb_piante[MAXNNEMICI];
	ThreadControlBlock tcb_proiettili_nemici[MAXNPROIETTILINEMICI];
	ThreadControlBlock tcb_disegna;
	ThreadControlBlock tcb_tempo;
}AllTCB;

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

// enumerazione che indica lo stato dell'oggetto tana
// elenco : TANA_OPENED, TANA_CLOSED 
typedef enum{
	TANA_OPENED, 
	TANA_CLOSED 
}TanaStatus;

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

// struttura per il tempo di gioco
// - secondi: secondi rimanenti per la manche
// - start: istante di tempo iniziale
// - current: istante di tempo corrente
typedef struct{
	int secondi;
	time_t start;
	time_t current;
	int milliseconds;
}Tempo;

// struttura per le info di gioco
// - tempo : per gestione del tempo
// - vite : le vite rimanenti della rana
// - punteggio : il punteggio dell'utente
// - livello : il livello del gioco
// - mache : il numero di manche attuale all'interno del livello
typedef struct {
	Tempo tempo;
	bool tempoIsChanged;
	int vite;
	bool viteIsChanged;
	int punteggio;
	bool punteggioIsChanged;
	int livello;
	bool livelloIsChanged;
	int manche;
	bool mancheIsChanged;
} GameInfo;		

// struttura che contiene tutti i pid dei processi usati
// - pidRana : il pid del thread che si occupa del movimento della rana
// - pidProiettili : array dei pid dei thread proiettile amico
// - pidProiettiliNemici : array dei pid dei processi proiettile nemico
// - pidNemici : array dei pid dei processi nemici
// - pidCoccodrilli : array dei pid dei processi coccodrillo
typedef struct{
	pthread_t pidRana;
	pthread_t pidProiettili[MAXNPROIETTILI];
	pthread_t pidProiettiliNemici[MAXNPROIETTILINEMICI];
	pthread_t pidNemici[MAXNNEMICI];
	pthread_t pidCoccodrilli[MAXNCOCCODRILLI];
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
typedef struct{
	int x;
	int y;
	bool on_coccodrillo;
	int id_coccodrillo;
	int offset_on_coccodrillo;
}RanaAbsPos;

/*	Struttura che registra lo stato del coccodrillo
*/
typedef struct{
	int id;
	bool is_buono;
	bool is_fase_pre_immersione;
	bool is_fase_immersione;
	bool is_going_deep;
	bool is_going_up;
	bool is_deep;
	bool lampeggia;
	bool rana_on;

	int offset_deep;
	int direction;
	int passi;
	int passi_in_immersione;
	int passi_in_pre_immersione;
	int passi_deep;				// passi percorsi totalmente sott'acqua
	int offset_rana;
}CocodrileControl;


typedef struct{
	int direction;
	int vel;
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
// - allTCB : array di ThreadControlBlock di tutti i threads(per ora solo proiettili )
// - controlloCoccodrilli[] : vettore di CocodrileControl 
// - int errore : var di controllo.
typedef struct{
	int* pipe; // la pipe
	PipeData pipeData; // struttura per salvare i dati letti da buffer
	RanaAbsPos ranaAbsPos; // struttura per posizione assoluta della rana
	Sprite sprites[N_SPRITES]; // le sprite degli oggetti
	Schermo schermo; // lo schermo
	Pids pids; // tutti i pid dei processi figli
	Cont contatori; // contatori per oggetti
	Tana tane[5]; // oggetti tana
	GameInfo gameInfo; // informaizioni sul gioco lvl, manche score vite
	OldPos oldPos; // dati oggetti al passo precedente
	AllTCB *allTCB;
	CocodrileControl controlloCoccodrilli[MAXNCOCCODRILLI]; // dati per oggetti coccodrilli
	Flusso flussi[8];
	int errore;
}GameData;



// Struttura che rappresenta una collisione tra due oggetti in gioco
// - tipoCollisione : il tipo della collisione, indica i due oggetti coinvolti
// - oggetto_attivo : il tipo di oggetto che ha provocato la collisione
// - id_oggetto_attivo : l'id dell'oggetto che ha provocato la collisione
// - oggetto_passivo : il tipo di oggetto che ha subito la collisione
// - id_oggetto_passivo : l'id dell'oggetto che ha subito la collisione
typedef struct{
	TipoCollisione tipoCollisione;
	TipoObj oggetto_attivo; // oggetto che si muove e che provoca collisione
	int id_oggetto_attivo;
	TipoObj oggetto_passivo; // oggetto che sta fermo e subisce la collisione
	int id_oggetto_passivo;
	int hit_point_x;	// usati per collisione Rana-Coccodrillo
	int hit_point_y;
}Collisione;


/*--- strutture e funzioni per Threads	----*/

// Struttura che contiene i semafori
// - sem_t window_mutex: semaforo per mutua esclusione su funzioni che usano la stdscr
// - sem_t mutex: i semaforo binario per la sincronizzazione
// - sem_t presenti: semaforo che indica se presente dati sul buffer
// - sem_t disponibili: semaforo che indica se il buffer è vuoto
// - sem_t tcb_mutex: semaforo per mutua esclusione sui tcb
typedef struct Semaphore {
	sem_t window_mutex;
	sem_t mutex;			// inizializzato a 1
	sem_t presenti;			// inizializzato a 0
	sem_t disponibili;		// inizializzato a DIM_BUFFER=1
	sem_t tcb_mutex;		// inizializzato a 1
}Semaphore;

// Struttura usata per passare i dati utili come parametri all'avvio dei threads
// - struct Semaphore* semafori: ptr a struttura che contiene i semafori.
// - PipeData* buffer : la struttura usata come buffer.
// - GameData* gameData : il puntatore alla struttura con tutte le informazioni di gioco
// - int id : intero che indica l'identificativo dell' oggetto (diverso dal thread_id)
typedef struct Params{
	struct Semaphore *semafori;
	PipeData *buffer;
	GameData *gameData;
	int id;
}Params;

/** Struttura di parametri solo per coccodrilli 
 * - struct Semaphore* semafori: ptr a struttura che contiene i semafori.
 * - PipeData* buffer : la struttura usata come buffer.
 * - GameData* gameData : il puntatore alla struttura con tutte le informazioni di gioco
 * - coccodrillo_init : struttura per passare al coccodrillo la coordinate iniziali
 * - int id : intero che indica l'identificativo dell' oggetto (diverso dal thread_id)

typedef struct Params_coccodrilli{
	struct Semaphore *semafori;
	PipeData *buffer;
	GameData *gameData;
	PipeData coccodrillo_init; // dati iniziali del coccodrillo
	int id;
}Params_coccodrilli;
*/


typedef struct Params_coccodrilli{
	Params* param;	
	PipeData coccodrillo_init; // dati iniziali del coccodrillo
}Params_coccodrilli;




int generaRandom_r(int min, int max, unsigned int *seed);


/** @brief Gestisce la sincronizzazione e Srive i valori di new_item sul buffer e aggiorna il TCB del thread
 * @param p: Struttura da cui recupera i semafori e buffer
 * @param new_item: l'oggetto con i nuovi valori da scrivere sul buffer
 * @param my_tcb: puntatore al ThreadControlBlock del thread
 * @param terminated_value: viene scritto sul TCB del thread per indicare se il thread sta terminando 
*/
void scriviSuBuffer(Params *p, PipeData new_item, ThreadControlBlock *my_tcb, bool terminated_value);

/** @brief Legge i dati dal buffer e salva i valori in readed
 * @param p: Struttura da cui recupera i semafori e buffer
 * @param readed: Struttura in cui salva i valori letti dal buffer 
*/
void leggiDaBuffer(Params *p, PipeData *readed);

/**	@brief Scrive i valori di new_item nel tcb destinazione, si occupa di sincronizzare gli accessi
 * 	@param param : da dove prende i semafori
 * @param dst_tcb: ptr al TCB destinaznioe
 * @param new_item: TCB sorgente con i valori da copiare
 * @return 0 se successo , -1 altrimenti
*/
int scriviSuTCB(Params *param, ThreadControlBlock* dst_tcb, ThreadControlBlock new_item);


/** @brief Controlla che il ptr a TCB sia diverso da NULL
 * @return TRUE se il TCB è valido, FALSE altrimenti
*/
bool isThreadValid(ThreadControlBlock* thread_tcb, sem_t* semaforo);


/** @brief Controlla che il thread sia impostato per terminare
 * @return Ritorna il valore di is_target (TRUE/FALSE)
*/
bool isThreadTarget(ThreadControlBlock* thread_tcb, sem_t* semaforo);


/**	@brief Controlla che il thread sia terminato
 * @return il valore di is_terminated del TCB
*/
bool isThreadTerminated(ThreadControlBlock* thread_tcb, sem_t* semaforo);

/**	@brief Imposta il TCB a valori di default, usa il semaforo
 * @param thread_tcb: il TCB da risettare
 * @param semaforo: il semaforo usato per la sincronizzazione
 * @return	0 successo, -1 errore
*/
int resetTCB(ThreadControlBlock* thread_tcb, sem_t* semaforo);

/**	@brief Restituisce il valore del thread_id dal TCB passato
 * @param thread_tcb: TCB del thread che voglio leggere
 * @param semaforo:: semaforo per sincronizzazione
 * @return 0 di default oppure il thread_id del thread
*/
pthread_t leggiTid(ThreadControlBlock* thread_tcb, sem_t* semaforo);

/**	@brief Legge un TCB e ne restituisce i valori in 
 * @param thread_tcb: TCB del thread che voglio leggere
 * @param semaforo:: semaforo per sincronizzazione
 * @return Restituisce una struttura di tipo TCB con tutti i valori del TCB letto
*/
ThreadControlBlock leggiTCB(ThreadControlBlock* thread_tcb, sem_t* semaforo);


/** @brief	Cerca un TCB in un vettore, usando il thread_id come chiave di ricerca; 
 * @param array_tcb: il vettore in cui cercare
 * @param thread_id: la chiave di ricerca
 * @param semaforo: il semaforo per sincronizzazione
 * @param dimVettore: dimensione del vettore
 * @return Ritorna il puntatore al TCB trovato, oppure NULL se non trova nulla 
*/
ThreadControlBlock* cercaThreadByTid(ThreadControlBlock* array_tcb, pthread_t thread_id, sem_t* semaforo, int dimVettore);











/* ---- prove--------*/

void cancellaOggettoDaMatrice(GameData *gameData, PipeData *oggetto ,PipeData *old_pos, TipoSprite tipoSprite);


/** @brief Esegue la join sul singolo thread se è terminato
 * @return 0 se esegue la join con successo, -1 se errore o se il thread non è ancora terminato
*/
int joinThreadMorto(ThreadControlBlock* deadThread, sem_t* semaforo);

/** @brief Chiama pthread_join() sui thread terminati
 * @param allSem : ptr a Struttura con i semafori
 * @param vettoreAllTCB : prt a Struttura con tutti i TCB
 * @return 0 se RANA successo, -1 in ogni altro caso di errore
*/
int pulisciThreadMorti( GameData* gameData , struct Semaphore* allSem);



/** @brief Cerca il TCB del thread da un array di TCB e ritorna il puntatore al TCB del thread scelto
 * @param array_tcb : vettore di TCB in cui cercare
 * @param thread_id : (pthread_t) il thread ID da cercare  
 * @param dimVettore : dimensione del vettore in cui cercare  
 * @return Ritorna puntatore a TCB (ThreadControlBlock*) se ha successo, NULL in caso contrario
*/
//ThreadControlBlock* recuperaThreadTCB(ThreadControlBlock* array_tcb, pthread_t thread_id, int dimVettore);


/** @brief Cerca un certo thread in un vettore di TCB e imposta una variabile per dire al thread di terminare 
 * @param array_tcb : vettore di TCB in cui cercare
 * @param thread_id : (pthread_t) il thread ID da cercare  
 * @param dimVettore : dimensione del vettore in cui cercare 
 * @return : 0 se ha successo, -1 se c'è un errore;
*/
//int impostaThreadTarget(ThreadControlBlock* array_tcb, pthread_t target_tid, int dimVettore);


/** @brief Imposta una variabile per dire al thread di terminare 
 * @param thread_tcb : TCB del thread da far terminare
 * @param semaforo : semaforo per sincronizzazione sui TCB
 * @return : 0 se ha successo, -1 se c'è un errore;
*/
int impostaThreadTarget(ThreadControlBlock* thread_tcb,sem_t *semaforo);

/** @brief Copia i valori del TCB sorgente in quello destinatario
 * @param tcb_dest: il TCB destinazione in cui copiare i valori
 * @param tcb_source: il TCB sorgente con i valori da scrivere
 * @param semaforo: semaforo per sincronizzare
 * @return 0 se succasso, -1 se errore
*/
int copiaTCB(ThreadControlBlock* tcb_dest, ThreadControlBlock tcb_source, sem_t* semaforo);


// inizializza ncurses ed i colori utilizzati
void inizializzaNcurses();

/** @brief Inizializza tutti i semafori
 * @param allSem : (struct Semaphore*) puntatore alla struttura dei semafori
*/
void creaSemafori(struct Semaphore* allSem);

/** @brief distrugge tutti i semafori passati
 * @param allSem : (struct Semaphore*) puntatore alla struttura dei semafori  
*/
void distruggiSemafori(struct Semaphore* allSem);


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
int id_disponibile(pthread_t *array_pid, int lunghezza);

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
bool isFrogMoveLecit(int newX, int newY);

/** Chiude i thread attivi per resettare la manche */
void resetManche(Params *p);
void resetManche_2(Params *p);
void terminaTuttiThread(GameData* gameData , struct Semaphore* allSem);

#endif // UTILITIES_H