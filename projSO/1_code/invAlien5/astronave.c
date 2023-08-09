
#include "header.h"
#include "astronave.h"
#include "bullet.h"

/*------Implementare gestione di piu processi contemporaneamente-----------*/
			// Opzione#2 - variabile contatore per i proccessi-bullet  attivi.
			//							+	pipe astronave-bullet e astronave-disegnatore

void killZombie(GameObject *bullet, Setup setup);

void Astronave(int *p, int* pipe_nbullets, Setup setup){			/*	figlio	*/
	
	GameObject astronave;
	GameObject bullet[10];
	pid_t pidBullet;
	int bulletStatus;	// exit status del processo-bullet
	int n_bullets =-1; // numero di processi-bullet attivi
	int bullet_dead=0;
	int diff_bullet = 0;
	//---Opt #2----
	pid_t pid_bullets[10];
	int pipe_bullets[2];		//file_descriptor per la pipe usata dai bullet
	
	GameObject objectReaded;
	
	// inizializzo array di bullet in cui mettere i dati dei bullet letti.
	for(int i=0;i<10; i++){
		bullet[i].x = bullet[i].y = 0;
		bullet[i].dirX = bullet[i].dirY = 0;
		bullet[i].c = ' ';
		bullet[i].id = 0;
	}
	
	
	// crea pipe 
	if(pipe(pipe_bullets) < 0 ){
		perror("pipe failed!");
		_exit(-17);
	}
	
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
		
		//diff_bullet = n_bullets-bullet_dead;
		
		
		if(bulletTimer(&time_old, &time_new, RESETC)) //ogni RESETC secondi deve lanciare un proiettile
		{	
			
			/*
			if(n_bullets > 2){
				wait(&bulletStatus);
				n_bullets--;
				flash();
			}
			/**/
			
			// crea processo bullet
			//n_bullets++;
			n_bullets = (n_bullets+1)%10; 
			pidBullet = fork();
			if(pidBullet < 0){
				perror("Bullet Error!!");
				_exit(-4);
			}
			if(pidBullet == 0){
				// figlio 
				Bullet(p, pipe_bullets, setup, &astronave, n_bullets);
				_exit(0);
			}else{
				// padre 
			}
		
		}// end if(bulletTimer)
		
		
		/*leggiamo da pipe pos_Y dei bullet	*/
			
		// legge da pipe tante volte quanti sono i bullet attivi, controlla se il bullet è ancora nello schermo
		bool dead = false;
		//for(int i=0; i<n_bullets; i++){
		
		if(n_bullets > 0){
			close(pipe_bullets[WRITE_END]);
			read(pipe_bullets[READ_END], &objectReaded, sizeof(GameObject));
			bullet[objectReaded.id] = objectReaded;  //assegna il bullet letto alla sua posizione nell array
		}else{
			if(n_bullets==0){
				killZombie(bullet, setup);
			}
		}//end else
		
		
		
		/*
		if(n_bullets!=0){
			close(pipe_bullets[WRITE_END]);
			read(pipe_bullets[READ_END], &objectReaded, sizeof(GameObject));
			if((!dead)&&(objectReaded.y == setup.screenY)) {		// se il bullet è a fine-corsa fa la wait
				//wait(&bulletStatus);
				dead = true;
				bullet_dead++;
				beep();
			}
		}
		if(bullet_dead > 0){
			//wait(&bulletStatus);
			//n_bullets = n_bullets-bullet_dead;
			bullet_dead--;
			n_bullets--;
			flash();
		}
		/**/
		
		
		
		/* scriviamo su pipe	*/
		// scrive su pipe il numero di bullet attivi
		
		
		/*
		close(pipe_nbullets[READ_END]);
		write(pipe_nbullets[WRITE_END], &n_bullets, sizeof(int));
		/**/
		
		// scive le coordinate di astronave 
		close(p[0]);
		write(p[1], &astronave, sizeof(GameObject));
		
		//n_bullets = n_bullets-bullet_dead;
		
		mvprintw(1, setup.screenX/2 +10, "b_attivi: %d",n_bullets);
		
		usleep(U_DELAY);
	}// end while
}/*	end Astronave	*/



/*	Conta i bullet terminati ed esegue le wait	*/
void killZombie(GameObject *bullet, Setup setup){
	int bullet_dead=0;
	for(int i=0; i<10;i++){
		if(bullet[i].y == setup.screenY){
			bullet_dead++;
		}
	}
	for(bullet_dead; bullet_dead > 1; bullet_dead--){
		wait(NULL);
		beep();
	}
	bullet_dead=0;
}// end killZombie










