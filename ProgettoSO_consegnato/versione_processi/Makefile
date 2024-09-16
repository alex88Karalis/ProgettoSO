# Nome dell'eseguibile
EXECUTABLE := frog

# Directory dell'eseguibile, degli oggetti, dei sorgenti e degli header
BINDIR := bin
OBJDIR := obj
SRCDIR := src
HDRDIR := hdr

# Nome completo dell'eseguibile con percorso
FULL_EXECUTABLE := $(BINDIR)/$(EXECUTABLE)

# Ricerca automatica dei file sorgente .c in SRCDIR e generazione degli oggetti in OBJDIR
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Compilatore C
CC := gcc

# recupera i flag di compilazione e di linking di SDL
SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LIBS := $(shell sdl2-config --libs)
SDL_MIXER_LIBS := -lSDL2_mixer

# Opzioni di compilazione
CFLAGS := -g -Wall -Wextra $(SDL_CFLAGS)

# Librerie necessarie
LDLIBS := -lncurses $(SDL_LIBS) $(SDL_MIXER_LIBS)

# Regola di compilazione dell'eseguibile
$(FULL_EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LDLIBS)

# Regole di compilazione dei file oggetto
$(OBJDIR)/main.o: $(SRCDIR)/main.c $(HDRDIR)/disegna.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/disegna.o: $(SRCDIR)/disegna.c $(HDRDIR)/rana.h $(HDRDIR)/logo.h $(HDRDIR)/menuIniziale.h $(HDRDIR)/inizializza.h $(HDRDIR)/proiettile.h $(HDRDIR)/hud.h $(HDRDIR)/schermo.h $(HDRDIR)/nemico.h $(HDRDIR)/proiettileNemico.h $(HDRDIR)/tempo.h $(HDRDIR)/musica.h $(HDRDIR)/soundPlayer.h
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJDIR)/rana.o: $(SRCDIR)/rana.c $(HDRDIR)/rana.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/proiettile.o: $(SRCDIR)/proiettile.c $(HDRDIR)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/logo.o: $(SRCDIR)/logo.c $(HDRDIR)/logo.h $(HDRDIR)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJDIR)/menuIniziale.o: $(SRCDIR)/menuIniziale.c $(HDRDIR)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJDIR)/inizializza.o: $(SRCDIR)/inizializza.c $(HDRDIR)/utilities.h $(HDRDIR)/rana.h $(HDRDIR)/hud.h $(HDRDIR)/nemico.h $(HDRDIR)/tempo.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/hud.o: $(SRCDIR)/hud.c $(HDRDIR)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJDIR)/schermo.o: $(SRCDIR)/schermo.c $(HDRDIR)/utilities.h $(HDRDIR)/proiettile.h $(HDRDIR)/collisione.h $(HDRDIR)/rana.h $(HDRDIR)/nemico.h $(HDRDIR)/proiettileNemico.h $(HDRDIR)/coccodrillo.h $(HDRDIR)/tempo.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/sound.o: $(SRCDIR)/sound.c $(HDRDIR)/sound.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/collisione.o: $(SRCDIR)/collisione.c $(HDRDIR)/collisione.h $(HDRDIR)/utilities.h $(HDRDIR)/rana.h $(HDRDIR)/schermo.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/nemico.o: $(SRCDIR)/nemico.c $(HDRDIR)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJDIR)/proiettileNemico.o: $(SRCDIR)/proiettileNemico.c $(HDRDIR)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/coccodrillo.o: $(SRCDIR)/coccodrillo.c $(HDRDIR)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/tempo.o: $(SRCDIR)/tempo.c $(HDRDIR)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/musica.o: $(SRCDIR)/musica.c $(HDRDIR)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/soundPlayer.o: $(SRCDIR)/soundPlayer.c $(HDRDIR)/utilities.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/utilities.o: $(SRCDIR)/utilities.c $(HDRDIR)/utilities.h $(HDRDIR)/sound.h
	$(CC) $(CFLAGS) -c $< -o $@

# Pulizia dei file oggetto e dell'eseguibile
clean:
	rm -f $(OBJDIR)/*.o $(FULL_EXECUTABLE)

openAll:
	gedit $(SRCDIR)/*.c $(HDRDIR)/*.h Makefile&
