#include "lemipc.h"

void	shm_init(int *shmfd, int *owner)
{
	int		fd;

	fd = shm_open(IPC_OBJPATH, O_CREAT | O_EXCL | O_RDWR, 0666);
	if (fd < 0)
	{
		if (errno == EEXIST)
			fd = shm_open(IPC_OBJPATH, O_RDWR, 0666);
		if (fd < 0)
		{
			perror("shm_open");
			exit(EXIT_FAILURE);
		}
		*owner = 0;
	}
	else
	{
		if (ftruncate(fd, sizeof(t_game)))
		{
			perror("ftruncate");
			exit(EXIT_FAILURE);
		}
		*owner = 1;
	}
	*shmfd = fd;
}

void	shm_erase(int shmfd)
{
	if (shm_unlink(IPC_OBJPATH))
	{
		perror("shm_unlink");
		exit(EXIT_FAILURE);
	}
}
