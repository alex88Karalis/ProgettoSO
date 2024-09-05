
# Nome dell'eseguibile
EXECUTABLE := frog

# Elenco dei file oggetto
OBJECTS := main.o utilities.o disegna.o rana.o logo.o menuIniziale.o inizializza.o proiettile.o hud.o schermo.o sound.o collisione.o nemico.o proiettileNemico.o coccodrillo.o tempo.o

# Compilatore C
CC := gcc

# recupera i flag di compilazione e di linking di SDL
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LIBS := $(shell sdl2-config --libs)
SDL_MIXER_LIBS := -lSDL2_mixer

# Opzioni di compilazione
CFLAGS := -Wall -Wextra $(SDL_CFLAGS) -g

#CFLAGS := -gdb -Wall -Wextra -g

# Librerie necessarie
LDLIBS := -lncurses -lpthread $(SDL_LIBS) $(SDL_MIXER_LIBS)


# Regola di compilazione dell'eseguibile
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LDLIBS)

build: clean $(EXECUTABLE)



# Regole di compilazione dei file oggetto
main.o: main.c disegna.h
	$(CC) $(CFLAGS) -c main.c

disegna.o: disegna.c rana.h logo.h menuIniziale.h inizializza.h proiettile.h hud.h schermo.h nemico.h proiettileNemico.h tempo.h
	$(CC) $(CFLAGS) -c disegna.c
	
rana.o: rana.c rana.h
	$(CC) $(CFLAGS) -c rana.c

proiettile.o: proiettile.c utilities.h
	$(CC) $(CFLAGS) -c proiettile.c

coccodrillo.o: coccodrillo.c utilities.h
	$(CC) $(CFLAGS) -c coccodrillo.c

tempo.o: tempo.c utilities.h
	$(CC) $(CFLAGS) -c tempo.c

logo.o: logo.c logo.h utilities.h
	$(CC) $(CFLAGS) -c logo.c
	
menuIniziale.o: menuIniziale.c utilities.h
	$(CC) $(CFLAGS) -c menuIniziale.c
	
inizializza.o: inizializza.c utilities.h rana.h hud.h nemico.h tempo.h
	$(CC) $(CFLAGS) -c inizializza.c

hud.o: hud.c utilities.h
	$(CC) $(CFLAGS) -c hud.c
	
schermo.o: schermo.c utilities.h proiettile.h collisione.h rana.h nemico.h proiettileNemico.h coccodrillo.h tempo.h
	$(CC) $(CFLAGS) -c schermo.c

sound.o: sound.c sound.h
	$(CC) $(CFLAGS) -c sound.c

collisione.o: collisione.c collisione.h utilities.h
	$(CC) $(CFLAGS) -c collisione.c

utilities.o: utilities.c utilities.h sound.h
	$(CC) $(CFLAGS) -c utilities.c


nemico.o: nemico.c utilities.h
	$(CC) $(CFLAGS) -c nemico.c

proiettileNemico.o: proiettileNemico.c utilities.h 
	$(CC) $(CFLAGS) -c proiettileNemico.c

# Pulizia dei file oggetto e dell'eseguibile
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

openAll:
	gedit *.c *.h MakeFile&
run:
	./$(EXECUTABLE)



