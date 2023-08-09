#include "header.h"

#include "astronave.h"
//#include "cannone.h"
#include "areaGioco.h"


//void Astronave(int *p, Setup setup);
//void Cannone (int *p, Setup setup);

/*	Versione con gestione di piu proiettili		--da finire---*/



int main(){
	int p[2]; /*	array di int come file descriptor*/
	pid_t pidAstronave, pidCannone;
	Setup setup;
	
	initscr();
	getmaxyx(stdscr,setup.screenY, setup.screenX); // defisce dimensioni area di gioco
	//keypad(stdscr,1);
	noecho();
	curs_set(0);
	setup.screenX -= 2;
	setup.screenY -= 2;
	
	int pipe_nbullets[2];
	/* crea pipe per leggere il num di proiettili	*/
	if(pipe(pipe_nbullets) == -1){
		perror("pipe failed");
		_exit(-17);
	}
	
	
	
	
	
	/* crea pipe	*/
	if(pipe(p) == -1){
		perror("pipe failed");
		_exit(-1);
	}
	
	
	pidAstronave = fork();
	if(pidAstronave <0){
		perror("fork failed");
		_exit(-2);
	}
	
	if(pidAstronave==0){	/* figlio Astronave	*/
		//usleep(1000000/3);
		Astronave(p, pipe_nbullets, setup);
		_exit(0);
		
	}else if(pidAstronave>0){	//	padre	
		/*
		pidCannone = fork();
		if(pidCannone <0){
			perror("fork failed");
			_exit(-3);
		}
		if(pidCannone==0){		// cannone	
		
			Cannone(p, setup);
			_exit(0);
		
		}// end cannone
		/**/
		
		
		//------	padre	------
		AreaGioco(p, pipe_nbullets, setup);
		
		/*	uccide i figli	*/
		kill(pidAstronave, 9);
		kill(pidCannone, 9);
		endwin();
			
	}										/* end padre*/
	
	_exit(0);
}										/*	end main */






