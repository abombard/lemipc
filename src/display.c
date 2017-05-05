#include "lemipc.h"

static void		display(t_shm *shm)
{
	int		i;

	write(1, shm->m, sizeof(shm->m));
	i = 0;
	while (i <= TRUEMAP_HEIGHT)
	{
		printf("\033[F");
		i += 1;
	}
	printf("\n");
}

void			loop_display(t_context *context)
{
	int		die;
	char	winner;

	die = 0;
	while (!die)
	{
		sem_wait(context->semid);
		if (context->shm->state != GAMESTATE_INIT &&
			ateamwin(context->map, &winner))
			die = 1;
		display(context->shm);
		sem_post(context->semid);
		usleep(200000);
	}
	fprintf(stderr, "Winner %c\n", winner);
}
