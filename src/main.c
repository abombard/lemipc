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

void	init(t_context *context, char team)
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
	player_init(&context->player, context->map, team);
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

int		isempty(char c)
{
	return (c == MAP_EMPTYCASE);
}

int		isally(t_player *player, char c)
{
	return (c == player->id);
}

int		isenemy(t_player *player, char c)
{
	return (!isempty(c) && !isally(player, c));
}

int		isoutofrange(unsigned int x, unsigned int y)
{
	return (x >= MAP_WIDTH || y >= MAP_HEIGHT);
}

typedef struct	s_lp
{
	unsigned int	x;
	unsigned int	y;
	unsigned int	d;
	char			team;
}				t_lp;

int		lpcmp(void const *a, void const *b)
{
	t_lp	*p1;
	t_lp	*p2;

	p1 = (t_lp *)a;
	p2 = (t_lp *)b;
	return (p1->d < p2->d);
}

#include <math.h>
t_lp	*map_find_all(char **map, t_player *player, int (*test)(t_player *, char), size_t *count)
{
	t_lp			*ps;
	size_t			pcount;
	t_lp			p;

	ps = NULL;
	pcount = 0;
	p.y = 0;
	while (p.y < MAP_HEIGHT)
	{
		p.x = 0;
		while (p.x < MAP_WIDTH)
		{
			if (p.x != player->pos.x && p.y != player->pos.y &&
				test(player, map[p.y][p.x]))
			{
				ps = realloc(ps, (pcount + 1) * sizeof(t_lp));
				if (!ps)
				{
					perror("realloc");
					exit(EXIT_FAILURE);
				}
				p.team = map[p.y][p.x];
				p.d = (unsigned int)sqrt(
					pow(p.x - player->pos.x, 2) +
					pow(p.y - player->pos.y, 2));
				ps[pcount] = p;
				pcount += 1;
			}
			p.x += 1;
		}
		p.y += 1;
	}
	qsort(ps, pcount, sizeof(t_lp), &lpcmp);
	*count = pcount;
	return (ps);
}

void	ia(t_context *context)
{
	t_lp	*ally;
	size_t	acount;
	t_lp	*enemy;
	size_t	ecount;

	char			msg[1024];
	ssize_t			size;
	unsigned int	prio;
	struct timespec	timeout;

	timeout.tv_sec = 1;
	timeout.tv_nsec = 0;
	size = mq_timedreceive(context->player.mq, msg, sizeof(msg), &prio, &timeout);
	if (size == -1)
	{
	}

	ally = map_find_all(context->map, &context->player, &isally, &acount);
	enemy = map_find_all(context->map, &context->player, &isenemy, &ecount);
	
	unsigned int	i;

	for (i = 0; i < acount; i ++)
	{
		fprintf(stderr, "Ally %u: %u %u d:%u team:%c\n", i, ally[i].x, ally[i].y, ally[i].d, ally[i].team);
	}
	for (i = 0; i < ecount; i ++)
	{
		fprintf(stderr, "Enemy %u: %u %u d:%u team:%c\n", i, enemy[i].x, enemy[i].y, enemy[i].d, enemy[i].team);
	}

	free(ally);
	free(enemy);

	/*
	int				timeout;
	unsigned int	x;
	unsigned int	y;

	timeout = 10;
	while (timeout)
	{
		if (isoutofrange(context->player.pos.x - 1, context->player.pos.y))
			x = context->player.pos.x + 1;
		else if (isoutofrange(context->player.pos.x + 1, context->player.pos.y))
			x = context->player.pos.x - 1;
		else
			x = context->player.pos.x + rand() % 3 - 1;
		if (isoutofrange(context->player.pos.x, context->player.pos.y - 1))
			y = context->player.pos.y + 1;
		else if (isoutofrange(context->player.pos.x, context->player.pos.y + 1))
			y = context->player.pos.y - 1;
		else
			y = context->player.pos.y + (x == context->player.pos.x ? rand() % 3 - 1 : 0);
		if (!isoutofrange(x, y))
		{
			if (isempty(context->map[y][x]))
			{
				context->map[context->player.pos.y][context->player.pos.x] = MAP_EMPTYCASE;
				context->map[y][x] = context->player.id;
				context->player.pos.x = x;
				context->player.pos.y = y;
				break ;
			}
		}
		timeout -= 1;
	}
	*/
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

void	loop(t_context *context)
{
	int		die = 0;

	while (!die /* && (!context->prime || context->gamestate == GAMESTATE_OVER) */)
	{
		sem_wait(context->sem_id);
		if (context->shm->state == GAMESTATE_OVER ||
			isdead(&context->player, context->map))
			die = 1;
		else if (context->shm->state == GAMESTATE_ON)
			ia(context);
		if (context->prime)
			display(context->shm);
		sem_post(context->sem_id);
		usleep(200000);

		//static int timeout = 10;
		//if (timeout -- == 0) break ;
	}
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
	init(&context, argv[1][0]);
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
	else
	{
		loop(&context);
	}
	sem_wait(context.sem_id);
	player_erase(&context.player, context.map);
	sem_post(context.sem_id);
	end(&context);
	return (0);
}
