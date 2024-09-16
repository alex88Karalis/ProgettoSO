#include "../hdr/disegna.h"

int main()
{
    // Pipe file descriptors
    int pipe_fd[2];
    // Crea la pipe
    creaPipe(pipe_fd);
    // inizializzazione della libreria ncurses
    inizializzaNcurses();
    // visualizza il logo ed aspetta che l'utente prema un tasto qualsiasi
    avviaLogo();
    // processo per il menu iniziale
    int scelta = avviaMenuIniziale();

    if (scelta == 0)
    {
        // selezionato start
        // avvia il processo che gestisce la stampa a schermo
        avviaDrawProcess(pipe_fd);
    }
    else
    {
        // Chiudi le estremità della pipe
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        // Aspetta che il processo figlio termini
        wait(NULL);
        // Termina ncurses
        endwin();
        printf("FROGGER CLOSED GOODBYE\n");
        return 0;
    }
    // Aspetta che il processo figlio termini
    wait(NULL);
    // Termina ncurses
    endwin();
    printf("FROGGER CLOSED GOODBYE\n");
    return 0;
}