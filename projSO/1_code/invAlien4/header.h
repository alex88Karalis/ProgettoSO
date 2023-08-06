#include <unistd.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h> /* per lavorare con sprite */



# define U_DELAY 1000000/5 	/*	(fps)  ritardo in microsecondi per la usleep()		*/ 
# define CICLI  100				/*	numero iterazioni per padre e figlio			*/
#define RESETC 5 				// reset counter per i bullet

typedef struct {
	int screenX;
	int screenY;
}Setup;


typedef struct{
	int x;
	int y;
	int dirX;
	int dirY;
	char c;
	//int vite;
}GameObject;



