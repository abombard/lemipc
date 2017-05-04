#include "lemipc.h"
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
	int					val;
	struct semid_ds		*buf;
	unsigned short int	*array;
	struct seminfo		*__buf;
};

static void	sem_init(int semid)
{
	union semun			arg;
	unsigned short int	value;

	value = 1;
	arg.array = &value;
	if (semctl(semid, 0, SETALL, arg) == -1)
	{
		perror("semctl SETALL");
		exit(EXIT_FAILURE);
	}
}

void		sem_get(int *semid, int creator)
{
	int		semflg;

	semflg = creator ? IPC_CREAT | IPC_EXCL : 0;
	*semid = semget(IPCKEY, 1, semflg | 0644);
	if (*semid == -1)
	{
		perror("semget");
		exit(EXIT_FAILURE);
	}
	if (creator)
		sem_init(*semid);
}

void	sem_destroy(int semid)
{
	if (semctl (semid, 1, IPC_RMID, 0) == -1)
	{
		perror("semctl IPC_RMID");
		exit(EXIT_FAILURE);
	}
}

void	sem_wait(int semid)
{
	struct sembuf	op;

	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;
	if (semop(semid, &op, 1))
	{
		perror("semop");
		exit(EXIT_FAILURE);
	}
}

void	sem_post(int semid)
{
	struct sembuf	op;

	op.sem_num = 0;
	op.sem_op = 1;
	op.sem_flg = 0;
	if (semop(semid, &op, 1))
	{
		perror("semop");
		exit(EXIT_FAILURE);
	}
}
