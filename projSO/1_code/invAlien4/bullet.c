#include "header.h"
#include "bullet.h"


// MIN + rand()%(MAX-MIN+1)


void Bullet(int* p, Setup setup, GameObject* astronaveMadre){
	GameObject bullet;	// proiettile
	// inizializza proiettile
	bullet.x = astronaveMadre->x;
	bullet.y = astronaveMadre->y +1;
	bullet.dirX = 0;
	bullet.dirY = 1;
	bullet.c = 'o';
	
	while( bullet.y < setup.screenY){
		bullet.y += bullet.dirY; 
	
		close(p[0]);
		write(p[1], &bullet, sizeof(GameObject));
		usleep(U_DELAY);
	}
	
	return;
}//end bullet

/*
	Imposta caratteristiche e posizione della Bullet
*/
void setBullet(Setup setup, GameObject* bullet){
	bullet->c = 'o';
	bullet->x = 1+ rand()%setup.screenX;
	bullet->y = 2+ rand()%(setup.screenY -1);
	bullet->dirX = 0;
	bullet->dirY = 0;
	
}// end setBullet	


void hideBullet(Setup setup, GameObject* bullet){
	bullet->c = ' ';
}


/*
	Prende un vettore di bullet e ne cancella il carattere
*/
void hideAllBullet(Setup setup, GameObject* bullet){
	for(int i=0; i<3 ;i++){
		mvaddch(bullet[i].y, bullet[i].x, ' '); 
	}
}


bool bulletTimer(int* time_old, int* time_new, int timer){
	*time_new = time(NULL);
	if((*time_new - *time_old) >= timer){
		*time_old = *time_new;
		return true;
	} 
	return false; 
}



	
