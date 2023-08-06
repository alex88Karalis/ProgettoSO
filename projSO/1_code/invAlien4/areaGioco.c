#include "header.h"
#include "areaGioco.h"
#include "bullet.h"

void AreaGioco(int *p, Setup setup){
	GameObject figlioAstronave;
	GameObject figlioCannone;
	GameObject figlio;
	GameObject figlioBullet;
	GameObject figlioCannoneBullet;
	
	
	char *gameOverMsg = "Game Over !";
	char *winMsg = "YOU WIN !";
	int vite = 3;
	
	int time_old;
	int time_new;
	time_new = time_old = time(NULL);
	
	figlio.x = figlio.y = 0;
	figlioAstronave.x = figlioAstronave.y = 0;
	figlioCannone.x = figlioCannone.y = 0;
	figlioBullet.c = ' ';
	figlioBullet.y =0;
	//GameObject bullets[3];
	//Bullet(setup, bullets);
	
	
	/*	leggo da pipe	 e disegna	*/
	
	close(p[1]);
	read(p[0], &figlio, sizeof(GameObject));
	
	if(figlio.c == '^'){
		figlioAstronave = figlio;
	}else if( figlio.c == '#'){
		figlioCannone = figlio; }
	
	//bool bulletLife=true;
	bool flag=true;
	while(vite > 0){
		// se il proiettile è attivo sullo schermo, cancella vecchia pos
		if(figlioBullet.c != ' ') 
			mvaddch(figlioBullet.y, figlioBullet.x, ' ');
		
		if(figlioCannoneBullet.c != ' ')
			mvaddch(figlioCannoneBullet.y, figlioCannoneBullet.x, ' ');
		
			
		mvaddch(figlioAstronave.y, figlioAstronave.x, ' ');	/*	cancella carattere alla pos precedente	*/
		mvaddch(figlioCannone.y, figlioCannone.x, ' ');
		fflush(NULL);
		
		close(p[1]);
		read(p[0], &figlio, sizeof(GameObject));
		
		if(figlio.c == '='){				// hai letto astronave
			figlioAstronave = figlio;
		}else if( figlio.c == '^'){	// hai letto cannone
			figlioCannone = figlio; 
		}else if( figlio.c == 'o'){	// hai letto proiettile
			figlioBullet = figlio;
		}else if(figlio.c = '*'){
			figlioCannoneBullet = figlio; // hai letto proiettile di cannone
		}
		
		
		if(figlioBullet.c == ' '){
		}else if(figlioBullet.c == 'o')
		{
				if((figlioCannone.x == figlioBullet.x) && (figlioCannone.y == figlioBullet.y))
				{
					hideBullet(setup, &figlioBullet);
					vite--;
				}
		}
		
		if(figlioBullet.y >= setup.screenY)	
			hideBullet(setup, &figlioBullet);
		
		
		if((figlioCannoneBullet.x == figlioAstronave.x) && (figlioCannoneBullet.y == figlioAstronave.y))
			vite = 6;
		
	
		/*------	Disegna	-----------------------*/
		
		mvaddch(figlioCannoneBullet.y, figlioCannoneBullet.x, figlioCannoneBullet.c);
		mvaddch(figlioBullet.y, figlioBullet.x, figlioBullet.c);
		
		mvaddch(figlioCannone.y, figlioCannone.x, figlioCannone.c);
		mvaddch(figlioAstronave.y, figlioAstronave.x, figlioAstronave.c);
		mvprintw(1,30,"bulletY: %3d, madreY:%3d, cannY:%3d, c_bulletY:%3d",figlioBullet.y, figlioAstronave.y, figlioCannone.y, figlioCannoneBullet.y);
		mvprintw(1,1,"vite: %d, time:%d",vite, time(NULL));
		//fflush(NULL);
		
		
		box(stdscr, ACS_VLINE, ACS_HLINE);
		refresh();
		//fflush(NULL);
		if(vite == 6) break; //hai vinto!
		
	}
	/*	termina gioco		*/
	clear();
	box(stdscr, ACS_VLINE, ACS_HLINE);
	if(vite == 6)
		mvprintw(setup.screenY/2, setup.screenX/2-(strlen(winMsg)/2), winMsg);	//hai vinto!
	else
		mvprintw(setup.screenY/2, setup.screenX/2-(strlen(gameOverMsg)/2), gameOverMsg);	// hai perso!
	refresh(); 
	sleep(3);
	
}// end AreaGioco

