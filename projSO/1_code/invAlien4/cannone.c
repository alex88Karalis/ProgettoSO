#include "header.h"
#include "cannone.h"

/*	Provare con vettore di GameObject bullet 
		e controllo sulla y per sapere se sono terminati
*/

void Cannone (int *p, Setup setup){
	
	pid_t cannoneBullet ;
	int cannoneBulletStatus;
	int count_bullet_ended = 0;
	
	GameObject cannone;
	cannone.x = setup.screenX/2; // posizione iniziale
	cannone.y = setup.screenY;
	cannone.c = '^';
	keypad(stdscr,1);
	close(p[0]);
	write(p[1], &cannone, sizeof(GameObject));
	
	
	timeout(0);
	int k;
	while(k != 113){
		k=getch();
		switch(k){
			case KEY_LEFT:
				if(cannone.x > 1) cannone.x--;
				break;
			case KEY_RIGHT:
				if(cannone.x < setup.screenX) cannone.x++;
				break;
		
			case KEY_UP:
				
				if(wait(&cannoneBulletStatus)); // wait su bullet precedente
				/*
				if(count_bullet_ended > 0){
					wait(&cannoneBulletStatus);
				}
				/**/
				/* crea nuovo processo bullet */
				
				cannoneBullet = fork();
				if(cannoneBullet){
					//padre
				}else{
					//figlio bullet
					GameObject c_Bullet;
					c_Bullet.x = cannone.x;
					c_Bullet.y = cannone.y-1;
					c_Bullet.dirX = 0;
					c_Bullet.dirY = -1;
					c_Bullet.c = '*';
					
					while(true){
						c_Bullet.y += c_Bullet.dirY;
						close(p[0]);
						write(p[1], &c_Bullet, sizeof(GameObject));
						usleep(U_DELAY);
						if(c_Bullet.y == 0) break;
					}
					//count_bullet_ended++; // mod alla var locale del figlio
					_exit(0);
				}//fine del figlio
				
				break;
		
		}//end switch-case
			
						
			close(p[0]);
			write(p[1], &cannone, sizeof(GameObject));
	
	}//end while
	
}// end Cannone

