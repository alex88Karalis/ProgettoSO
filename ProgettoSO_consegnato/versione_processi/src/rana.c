#include "../hdr/rana.h"

// funzione che si occupa di avviare il processo rana e restituirne il pid
pid_t avviaRana(int* pipe_fd){
	pid_t move_pid = fork(); // pid che contiente il pid della rana
    
  if (move_pid < 0) {
  	perror("Fork failed");
    exit(1);
  } else if (move_pid == 0) {
  		// Processo "muovi"
      close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
      moveProcess(pipe_fd);
      exit(0);
  }
	return move_pid;
}

// il main del processo rana
void moveProcess(int* pipe_fd) {
	// inizializzazione pipeData per comunicazione
	PipeData pipeData;
	// coordinate relative iniziali nulle
	pipeData.x=0;
	pipeData.y=0;
	pipeData.type='X';
	pipeData.id=0;
		
	// Invia le coordinate iniziali attraverso la pipe
    write(pipe_fd[1], &pipeData, sizeof(PipeData));
    
	noecho();
	int ch='D';
    bool change=false; // flag tasto premuto/non premuto
	// loop principale del processo rana
	while (1) {
		
    	change=false; // reset flag tasto premuto
    	pipeData.type='X'; // resetta il normale carattere della rana
       	
        ch = getch(); // Leggi il carattere dall'input
        
        // Muovi il personaggio in base all'input dell'utente
        switch (ch) {
        	case KEY_UP:
			case 'w':
			case 'W':
				pipeData.y=-2;
            	change=true;
            break;
          case KEY_DOWN:
		  case 's':
		  case 'S':
            	pipeData.y=+2;
            	change=true;
            break;
          case KEY_LEFT:
		  case 'a':
		  case 'A':
            	pipeData.x=-2;
            	change=true;  
            break;
          case KEY_RIGHT:
		  case 'd':
		  case 'D':
            	pipeData.x=2;
            	change=true;
            break;
          case 32: // KEY_SPACE 
          	pipeData.type='S'; //cambia carattere per dire a processoDisegna che  rana sta sparando
          	change=true;
            break;
          case 'p': // tasto p  PAUSA
          case 'P':
          	pipeData.type = 'Z';
          	change=true; 
            break;
          default:
          	break;
        }
		
		if(change){
			// Invia le coordinate attraverso la pipe
        	write(pipe_fd[1], &pipeData, sizeof(PipeData));
			// reset coordinate e tipo
			pipeData.x=0;
			pipeData.y=0;
			pipeData.type='X';
			usleep(20000);
		}
         
    }
    return;
}

void inizializzaPosRana(GameData* gameData){
	gameData->ranaAbsPos.x=40;
	gameData->ranaAbsPos.y=MARCIAPIEDEROWSTART;
	//gameData->ranaAbsPos.y=ARGINEROWSTART;
	gameData->ranaAbsPos.on_coccodrillo=false;
	gameData->ranaAbsPos.id_coccodrillo=NOID;
	gameData->ranaAbsPos.offset_on_coccodrillo=0;
	gameData->oldPos.rana.x = gameData->ranaAbsPos.x;
	gameData->oldPos.rana.y = gameData->ranaAbsPos.y;
	return;
}