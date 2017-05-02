#include "lemipc.h"

void	game_alloc(t_game **game, int shmfd)
{
	*game = (t_game *)mmap(NULL, sizeof(t_game),
		PROT_READ | PROT_WRITE, MAP_SHARED,
		shmfd, 0);
	if (*game == MAP_FAILED)
	{
		perror("mmap");
		exit(EXIT_FAILURE);
	}
}

void	game_free(t_game *game)
{
	if (munmap(game, sizeof(t_game)))
	{
		perror("munmap");
		exit(EXIT_FAILURE);
	}
}

