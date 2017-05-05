#include "lemipc.h"

static void		init_game(t_shm *shm)
{
	char	*line;
	int		i;

	shm->state = GAMESTATE_INIT;
	ft_memset(shm->m, MAP_BORDERWIDTH, TRUEMAP_WIDTH - 1);
	shm->m[TRUEMAP_WIDTH - 1] = '\n';
	ft_memset(shm->m + sizeof(shm->m) - TRUEMAP_WIDTH, MAP_BORDERWIDTH, TRUEMAP_WIDTH - 1);
	shm->m[sizeof(shm->m) - 1] = '\n';
	i = 0;
	while (i < MAP_HEIGHT)
	{
		line = shm->m + ((i + 1) * TRUEMAP_WIDTH);
		line[0] = MAP_BORDERHEIGHT;
		line[TRUEMAP_WIDTH - 2] = MAP_BORDERHEIGHT;
		line[TRUEMAP_WIDTH - 1] = '\n';
		ft_memset(line + 1, MAP_EMPTYCASE, MAP_WIDTH);
		i += 1;
	}
}

static void		shm_link(char **map, t_shm *shm)
{
	int		i;

	i = 0;
	while (i < MAP_HEIGHT)
	{
		map[i] = shm->m + (i + 1) * TRUEMAP_WIDTH + 1;
		i += 1;
	}
}

void			init(t_context *context, char *algo)
{
	context->algo = algo ? ft_atoi(algo) : 1;
	shm_get(&context->shmid, &context->creator);
	shm_attach(&context->shm, context->shmid);
	sem_get(&context->semid, context->creator);
	sem_wait(context->semid);
	shm_link(context->map, context->shm);
	if (context->creator)
	{
		init_game(context->shm);
	}
	sem_post(context->semid);
	mq_get(&context->mqid);
}
