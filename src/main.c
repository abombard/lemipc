#include "lemipc.h"

void	init_game(t_shm *shm)
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

void	init(t_context *context)
{
	int		created;

	shm_init(&context->shmfd, &created);
	context->prime = created;
	sem_attach(&context->sem_id, created);
	sem_wait(context->sem_id);
	shm_alloc(&context->shm, context->shmfd);
	shm_link(context->map, context->shm);
	if (created)
	{
		init_game(context->shm);
	}
	sem_post(context->sem_id);
}

void	end(t_context *context)
{
	shm_free(context->shm);
	sem_detach(context->sem_id);
	if (context->prime)
	{
		sem_erase();
		shm_erase();
	}
}

void	display(t_shm *shm)
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

int		isdead(t_player *player, char **map)
{
	static int		delta[][2] = {
		{ -1, -1 }, { -1,  0 }, { -1,  1 },
		{  0, -1 }, {  0,  0 }, {  0,  1 },
		{  1, -1 }, {  1,  0 }, {  1,  1 }
	};
	t_pos			p;
	int				enemycount;
	unsigned int	i;

	enemycount = 0;
	i = 0;
	while (i < sizeof(delta) / sizeof(delta[0]))
	{
		p.x = (unsigned int)((int)player->pos.x + delta[i][0]);
		p.y = (unsigned int)((int)player->pos.y + delta[i][1]);
		if (!isoutofrange(p.x, p.y))
		{
			if (isenemy(player, map[p.y][p.x]))
				enemycount += 1;
		}
		i += 1;
	}
	return (enemycount >= 2);
}

int		last_team_standing(char **map, char *winner)
{
	char			team;
	unsigned int	x;
	unsigned int	y;

	team = MAP_EMPTYCASE;
	y = 0;
	while (y < MAP_HEIGHT)
	{
		x = 0;
		while (x < MAP_WIDTH)
		{
			if (map[y][x] != MAP_EMPTYCASE)
			{
				if (team == MAP_EMPTYCASE)
					team = map[y][x];
				else if (team != map[y][x])
					return (0);
			}
			x += 1;
		}
		y += 1;
	}
	*winner = team;
	return (1);
}

void	loop(t_context *context)
{
	int		die = 0;
	char	winner;

	while (!die)
	{
		sem_wait(context->sem_id);
		if (context->shm->state != GAMESTATE_INIT &&
			last_team_standing(context->map, &winner))
			die = 1;
		if (context->shm->state == GAMESTATE_OVER ||
			isdead(&context->player, context->map))
			die = 1;
		else if (context->shm->state == GAMESTATE_ON)
			ia(context);
		sem_post(context->sem_id);
		sleep(5);
		//usleep(200000);
	}
}

void	loop_display(t_context *context)
{
	int		die = 0;
	char	winner;

	while (!die)
	{
		sem_wait(context->sem_id);
		if (context->shm->state != GAMESTATE_INIT &&
			last_team_standing(context->map, &winner))
			die = 1;
		display(context->shm);
		sem_post(context->sem_id);
		usleep(200000);

		//static int timeout = 10;
		//if (timeout -- == 0) break ;
	}
	printf("Winner %c\n", winner);
}

int		main(int argc, char **argv)
{
	t_context	context;

	if (argc != 2)
	{
		printf("Usage: %s <Team Id>\n", argv[0]);
		return (2);
	}
	srand((unsigned int)time(NULL));
	init(&context);
	if (!ft_strcmp(argv[1], "start"))
	{
		sem_wait(context.sem_id);
		context.shm->state = GAMESTATE_ON;
		sem_post(context.sem_id);
	}
	else if (!ft_strcmp(argv[1], "stop"))
	{
		sem_wait(context.sem_id);
		context.shm->state = GAMESTATE_OVER;
		sem_post(context.sem_id);
	}
	else if (!ft_strcmp(argv[1], "display"))
	{
		loop_display(&context);
	}
	else
	{
		sem_wait(context.sem_id);
		player_init(&context.player, context.map, argv[1][0]);
		sem_post(context.sem_id);
		loop(&context);
		sem_wait(context.sem_id);
		player_erase(&context.player, context.map);
		sem_post(context.sem_id);
	}
	end(&context);
	fprintf(stderr, "%s died\n", argv[1]);
	return (0);
}
