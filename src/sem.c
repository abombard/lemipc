#include "lemipc.h"

void	sem_attach(sem_t **sem_id, int owner)
{
	sem_t	*id;
	int		flg;

	flg = owner ? O_CREAT | O_EXCL : 0;
	id = sem_open(IPC_SEMNAME, flg, 0666, 1);
	if (id == SEM_FAILED)
	{
		perror("sem_open");
		exit(EXIT_FAILURE);
	}
	*sem_id = id;
}

void	sem_detach(sem_t *sem_id)
{
	if (sem_close(sem_id))
	{
		perror("sem_close");
		exit(EXIT_FAILURE);
	}
}

void	sem_erase(sem_t *sem_id)
{
	if (sem_unlink(IPC_SEMNAME))
	{
		perror("sem_unlink");
		exit(EXIT_FAILURE);
	}
}
