#include "lemipc.h"
#include <sys/ipc.h>
#include <sys/shm.h>

void	shm_get(int *shmid, int *created)
{
	int		shmfd;

	shmfd = shmget(IPCKEY, sizeof(t_shm), IPC_CREAT | IPC_EXCL | 0644);
	if (shmfd != -1)
	{
		*shmid = shmfd;
		*created = 1;
		return ;
	}
	if (errno != EEXIST)
	{
		perror("shmget");
		shm_destroy(*shmid);
		exit(EXIT_FAILURE);
	}
	shmfd = shmget(IPCKEY, sizeof(t_shm), 0);
	if (shmfd == -1)
	{
		perror("shmget");
		shm_destroy(*shmid);
		exit(EXIT_FAILURE);
	}
	*shmid = shmfd;
	*created = 0;
}

void	shm_destroy(int shmid)
{
	if (shmctl (shmid, IPC_RMID, 0) == -1)
	{
		perror("shmctl IPC_RMID");
		exit(EXIT_FAILURE);
	}
}

void	shm_attach(t_shm **shmaddr, int shmid)
{
	*shmaddr = shmat(shmid, NULL, 0);
	if (*shmaddr == (void *)-1)
	{
		perror("shmat");
		exit(EXIT_FAILURE);
	}
}

void	shm_detach(void *shmaddr)
{
	if (shmdt(shmaddr))
	{
		perror("shmdt");
		exit(EXIT_FAILURE);
	}
}
