# Nome dell'eseguibile
EXECUTABLE := frog

# Elenco dei file oggetto
OBJECTS := main.o utilities.o disegna.o auto.o tronco.o rana.o logo.o menuIniziale.o inizializza.o

# Compilatore C
CC := gcc

# Opzioni di compilazione
CFLAGS := -Wall -Wextra

# Librerie necessarie
LDLIBS := -lncurses


# Regola di compilazione dell'eseguibile
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LDLIBS)

# Regole di compilazione dei file oggetto
main.o: main.c disegna.h
	$(CC) $(CFLAGS) -c main.c

disegna.o: disegna.c auto.h tronco.h rana.h logo.h menuIniziale.h inizializza.h
	$(CC) $(CFLAGS) -c disegna.c
	
auto.o: auto.c utilities.h
	$(CC) $(CFLAGS) -c auto.c

tronco.o: tronco.c utilities.h
	$(CC) $(CFLAGS) -c tronco.c
	
rana.o: rana.c utilities.h
	$(CC) $(CFLAGS) -c rana.c

logo.o: logo.c utilities.h
	$(CC) $(CFLAGS) -c logo.c
	
menuIniziale.o: menuIniziale.c utilities.h
	$(CC) $(CFLAGS) -c menuIniziale.c
	
inizializza.o: inizializza.c utilities.h
	$(CC) $(CFLAGS) -c inizializza.c
	
utilities.o: utilities.c utilities.h
	$(CC) $(CFLAGS) -c utilities.c

# Pulizia dei file oggetto e dell'eseguibile
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
