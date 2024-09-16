#include "../hdr/coccodrillo.h"

pid_t avviaCoccodrillo(int *pipe_fd, PipeData* coccodrillo_init,int direction,int vel)
{
    pid_t coccodrillo_pid = fork();

    if (coccodrillo_pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }
    else if (coccodrillo_pid == 0)
    {
        // Chiudi l'estremità di lettura della pipe
        close(pipe_fd[0]);
        // Processo coccodrillo
        coccodrillo(pipe_fd, coccodrillo_init,direction,vel);
        exit(0);
    }
    else
    {
        return coccodrillo_pid;
    }
}

void coccodrillo(int *pipe_fd, PipeData *coccodrillo_init,int direction,int vel)
{
    // set informazioni iniziali del coccodrillo
    PipeData coccodrillo; 
    coccodrillo.x = coccodrillo_init->x;
    coccodrillo.y = coccodrillo_init->y;
    coccodrillo.type = coccodrillo_init->type;
    coccodrillo.id = coccodrillo_init->id;
    int dirX = direction;
    int velocity;
    switch(vel){
        case 0:
        velocity = FLUSSO_LENTO;
        break;
        case 1:
        velocity =FLUSSO_NORM;
        break;
        case 2:
        velocity= FLUSSO_VELOCE;
        break;
    }
    // Seme unico per ogni processo
    unsigned int seed = time(NULL) ^ (getpid() << 16); 
    // numero randomico tra min e max compresi
    int randomico = generaRandom_r(velocity , velocity*15,&seed);
    // piccola usleep randomica
    usleep(randomico);
    
    while (1)
    {
        coccodrillo.x += dirX;
        write(pipe_fd[1], &coccodrillo, sizeof(PipeData));
        usleep(velocity); // Aspetta un po' prima di generare nuove coordinate
    }
}

void uccidiCoccodrillo(pid_t *pids_coccodrilli, int id_coccodrillo)
{
    if ((id_coccodrillo != NOID) && (pids_coccodrilli[id_coccodrillo] != NOPID))
    {
        kill(pids_coccodrilli[id_coccodrillo], SIGKILL);
        int err = waitpid(pids_coccodrilli[id_coccodrillo], NULL, 0);
        if (err == -1)
        {
            perror("Errore nella waitpid");
            exit(1);
        }
        pids_coccodrilli[id_coccodrillo] = NOPID;
    }
    return;
}