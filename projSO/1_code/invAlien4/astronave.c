
#include "header.h"
#include "astronave.h"
#include "bullet.h"

void Astronave(int *p, Setup setup){			/*	figlio	*/
	
	GameObject astronave;
	GameObject bullet;
	pid_t pidBullet;
	int bulletStatus;	// exit status del processo-bullet
	int n_bullet =0; // numero di processi-bullet attivi
	
	
	
	srand(time(NULL));
	astronave.c = '=';
	astronave.x = setup.screenX/2;	/*	posizione iniziale	*/
	astronave.y = 2;
	astronave.dirX = rand()%3 -1;
	astronave.dirY = 0;
	
	while(astronave.dirX == 0){
		astronave.dirX = rand()%3 -1;
	}
	
	int time_old,time_new;
	time_old = time_new = time(NULL);
	
	bool flag=true; // finchè il gioco prosegue
	while(flag){
		astronave.x += astronave.dirX;												//aggiorna posizione
		if(astronave.x == 1 || astronave.x == setup.screenX){	//inverti direzione al bordo
			astronave.dirX*=-1;
		}
		
		
		if(bulletTimer(&time_old, &time_new, RESETC)) //ogni RESETC secondi
		{	
			
			/*------Implementare gestione di piu processi contemporaneamente-----------*/
			// Opzione#1 - variabile contatore per i proccessi-bullet  attivi.
			
			if(n_bullet > 2){
				wait(&bulletStatus);
				n_bullet--;
				flash();
			}
			
			
			//if(	wait(&bulletStatus)	) ; // esegue la wait sul bullet appena finito
			
			
			// crea processo bullet
			n_bullet++; 
			pidBullet = fork();
			if(pidBullet < 0){
				perror("Bullet Error!!");
				_exit(-4);
			}
			if(pidBullet == 0){
				// figlio 
				Bullet(p, setup, &astronave);
				_exit(0);
			}else{
				// padre 
			}
		
		}// end if(bulletTimer)
		
		
		
		
		/* scriviamo su pipe	*/
		close(p[0]);
		write(p[1], &astronave, sizeof(GameObject));
		
		usleep(U_DELAY);
	}// end while
}/*	end Astronave	*/

