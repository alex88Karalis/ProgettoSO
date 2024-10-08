#include "disegna.h"

//#define DEBUG 

// per far partire il processo che si occupa di disegnare
void avviaDrawProcess(int pipe_fd[2]) {
    pid_t draw_pid = fork();
    if (draw_pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (draw_pid == 0) {
        // Processo "disegna"
        //close(pipe_fd[1]); // Chiudi l'estremità di scrittura della pipe, TODO: è davvero utile?
        drawProcess(pipe_fd);
        exit(0); // TODO: è davvero utile?
    }
}

// processo che si occupa di disegnare lo schermo
void drawProcess(int* pipe_fd) {
	
	time_t start_time, current_time; //usate per il timer
	double diff_time;
	int diff_time_sec;
	time(&start_time);
	time(&current_time);
	
	/*
	int tempo = 60;
	int vite = 4;
	int punteggio = 0;
	int livello = 1;
	/**/
	int taneChiuse = 0;
	bool morteRana = false;
	bool vittoriaRana = false;
	bool gameOver = false;
	bool gameWin = false;

	GameHUD gameHud;	// viene inizializzato nalla fuunz. inizializzaMatriceSchermo()
	//inizializzaGameHUD(&gameHud);
	
	int arrayDirTronchi[4]; // vettore per registrare la direzione di chiascun tronco
	int pipeRana_fd [2];
	creaPipe(pipeRana_fd);
	
	pid_t pidRana;
	pidRana = avviaRana(pipe_fd, pipeRana_fd); // avvia il processo che gestisce il movimento della rana
	
	pid_t pid_veicoli[8];
	gestoreMacchine(pipe_fd,pid_veicoli); // avvia le auto e i camion
	
	pid_t pid_tronchi[3];
	gestoreTronchi(pipe_fd,pid_tronchi); // avvia i tronchi
	
	PipeData pipeData; // struttura per leggere la pipe
	
	PipeData old_pos [OLDPOSDIM]; // array di strutture che contiene dati oggetti al passo precedente; rana, tronchi x3, auto x4, ...
	inizializzaOldPos(old_pos,OLDPOSDIM);
	
	PipeData old_pos_proiettili[MAXNPROIETTILI];
	inizializzaOldPosProiettili(old_pos_proiettili,MAXNPROIETTILI);
	int contatore_proiettili_in_gioco=0;
	pid_t array_pid_proiettili[MAXNPROIETTILI];
	
	PipeData old_pos_proiettili_nemici[MAXNPROIETTILINEMICI];
	inizializzaOldPosProiettili(old_pos_proiettili_nemici,MAXNPROIETTILI);
	int contatore_proiettili_nemici_in_gioco=0;
	pid_t array_pid_proiettili_nemici[MAXNPROIETTILINEMICI];
	
	PipeData old_pos_nemici[3];
	//inizializzaOldPosNemici(old_pos_nemici,3);
	int contatore_nemici_in_gioco=0;
	pid_t array_pid_nemici[3];
	
	// strutture di tane
	Tana tane[5];
	for(int i =0; i<5; i++){
		tane[i].info.y = 6;
		tane[i].info.x = i*22;
		tane[i].info.id = i;
		tane[i].status = 0;
	}
	
	// definizione sprite utili
	char *sprite_rana[RANA_ROWS] = {"\\./","/-\\"};
	char *sprite_tronco[TRONCO_ROWS] = {"~ ~ ~ ~ ~", " ~ ~ ~ ~ "};
	char *sprite_auto[AUTO_ROWS] = {"xxxx", "oooo"};
	char *sprite_camion[CAMION_ROWS] = {"xxxxxxx", "ooooooo"};
	char *sprite_proiettile[PROIETTILE_ROWS]={"^","|"};
	char *sprite_proiettile_nemico[PROIETTILE_ROWS]={"|","v"};
	char *sprite_nemico[2]={" ^ ","/ \\"};
	
	char *sprite_tana_open[TANA_ROWS] = {"           ", "           ", "           "};
	char *sprite_tana_closed[TANA_ROWS] = {"H         H", "H         H", "HHHHHHHHHHH"};
	
	
	Sprite ranaSprite = {RANA_ROWS, RANA_COLS, sprite_rana, RANA};
  Sprite troncoSprite = {TRONCO_ROWS, TRONCO_COLS, sprite_tronco, TRONCHI};
  Sprite autoSprite = {AUTO_ROWS, AUTO_COLS, sprite_auto, AUTO};
  Sprite camionSprite = {CAMION_ROWS, CAMION_COLS, sprite_camion, AUTO};
  Sprite proiettileSprite = {PROIETTILE_ROWS, PROIETTILE_COLS, sprite_proiettile, PROIETTILE};
  Sprite proiettileNemicoSprite = {PROIETTILE_ROWS,PROIETTILE_COLS,sprite_proiettile_nemico,PROIETTILE};
  Sprite nemicoSprite = {2,3,sprite_nemico,RANA};
						//--------
	Sprite tanaApertaSprite = {TANA_ROWS, TANA_COLS, sprite_tana_open, TANE};
	Sprite tanaChiusaSprite = {TANA_ROWS, TANA_COLS, sprite_tana_closed, TANE};
	Sprite taneSprite [2];
	taneSprite[OPEN] = tanaApertaSprite;
	taneSprite[CLOSED] = tanaChiusaSprite;
	
	
	// vettore di sprite degli oggetti di gioco, usato per rilevazione delle collisioni
	Sprite spriteOggetto[7];
  spriteOggetto[RANA_SPRITE] = ranaSprite;
  spriteOggetto[TRONCO_SPRITE] = troncoSprite;
  spriteOggetto[AUTO_SPRITE] = autoSprite;
  spriteOggetto[CAMION_SPRITE] = camionSprite;
	spriteOggetto[PROIETTILE_SPRITE] = proiettileSprite;
  spriteOggetto[PROIETTILE_NEMICO_SPRITE] = proiettileNemicoSprite;
  spriteOggetto[NEMICO_SPRITE] = nemicoSprite;
	
  ScreenCell screenMatrix[HEIGHT][WIDTH]; // matrice che rappresenta lo schermo
	ScreenCell staticScreenMatrix[HEIGHT][WIDTH]; // matrice degli elementi statici dello schermo
	inizializzaMatriceSchermo(&gameHud, screenMatrix, staticScreenMatrix); // inizializza entrambe le matrici
	
	
	//PipeData rana_mod; //var di supporto
	bool troncoCollision = false;
	
	while ((!gameOver) && (!gameWin)) {
  		read(pipe_fd[0], &pipeData, sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe
    
	#ifdef DEBUG 
		// test zona
		mvprintw(37,2,"                                                       ");
		mvprintw(37,2,"pid rana: %d",pidRana);
		mvprintw(36,2,"                                                                                                                           ");
		mvprintw(36,2,"pid proiettili: ");
		for(int i=0;i<MAXNPROIETTILI;i++){
			mvprintw(36,18+(i*11),"pid%d: %d",i,array_pid_proiettili[i]);
		}
		mvprintw(35,2,"                                                                                                                           ");
		mvprintw(35,2,"pid proiettili nemici: ");
		for(int i=0;i<MAXNPROIETTILINEMICI;i++){
			mvprintw(35,26+(i*11),"pid%d: %d",i,array_pid_proiettili_nemici[i]);
		}
		mvprintw(34,2,"                                                                                                                           ");
		mvprintw(34,2,"pid nemici: ");
		for(int i=0;i<MAXNNEMICI;i++){
			mvprintw(34,15+(i*11),"pid%d: %d",i,array_pid_nemici[i]);
		}
		mvprintw(33,2,"                                                                                                                           ");
		mvprintw(33,2,"pid veicoli: ");
		for(int i=0;i<8;i++){
			mvprintw(33,15+(i*11),"pid%d: %d",i,pid_veicoli[i]);
		}
		
		mvprintw(38,2,"                                                                                                                           ");
		mvprintw(38,2,"pid tronchi: ");
		for(int i=0;i<3;i++){
			mvprintw(38,15+(i*11),"pid%d: %d",i,pid_tronchi[i]);
		}
		
		mvprintw(37,110,"                        ");
		mvprintw(37,110,"proiettili in gioco: %d",contatore_proiettili_in_gioco);
		mvprintw(38,110,"                               ");
		mvprintw(38,110,"nemici in gioco: %d",contatore_nemici_in_gioco);
		mvprintw(39,110,"                                  ");
		mvprintw(39,110,"proiettili nemici in gioco: %d",contatore_proiettili_nemici_in_gioco);
		// fine test zona
	#endif
    
		
    
		int next_rana_bullet_id=0;
		int next_enemy_bullet_id = 0;
		int next_enemy_id = 0;
		
		switch(pipeData.type){
			case 'X':
				if(troncoCollision)
				{
					
				}else{
					aggiornaPosizioneOggetto(&pipeData, &old_pos[0], screenMatrix, staticScreenMatrix, &ranaSprite);
					//aggiornaPosizioneOggetto(&rana_new, &old_pos[0], screenMatrix, staticScreenMatrix, &ranaSprite);
				}
				#ifdef DEBUG
				mvprintw(0,110,"                                    ");
				//mvprintw(0,110,"RANA tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
				mvprintw(0,110,"RANA tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type, old_pos[0].x,old_pos[0].y, pipeData.id);
				#endif
				break;
			case 'T':
				aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix,staticScreenMatrix, &troncoSprite);
				aggiornaDirezioneTronchi( &pipeData, &old_pos[pipeData.id], arrayDirTronchi);

				if((contatore_nemici_in_gioco > 0) && (contatore_nemici_in_gioco <= MAXNNEMICI))
				{
					/*	se ci sono nemici, disegna nemici sui tronchi */
					int id_tronco = pipeData.id; // id_tronco == nemico.id +1; old_pos_nemici[0,1,2] ; tronchi == old_pos[1,2,3];  
					int indice_nemico = id_tronco-1;  // 
					//prende la pos del tronco, crea su tronco nuovo oggetto nemico,
					PipeData new_pos_nemico = {old_pos[id_tronco].x +2, old_pos[id_tronco].y, 'n', id_tronco };
					
					// pulisce vecchia posizione del nemico, aggiorna la posizione nemico, stampa posizione nemico in matrice
					//pulisciSpriteInMatrice(old_pos_nemici[indice_nemico].y, old_pos_nemici[indice_nemico].x, &nemicoSprite, screenMatrix, staticScreenMatrix);
					aggiornaOldPos(&old_pos_nemici[indice_nemico], &new_pos_nemico);
					stampaSpriteInMatrice(old_pos_nemici[indice_nemico].y, old_pos_nemici[indice_nemico].x, &nemicoSprite, screenMatrix);
				}
				
				#ifdef DEBUG
				mvprintw(pipeData.id,110,"                                    ");
				mvprintw(pipeData.id,110,"TRONCO tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
				#endif
				break;
			case 'A':
				aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix, &autoSprite);
				#ifdef DEBUG
				mvprintw(pipeData.id,110,"                                    ");
				mvprintw(pipeData.id,110,"AUTO tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
				#endif
				break;
			
			case 'C':  // legge il camion da pipe e aggiorna posizione
				aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix, &camionSprite);
				#ifdef DEBUG
				mvprintw(pipeData.id,110,"                                    ");
				mvprintw(pipeData.id,110,"CAMION tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
				#endif
				break;
			case 'c':
				// NON disegnare il camion per un certo tempo, quando esce dallo schermo
				break;
			case 'S':
				//proiettile sparato da utente
				// avvia il proiettile con posizione iniziale della rana (o dell oggetto che ha sparato)
				if(contatore_proiettili_in_gioco<MAXNPROIETTILI)	// se si hanno ancora munizioni
				{ 
					// incremento contatore e faccio partire il processo proiettile, salvo il pid del processo
					next_rana_bullet_id = id_disponibile(array_pid_proiettili,MAXNPROIETTILI);
					if(next_rana_bullet_id != -1) //se c'è un id disponibile
					{
						array_pid_proiettili[next_rana_bullet_id]=avviaProiettile(pipe_fd, &pipeData, next_rana_bullet_id);
						contatore_proiettili_in_gioco++;
					}
				}
				break;
			case 'N':										// hai letto tronco che diventa troncoNemico
				if(contatore_nemici_in_gioco < MAXNNEMICI){
					//assert(contatore_nemici_in_gioco < MAXNNEMICI);
					int nemico_id = pipeData.id-1; // nemico_id == tronco.id -1

					if(old_pos_nemici[nemico_id].type != 'N')	//se su questo tronco non ci sono nemici
					{
						/*	crea nemico	*/
						
						old_pos_nemici[nemico_id]=(PipeData){pipeData.x +2, pipeData.y, 'N', pipeData.id};
						
						//avvia processoNemico	
						array_pid_nemici[nemico_id]=avviaNemico(pipe_fd,&pipeData, nemico_id);
						
						contatore_nemici_in_gioco++;	// aggiorna contatore
					}
				}
				break;
			case 'n':							// hai letto processoNemico 
				//		aggiorna old_pos_nemici
				
				/* 	------	disegna nemico -------------
				;
				int id_tronco = pipeData.id +1; // id_tronco == nemico.id +1
				
				PipeData new_pos_nemico = {old_pos[id_tronco].x +2 , old_pos[id_tronco].y, 'n', id_tronco };
				
				pulisciSpriteInMatrice(old_pos_nemici[pipeData.id].y, old_pos_nemici[pipeData.id].x, &nemicoSprite, screenMatrix, staticScreenMatrix);
				aggiornaOldPos(&old_pos_nemici[pipeData.id], &new_pos_nemico);
				stampaSpriteInMatrice(old_pos_nemici[pipeData.id].y, old_pos_nemici[pipeData.id].x, &nemicoSprite, screenMatrix);
				/**/
				//---------------------------------------------------------------
				/*
				if(contatore_nemici_in_gioco<MAXNNEMICI)  // se non si è raggiunto il numero massimo di nemici
				{ 
					// incremento contatore e faccio partire il processo nemico, salvo il pid del processo
					next_enemy_id = id_disponibile(array_pid_nemici,MAXNNEMICI);
					if(next_enemy_id != -1){
						array_pid_nemici[next_enemy_id]=avviaNemico(pipe_fd,&pipeData, next_enemy_id);
						contatore_nemici_in_gioco++;
					}    		
				}
				/**/
				break;
			case 's':
				// proiettile nemico sparato
				if(contatore_proiettili_nemici_in_gioco<MAXNPROIETTILINEMICI) // se non si è raggiunto il numero massimo di nemici
				{ 
					// incremento contatore e faccio partire il processo nemico, salvo il pid del processo
					next_enemy_bullet_id = id_disponibile(array_pid_proiettili_nemici,MAXNPROIETTILINEMICI);
					if( next_enemy_bullet_id != -1)
					{
						PipeData nemicoShoooter = old_pos_nemici[pipeData.id]; 
						array_pid_proiettili_nemici[next_enemy_bullet_id]= avviaProiettileNemico(pipe_fd, &nemicoShoooter, next_enemy_bullet_id);
						contatore_proiettili_nemici_in_gioco++;
					}
				}
				break;
			case 'P':
				// nuove coordinate proiettile
				// se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
				#ifdef DEBUG
				mvprintw(13+pipeData.id,110,"                                       ");
				mvprintw(13+pipeData.id,110,"PROIETTILE tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
				#endif
				if(pipeData.y<0){
					#ifdef DEBUG
						mvprintw(13+pipeData.id,110,"                                    ");
						mvprintw(13+pipeData.id,110,"id: %d uccisione proiettile con pid: %d",pipeData.id,array_pid_proiettili[pipeData.id]);
					#endif
					
					uccidiProiettile( array_pid_proiettili, pipeData.id); // uccide il processo proiettile
					cancellaOggetto(old_pos_proiettili, &proiettileSprite, screenMatrix, staticScreenMatrix, pipeData.id);
					contatore_proiettili_in_gioco--;
					
				}
				else{
					aggiornaPosizioneOggetto(&pipeData, &old_pos_proiettili[pipeData.id], screenMatrix, staticScreenMatrix, &proiettileSprite);
				}
				break;
			case 'p':
				// nuove coordinate proiettile nemico
				// se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
				#ifdef DEBUG
					mvprintw(25+pipeData.id,110,"                                       ");
					mvprintw(25+pipeData.id,110,"PROIETTILEN tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
				#endif
				if(pipeData.y>30){
					#ifdef DEBUG
						mvprintw(25+pipeData.id,110,"                                    ");
						mvprintw(25+pipeData.id,110,"id: %d uccisione proiettile con pid: %d",pipeData.id,array_pid_proiettili[pipeData.id]);
					#endif
					uccidiProiettileNemico( array_pid_proiettili_nemici, pipeData.id); // uccide il processo proiettile
					cancellaOggetto(old_pos_proiettili_nemici, &proiettileNemicoSprite, screenMatrix, staticScreenMatrix, pipeData.id);
					contatore_proiettili_nemici_in_gioco--;
				
				}
				else{
					aggiornaPosizioneOggetto(&pipeData, &old_pos_proiettili_nemici[pipeData.id], screenMatrix, staticScreenMatrix, &proiettileNemicoSprite);
				}
				break;
			case 'Z':
				beep();
				// funzione che mette in pausa
				// funzione che visualizza menu
				// exit del menu -> intero con scelta
				// gli vanno passati tutti i pid che deve mettere in pausa
				
				//int scelta = pausa(pidRana);
				stampaRefreshMatrice(screenMatrix);
				// se preme continua allora stampare tutta la matrice dinamica
				// se preme esci termire tutti i processi
				
				
				// switch su scelta
				break;
			default:
				break;
		}//end switch-case su type

		
		/*
		for(int i=0; i<MAXNNEMICI; i++){
			if(old_pos_nemici[i].type == 'N'){
				pulisciSpriteInMatrice(old_pos_nemici[i].y, old_pos_nemici[i].x, &nemicoSprite, screenMatrix, staticScreenMatrix);
				//stampaSpriteInMatrice(old_pos_nemici[i].y, old_pos_nemici[i].x, &nemicoSprite, screenMatrix);
			}
		}
		for(int i=0; i<MAXNNEMICI; i++){
			if(old_pos_nemici[i].type == 'N'){
				;
				old_pos_nemici[i].x = old_pos_nemici[i].x + arrayDirTronchi[i+1];
				;
				old_pos_nemici[i].y = old_pos_nemici[i].y;
				stampaSpriteInMatrice(old_pos_nemici[i].y,old_pos_nemici[i].x, &nemicoSprite, screenMatrix);
			}
		}
		/**/
		//assert(troncoCollision==false);

		//perror("Error on contatoreNemici");
		/*
		PipeData nemico;
		for(int i=0; i<contatore_nemici_in_gioco; i++){
			//assert (contatore_nemici_in_gioco <= MAXNNEMICI);
			nemico = old_pos_nemici[i];
			//aggiorna pos nemico in base alla direzione del tronco 
			nemico.x = nemico.x + arrayDirTronchi[nemico.id] ; 
			if(arrayDirTronchi[nemico.id]!=0){
				// stampa sprite nemico
				stampaSpriteInMatrice(nemico.y,nemico.x, &nemicoSprite, screenMatrix);
			}
			
			old_pos_nemici[i] = nemico;	//aggiorna la posizione del nemico in array;
		}
		/**/
    	//-------------------------collisioni rana------
		
		bool autoCollision = false;
		bool tanaApertaCollision = false;
		bool tanaChiusaCollision = false;
		bool autoProiettiliNemiciCollision = false;
		bool autoProiettileCollision = false;
		bool fiumeCollision = false;
		bool proiettiliCollision = false;
		
		int  enemyBulletCollision = -1; // indice del proiettileNemico che colpisce la Rana
		int  proiettileNemico_id = -1;
		int  proiettileRana_id = -1;
		
		fiumeCollision = checkRanaFiume(old_pos, spriteOggetto); // restituisce TRUE se la Rana cade nel fiume
		troncoCollision = checkRanaTronco(old_pos, spriteOggetto); //collisione Rana-Tronco
		autoCollision = checkRanaVeicolo(old_pos, spriteOggetto); //collisione Auto - Rana
		
		//----------------------------------COLLISIONI PROIETTILI-----------------------------
		//collisione Auto-Proiettili
		if(contatore_proiettili_nemici_in_gioco > 0)	//se ci sono proiettiliNemici in gioco
		{
			//check collisione Auto-ProiettileNemici
			autoProiettiliNemiciCollision = checkAutoProiettile(old_pos, old_pos_proiettili_nemici, 
																												spriteOggetto, PROIETTILE_NEMICO_SPRITE);
					//collisione Rana-ProiettileNemico, ritorna id proiettile (0,1,2) o -1 se non c'è collisone
			enemyBulletCollision= collisioneProiettiliNemici(old_pos, old_pos_proiettili_nemici, spriteOggetto);
		}
		//collisione Auto-ProiettileRana TIP: aspettare che compaiano tutti veicoli prima di sparare 
		if(contatore_proiettili_in_gioco > 0){
			autoProiettileCollision = checkAutoProiettile(old_pos, old_pos_proiettili, spriteOggetto, PROIETTILE_SPRITE);
		}
		if((contatore_proiettili_in_gioco > 0) && (contatore_proiettili_nemici_in_gioco > 0)) //se ci sono proiettili in gioco
		{
			// controlla se collidono
			proiettiliCollision = checkProiettileNemicoProiettile( old_pos_proiettili, old_pos_proiettili_nemici, 
																		spriteOggetto, PROIETTILE_SPRITE, PROIETTILE_NEMICO_SPRITE);
			
			if(proiettiliCollision) //se qualche proiettile a colliso con un altro
			{
				//identifica quali proiettili hanno colliso, ritorna indice del proiettile(0,1,2) o -1 
				proiettileNemico_id = collisioneProiettileNemicoProiettile( old_pos_proiettili_nemici, old_pos_proiettili, 
																		spriteOggetto, PROIETTILE_NEMICO_SPRITE, PROIETTILE_SPRITE);
				
				proiettileRana_id = collisioneProiettileNemicoProiettile( old_pos_proiettili, old_pos_proiettili_nemici,
																		spriteOggetto, PROIETTILE_SPRITE, PROIETTILE_NEMICO_SPRITE);
			}
		}
		
		tanaChiusaCollision = checkRanaTanaChiusa(old_pos, tane, spriteOggetto, taneSprite);
		tanaApertaCollision = checkRanaTanaAperta(old_pos, tane, spriteOggetto, taneSprite);
		
		if(proiettiliCollision) // se c'è collisione tra proiettili
		{	
			if((proiettileNemico_id != -1) && (proiettileRana_id != -1)) // e conosco queli proiettili hanno colliso
			{
				uccidiProiettileNemico( array_pid_proiettili_nemici, proiettileNemico_id); // uccide il processo proiettile
				uccidiProiettile( array_pid_proiettili, proiettileRana_id);
				// cancella sprite dei proiettili dalla matrice
				cancellaOggetto(old_pos_proiettili_nemici, &proiettileNemicoSprite, screenMatrix, staticScreenMatrix, proiettileNemico_id);
				cancellaOggetto(old_pos_proiettili, &proiettileSprite, screenMatrix, staticScreenMatrix, proiettileRana_id);
				// aggiorna contatori proiettili
				contatore_proiettili_nemici_in_gioco--;
				contatore_proiettili_in_gioco--;
				
				gameHud.gameInfo.punteggio++;
				
				beep();
			}
		}
		
		
		if(fiumeCollision) //beep();
		
		if(tanaChiusaCollision) 
		{ 
			morteRana=true;
			beep(); 
		}
		if(tanaApertaCollision){
			taneChiuse++;
			vittoriaRana=true;
			beep(); 
			for(int i=0; i<5; i++){
				if(tane[i].status == CLOSED){
					stampaSpriteInMatrice( tane[i].info.y, tane[i].info.x, &taneSprite[CLOSED], staticScreenMatrix);
					stampaSpriteInMatrice( tane[i].info.y, tane[i].info.x, &taneSprite[CLOSED], screenMatrix);
				}
			}
	 	}
	 	
	 	if(autoProiettiliNemiciCollision)		// c'è collisione tra proiettileNemico e veicolo
	 	{ 
	 		// individua il proiettileNemico che ha colliso
	 		proiettileNemico_id = collisioneAutoProiettile( old_pos, old_pos_proiettili_nemici, spriteOggetto, PROIETTILE_NEMICO_SPRITE);
	 		
	 		// il proiettileNemico va distrutto
	 		uccidiProiettileNemico( array_pid_proiettili_nemici, proiettileNemico_id); // uccide il processo proiettile
	 		
	 		// cancella oggetto dalle matrici
	 		cancellaOggetto(old_pos_proiettili_nemici, &proiettileNemicoSprite, screenMatrix, staticScreenMatrix, proiettileNemico_id);
	 		
	 		// aggiorna contatore
	 		contatore_proiettili_nemici_in_gioco--; 
	 		
	 		//beep();
 		}
 		
	 	if(autoProiettileCollision) 	// c'è collisione tra proiettileRana e veicolo
	 	{
	 		// individia il proiettile
	 		proiettileRana_id = collisioneAutoProiettile( old_pos, old_pos_proiettili, spriteOggetto, PROIETTILE_SPRITE);
	 		
	 		// cancella oggetto dalle matrici
	 		cancellaOggetto(old_pos_proiettili, &proiettileSprite, screenMatrix, staticScreenMatrix, proiettileRana_id);
			
			// il proiettileRana va distrutto
	 		uccidiProiettile( array_pid_proiettili, proiettileRana_id); // uccide il processo proiettile
	 		
	 		// aggiorna contatore
	 		contatore_proiettili_in_gioco--;
			//beep(); 
		}
	 	
		if( autoCollision )
		{ 
			morteRana = true;
		}
		
		if(enemyBulletCollision != -1){
			//beep();
			uccidiProiettileNemico( array_pid_proiettili_nemici, enemyBulletCollision); // uccide il processo proiettile
			cancellaOggetto(old_pos_proiettili_nemici, &proiettileNemicoSprite, screenMatrix, staticScreenMatrix, enemyBulletCollision);
			contatore_proiettili_nemici_in_gioco--;
			morteRana = true;
		}	
    	if (gameHud.gameInfo.tempo == 0) morteRana = true;
		if(morteRana)
		{
			cancellaOggetto(old_pos, &ranaSprite, screenMatrix, staticScreenMatrix, 0);
			pidRana = resetRana(pipe_fd, pipeRana_fd, pidRana); 
			gameHud.gameInfo.vite--;
			timerReset(&current_time, &start_time);
			morteRana = false;
		}
		if(vittoriaRana)
		{
			cancellaOggetto(old_pos, &ranaSprite, screenMatrix, staticScreenMatrix, 0);
			pidRana = resetRana(pipe_fd, pipeRana_fd, pidRana); 
			gameHud.gameInfo.punteggio+=10;
			timerReset(&current_time, &start_time);
			vittoriaRana = false;
		}
		/**/
		gameHud.gameInfo.tempo = timer(&current_time, &start_time, MAX_TIME);
		
		
		//aggiornaGameInfo(&gameHud, tempo, vite, punteggio, livello);
		aggiornaHudInMatrice(&gameHud, screenMatrix);
    
		stampaMatrice(screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
    	refresh(); // Aggiorna la finestra
		
		// condizioni di vittoria o sconfitta
    	if(gameHud.gameInfo.vite == 0)	gameOver=true;

		if(taneChiuse == 5) gameWin=true;


	}//end while
	
	// a fine gioco chiude tutti i processi
	uccidiProcesso(&pidRana, 0);
	
	for(int i=0; i< 3; i++)
	{
		uccidiProcesso(pid_tronchi, i);
	}
	for(int i=0; i< 8; i++)
	{
		uccidiProcesso(pid_veicoli, i);
	}
	for(int i=0; i< contatore_proiettili_nemici_in_gioco; i++)
	{
		uccidiProcesso(array_pid_proiettili_nemici, i);
	}
	for(int i=0; i< contatore_proiettili_in_gioco; i++)
	{
		uccidiProcesso(array_pid_proiettili, i);
	}
	for(int i=0; i< contatore_nemici_in_gioco; i++)
	{
		uccidiProcesso(array_pid_nemici, i);
	}
	/**/
  return;  
}//end drawProcess
//--------------------------------------------FINE PROCESSO DISEGNA----------------------------------

void timerReset(time_t *current_time, time_t *start_time)
{
	*start_time = time(current_time);
}

int timer(time_t *current_time, time_t *start_time, int max_time)	//ritorna i secondi di countdown
{
	int seconds=max_time;
	time(current_time);
    double diff_time = difftime(*current_time, *start_time);
    int diff_time_sec = (int) diff_time;
    if(diff_time_sec > 0) seconds=(max_time - diff_time_sec)%max_time;
    if(diff_time_sec == max_time) *start_time = *current_time;	//resetta il tempo di start
    return seconds;
}


//-----------------------------------------------------------
int id_disponibile(pid_t *array_pid, int lunghezza){
	for(int i=0;i<lunghezza;i++){
		if(array_pid[i]==0)return i;
	}
	return -1;
}
//----------------------------------

void aggiornaDirezioneTronchi(PipeData *pipeData, PipeData *old_pos, int *arr_dir_tronchi ){
		int tronco_id = pipeData->id;													//individua che tronco hai letto
		arr_dir_tronchi[tronco_id] = pipeData->x - old_pos[pipeData->id].x; //controlla la direzione in base alla posizione precedente
}


//--------------------------------------------Stampa Puntuale----------------------------------------------------------------------
void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]){
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
		// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE
		// il flag sara TRUE solo se il carattere è stato modificato
			if(screenMatrix[i][j].is_changed){							
				attron(COLOR_PAIR(screenMatrix[i][j].color));
				mvaddch(i, j, screenMatrix[i][j].ch);
				attroff(COLOR_PAIR(screenMatrix[i][j].color));
				screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
			}
		}
	}
	return;
}
void stampaRefreshMatrice( ScreenCell (*screenMatrix)[WIDTH]){
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE
			// il flag sara TRUE solo se il carattere è stato modificato
							
			attron(COLOR_PAIR(screenMatrix[i][j].color));
			mvaddch(i, j, screenMatrix[i][j].ch);
			attroff(COLOR_PAIR(screenMatrix[i][j].color));
			screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
			
		}
	}
	return;
}
// per aggiornamento della posizione al passo precedente degli oggetti dinamici
void aggiornaOldPos(PipeData *old_pos,PipeData *pipeData){
    old_pos->x=pipeData->x;
    old_pos->y=pipeData->y;
    old_pos->type=pipeData->type;
    return;
}
//--------------------------------------------AGGIORNAMENTO OGGETTI IN MATRICE--------------------------------
void aggiornaPosizioneOggetto(PipeData *pipeData, PipeData *old_pos, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH], Sprite *sprite) {
    // se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
    if (pipeData->x != old_pos->x || pipeData->y != old_pos->y) {
        pulisciSpriteInMatrice(old_pos->y, old_pos->x, sprite, screenMatrix, staticScreenMatrix);
        stampaSpriteInMatrice(pipeData->y, pipeData->x, sprite, screenMatrix);
        aggiornaOldPos(old_pos, pipeData);
    }
}
//----------------------------------------------------------------
void stampaSpriteInMatrice(int startRow, int startCol, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH]) {
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;

    for (int i = 0; i < maxRows; i++) {
        for (int j = 0; j < maxCols; j++) {
            int row = startRow + i;
            int col = startCol + j;

            screenMatrix[row][col].ch = sprite->sprite[i][j];
            screenMatrix[row][col].color = sprite->color;
            screenMatrix[row][col].is_changed = true;
        }
    }
}
//----------------------------------------------------------------
void pulisciSpriteInMatrice(int row, int col, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]) {
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;
    
    if (row != -1) {
        for (int i = row; i < row + maxRows; i++) {
            for (int j = col; j < col + maxCols; j++) {
                screenMatrix[i][j].ch = staticScreenMatrix[i][j].ch;
                screenMatrix[i][j].color = staticScreenMatrix[i][j].color;
                screenMatrix[i][j].is_changed = true;
            }
        }
    }
}

//----------------------------------------------------------------------
// cancella la sprite dell'oggetto dalle matrici e lo 'disattiva' (type = ' ')
void cancellaOggetto(PipeData *array_oggetti, Sprite* sprite_oggetto, 
											ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH], int id_oggetto)
{
	if(id_oggetto >= 0) // se l'id è un indice di array valido
	{
		pulisciSpriteInMatrice(array_oggetti[id_oggetto].y, array_oggetti[id_oggetto].x, sprite_oggetto, screenMatrix, staticScreenMatrix);
		array_oggetti[id_oggetto].type = ' ';
		array_oggetti[id_oggetto].x = 0;
		array_oggetti[id_oggetto].y = 0;
	}
}
//--------------------------------------------aggiornamento HUD ---------------------

void aggiornaGameInfo(GameHUD *gameHud, int tempo, int vite, int punteggio, int livello) //usata in disegna
{
	gameHud->gameInfo.tempo = tempo;
	gameHud->gameInfo.vite = vite;
	gameHud->gameInfo.punteggio = punteggio;
	gameHud->gameInfo.livello = livello;
}


// aggiorna le stringhe con le info di gioco
void aggiornaGameHud(GameHUD *gameHud)
{
		// aggiorna la visualizzazione grafica delle vite, per ogni vita rimasta stampa un *
		int vite_restanti = gameHud->gameInfo.vite;
		char string_lifes[4] = "    ";
		for(int i=0; i<vite_restanti; i++)
		{
			string_lifes[i]='*';
		}
		//aggiorna le stringhe con le info
		sprintf(gameHud->score_hud, "Livello: %2d \t SCORE: %3d" , gameHud->gameInfo.livello, gameHud->gameInfo.punteggio);
		sprintf(gameHud->life_hud, "tempo: %3d \t vite: %s \t :%d", gameHud->gameInfo.tempo, string_lifes, vite_restanti);
		
		//calcola lunghezza delle nuove stringhe
		int score_hud_len = strlen(gameHud->score_hud);
		int life_hud_len = strlen(gameHud->life_hud);
		int startX	=		gameHud->start_x_hud;
		
		// aggiorna pos dell'ultimo carattere delle stringhe
		gameHud->end_x_hud = startX + score_hud_len +1;
		gameHud->end_x_life_hud = startX + life_hud_len +1; 
}

//----------------------------------------------
void aggiornaHudInMatrice(GameHUD *gameHud, ScreenCell (*screenMatrix)[WIDTH])
{
	aggiornaGameHud(gameHud);
	
	int score_hud_len = strlen(gameHud->score_hud);	// aggiorna dimensione delle stringhe
	int life_hud_len = strlen(gameHud->life_hud);
	
	int startX	=		gameHud->start_x_hud;					// prende coordinate della HUD
	int endX 		=		gameHud->end_x_hud;
	int endXLifeHud = gameHud->end_x_life_hud;
	
	int score_hud_index = 0;	// indici per le stringhe
	int life_hud_index = 0;
	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<WIDTH;j++)
		{
			screenMatrix[i][j].color = SFONDO;
			switch(i)
			{
				case 1:																// riga y=1
					if ((j>startX && j< endX) )
					{
						screenMatrix[i][j].ch = gameHud->score_hud[score_hud_index];  //stampa i caratteri della scritta
						score_hud_index = (score_hud_index+1)%score_hud_len; // aggiorna indice per la stringa
					}else{
						screenMatrix[i][j].ch = ' ';
					}
					break;
					
				case 2:																// riga y=2
					if((j>startX && j< endXLifeHud))
					{
						screenMatrix[i][j].ch = gameHud->life_hud[life_hud_index];
						life_hud_index = (life_hud_index +1)%life_hud_len;
				 	}else{
						screenMatrix[i][j].ch = ' ';
					}
					break;
				default:
					screenMatrix[i][j].ch = ' ';
					break;
			}
			screenMatrix[i][j].is_changed = true;
		}
	}
	return;
}






