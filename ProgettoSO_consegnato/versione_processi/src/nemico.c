#include "../hdr/nemico.h"
pid_t avviaNemico(int *pipe_fd, int id)
{
	pid_t nemico_pid = fork();
	if (nemico_pid < 0)
	{
		perror("Fork failed");
		exit(1);
	}
	else
	{
		if (nemico_pid == 0)
		{
			close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
			nemico(pipe_fd, id);
			exit(0);
		}
		else
		{
			return nemico_pid;
		}
	}
	return nemico_pid;
}

void nemico(int *pipe_fd, int id)
{ // l'id fa riferimento all'id del tronco su cui il processo nemico sta
	PipeData nemico;
	nemico.id = id;
	int initialX = 0;
	nemico.y = ARGINEROWSTART + 1;

	// Seme unico per ogni processo
	unsigned int seed = time(NULL) ^ (getpid() << 16);
	// switch su id in modo da asseganre coordinate iniziali
	switch (id)
	{
	case 0:
		initialX = PLANT0STARTCOL;
		break;
	case 1:
		initialX = PLANT1STARTCOL;
		break;
	case 2:
		initialX = PLANT2STARTCOL;
		break;
	case 3:
		initialX = PLANT3STARTCOL;
		break;

	default:
		break;
	}
	nemico.x = initialX;
	nemico.type = 'n';
	// numero randomico tra min e max compresi
	int randomico = generaRandom_r(600000, 600000 * 25, &seed);
	// piccola usleep
	usleep(randomico);
	write(pipe_fd[1], &nemico, sizeof(PipeData)); // prima scrittura

	// cambio tipo d'ora in poi lancerà soltanto proiettili
	nemico.type = 's';
	nemico.x++;
	nemico.y++;
	int contatore = 0;
	while (1)
	{
		// parte relativa al lancio dei proiettili
		if (contatore % 100 == 0)
		{
			write(pipe_fd[1], &nemico, sizeof(PipeData));
		}
		contatore = (contatore + 1) % 50;
		usleep(200000); // Aspetta un po' prima di generare nuove coordinate
	}
}

void uccidiNemico(pid_t *array_pid_nemici, int id_nemico)
{
	if ((id_nemico != NOID) && (array_pid_nemici[id_nemico] != NOPID))
	{
		kill(array_pid_nemici[id_nemico], SIGKILL);
		int err = waitpid(array_pid_nemici[id_nemico], NULL, 0);
		if (err == -1)
		{
			perror("Errore nella waitpid");
			exit(1);
		}
		array_pid_nemici[id_nemico] = NOPID;
	}
}