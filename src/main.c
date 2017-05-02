#include "lemipc.h"

void	init_shm(t_shm *shm)
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
	int		prime;
	int		shmfd;
	sem_t	*sem_id;
	t_shm	*shm;

	shm_init(&shmfd, &prime);
	shm_alloc(&shm, shmfd);
	shm_link(context->map, shm);
	sem_attach(&sem_id, prime);
	sem_wait(sem_id);
	if (prime)
	{
		init_shm(shm);
	}
	sem_post(sem_id);
	context->prime = prime;
	context->shmfd = shmfd;
	context->shm = shm;
	context->sem_id = sem_id;
}

void	end(t_context *context)
{
	shm_free(context->shm);
	sem_detach(context->sem_id);
	if (context->prime)
	{
		sem_erase(context->sem_id);
		shm_erase(context->shmfd);
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

int		isempty(char c)
{
	return (c == MAP_EMPTYCASE);
}

int		isally(char c, t_player *player)
{
	return (c == player->id);
}

int		isenemy(char c, t_player *player)
{
	return (!isempty(c) && !isally(c, player));
}

int		isoutofrange(unsigned int x, unsigned int y)
{
	return (x >= MAP_WIDTH || y >= MAP_HEIGHT);
}

void	ia(t_player *player, char **map)
{
	int				timeout;
	unsigned int	x;
	unsigned int	y;

	timeout = 10;
	while (timeout)
	{
		if (isoutofrange(player->pos.x - 1, player->pos.y))
			x = player->pos.x + 1;
		else if (isoutofrange(player->pos.x + 1, player->pos.y))
			x = player->pos.x - 1;
		else
			x = player->pos.x + rand() % 3 - 1;
		if (isoutofrange(player->pos.x, player->pos.y - 1))
			y = player->pos.y + 1;
		else if (isoutofrange(player->pos.x, player->pos.y + 1))
			y = player->pos.y - 1;
		else
			y = player->pos.y + (x == player->pos.x ? rand() % 3 - 1 : 0);
		if (!isoutofrange(x, y))
		{
			if (isempty(map[y][x]))
			{
				map[player->pos.y][player->pos.x] = MAP_EMPTYCASE;
				map[y][x] = player->id;
				player->pos.x = x;
				player->pos.y = y;
				break ;
			}
		}
		timeout -= 1;
	}
}

int		isdead(t_player *player, char **map)
{
	static t_pos	delta[] = {
		{ -1, -1 }, { -1,  0 }, { -1,  1 },
		{  0, -1 }, {  0,  0 }, {  0,  1 },
		{  1, -1 }, {  1,  0 }, {  1,  1 }
	};
	t_pos			p;
	int				enemycount;
	int				i;

	enemycount = 0;
	i = 0;
	while (i < sizeof(delta) / sizeof(delta[0]))
	{
		p.x = player->pos.x + delta[i].x;
		p.y = player->pos.y + delta[i].y;
		if (!isoutofrange(p.x, p.y))
		{
			if (isenemy(map[p.y][p.x], player))
				enemycount += 1;
		}
		i += 1;
	}
	return (enemycount >= 2);
}

int		loop(t_gamestate gamestate, char **map, t_player *player)
{
	//if (gamestate == GAMESTATE_INIT)
	//	return (0);
	if (gamestate == GAMESTATE_OVER)
		return (1);
	if (isdead(player, map))
		return (1);
	ia(player, map);

	static int dieTimeout = 3;
	if (dieTimeout -- == 0) return 1;

	return (0);
}

int		main(int argc, char **argv)
{
	t_context	context;
	t_player	player;
	int			die;

	if (argc != 2)
	{
		printf("Usage: %s <Team Id>\n", argv[0]);
		return (2);
	}

	srand(time(NULL));
	die = 0;
	init(&context);
	sem_wait(context.sem_id);
	player_init(&player, context.map, argv[1][0]);
	sem_post(context.sem_id);
	while (!die /* && (!context.prime || context.gamestate == GAMESTATE_OVER) */)
	{
		sem_wait(context.sem_id);
		die = loop(context.shm->state, context.map, &player);
		if (context.prime)
		{
			display(context.shm);
		}
		sem_post(context.sem_id);
		usleep(200000);
	}
	player_erase(&player, context.map);
	end(&context);
	return (0);
}
