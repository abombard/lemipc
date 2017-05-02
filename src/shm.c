#include "lemipc.h"

void	shm_init(int *shmfd, int *prime)
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
		*prime = 0;
	}
	else
	{
		if (ftruncate(fd, sizeof(t_shm)))
		{
			perror("ftruncate");
			exit(EXIT_FAILURE);
		}
		*prime = 1;
	}
	*shmfd = fd;
}

void	shm_erase(void)
{
	if (shm_unlink(IPC_OBJPATH))
	{
		perror("shm_unlink");
		exit(EXIT_FAILURE);
	}
}

void	shm_alloc(t_shm **shm, int shmfd)
{
	*shm = (t_shm *)mmap(NULL, sizeof(t_shm),
		PROT_READ | PROT_WRITE, MAP_SHARED,
		shmfd, 0);
	if (*shm == MAP_FAILED)
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}
}

void	shm_free(t_shm *shm)
{
	if (munmap(shm, sizeof(t_shm)))
	{
		perror("munmap");
		exit(EXIT_FAILURE);
	}
}

void	shm_link(char *map[MAP_HEIGHT], t_shm *shm)
{
	int		i;

	i = 0;
	while (i < MAP_HEIGHT)
	{
		map[i] = shm->m + (i + 1) * TRUEMAP_WIDTH + 1;
		i += 1;
	}
}
