#include "header.h"
#include "areaGioco.h"
#include "bullet.h"

void AreaGioco(int *p, int* pipe_nbullets, Setup setup){
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
	
	int max_bullets = 10;
	GameObject bullet[max_bullets];
	int b_index=0;
  
	for(int i=0; i<max_bullets; i++){
		bullet[i].y = bullet[i].x = 0;
		bullet[i].c = ' ';
	}
	//Bullet(setup, bullets);
	//--------------------------------Aggiornare lettura da pipe per leggere piu proiettili----------
	
	int bullet_attivi = 10;
	int bullet_astronave = 0;
	
	/*	leggo da pipe	 e disegna	*/
	//int time_old,time_new;
	
	
	close(p[1]);
	read(p[0], &figlio, sizeof(GameObject));
	
	if(figlio.c == '^'){
		figlioAstronave = figlio;
	}else if( figlio.c == '#'){
		figlioCannone = figlio; }
	
	//bool bulletLife=true;
	bool flag=true;
	while(vite > 0){
		
		/*
		// ogni tot secondi legge il numero di bullet attvi 
		if(bulletTimer(&time_old, &time_new, RESETC)) {	
			close(pipe_nbullets[WRITE_END]);
			read(pipe_nbullets[READ_END], &bullet_astronave, sizeof(int)); //leggi quanti bullet attivi
		}
		/**/
		/*
		close(pipe_nbullets[WRITE_END]);
		read(pipe_nbullets[READ_END], &bullet_astronave, sizeof(int)); //leggi quanti bullet attivi
		/**/
		
		
		
		for(int i=0; i<bullet_attivi; i++){				// per tutti i bullet attivi...
			// se il proiettile è attivo sullo schermo, cancella vecchia pos
			if(bullet[i].c != ' ') 
				mvaddch(bullet[i].y, bullet[i].x, ' ');
		}
		
		
		if(figlioCannoneBullet.c != ' ')
			mvaddch(figlioCannoneBullet.y, figlioCannoneBullet.x, ' ');
		
			
		mvaddch(figlioAstronave.y, figlioAstronave.x, ' ');	/*	cancella carattere alla pos precedente	*/
		mvaddch(figlioCannone.y, figlioCannone.x, ' ');
		fflush(NULL);
		
		//------leggere da pipe tantevolte quante gli oggetti produttori/bullets--------
		
		
		/*
		// leggi da pipe tante volte quanti sono i processi attivi
		for(int i=0; i < 2; i++){
			close(p[1]);
			read(p[0], &figlio, sizeof(GameObject));
			
			// smista in base al figlio letto da pipe
			if(figlio.c == '='){				// hai letto astronave
				figlioAstronave = figlio;
			}else if( figlio.c == '^'){	// hai letto cannone
				figlioCannone = figlio; 
			}else if( figlio.c == 'o'){	// hai letto proiettile
				bullet[i] = figlio;
			}else if(figlio.c = '*'){
				figlioCannoneBullet = figlio; // hai letto proiettile di cannone
			}
		
		}// fine lettura da pipe
		/**/
		
		close(p[1]);
		read(p[0], &figlio, sizeof(GameObject));
		
		// smista in base al figlio letto da pipe
		if(figlio.c == '='){				// hai letto astronave
			figlioAstronave = figlio;
		}else if( figlio.c == '^'){	// hai letto cannone
			figlioCannone = figlio; 
		}else if( figlio.c == 'o'){	// hai letto proiettile
			// --- far distinguere diversi proiettili-----
			b_index = figlio.id;
			bullet[b_index] = figlio;
			//b_index = (b_index+1)%3;
		}else if(figlio.c = '*'){
			figlioCannoneBullet = figlio; // hai letto proiettile di cannone
		}
		
		
		
		
		/*
		if(figlioBullet.c == ' '){
		}else if(figlioBullet.c == 'o')
		{
				if((figlioCannone.x == figlioBullet.x) && (figlioCannone.y == figlioBullet.y))
				{
					hideBullet(setup, &figlioBullet);
					vite--;
				}
		}
		/**/
		
		for(int i=0; i< bullet_attivi; i++){	// per ogni bullet attivo 
			if(bullet[i].y >= setup.screenY)		// controlla che sia dentro lo schermo
				hideBullet(setup, &bullet[i]);
		}
		
		
		if((figlioCannoneBullet.x == figlioAstronave.x) && (figlioCannoneBullet.y == figlioAstronave.y))
			vite = 6;
		
	
		/*------	Disegna	-----------------------*/
		
		mvaddch(figlioCannoneBullet.y, figlioCannoneBullet.x, figlioCannoneBullet.c);
		
		
		mvaddch(figlioCannone.y, figlioCannone.x, figlioCannone.c);
		mvaddch(figlioAstronave.y, figlioAstronave.x, figlioAstronave.c);
		
		
		for(int i=0; i< bullet_attivi; i++){
			mvaddch(bullet[i].y, bullet[i].x, bullet[i].c);
		}
		
		
		mvprintw(1,30,"bulletY: %3d, madreY:%3d, cannY:%3d, c_bulletY:%3d",figlioBullet.y, figlioAstronave.y, figlioCannone.y, figlioCannoneBullet.y);
		mvprintw(1, setup.screenX/2 +20, "b_attivi: %d",bullet_astronave);
		mvprintw(1, setup.screenX/2 +10, "last_b_index: %d",b_index);
		
		mvprintw(1,1,"vite: %d, time:%d",vite, time(NULL));
		//fflush(NULL);
		
		
		
		box(stdscr, ACS_VLINE, ACS_HLINE);
		refresh();
		
		//close(pipe_nbullets[WRITE_END]);
		//read(pipe_nbullets[READ_END], &bullet_attivi, sizeof(int)); //leggi quanti bullet attivi
		
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

