#include "lemipc.h"

void	map2d_from_game(t_game *game, char *map[MAP_HEIGHT])
{
	int		i;

	i = 0;
	while (i < MAP_HEIGHT)
	{
		map[i] = game->m + (i + 1) * TRUEMAP_WIDTH;
		i += 1;
	}
}

void	init_game(t_game *game)
{
	char	*line;
	int		i;

	game->state = GAMESTATE_INIT;
	ft_memset(game->m, MAP_BORDERWIDTH, TRUEMAP_WIDTH - 1);
	game->m[TRUEMAP_WIDTH - 1] = '\n';
	ft_memset(game->m + sizeof(game->m) - TRUEMAP_WIDTH, MAP_BORDERWIDTH, TRUEMAP_WIDTH - 1);
	game->m[sizeof(game->m) - 1] = '\n';
	i = 0;
	while (i < MAP_HEIGHT)
	{
		line = game->m + ((i + 1) * TRUEMAP_WIDTH);
		line[0] = MAP_BORDERHEIGHT;
		line[TRUEMAP_WIDTH - 2] = MAP_BORDERHEIGHT;
		line[TRUEMAP_WIDTH - 1] = '\n';
		ft_memset(line + 1, MAP_EMPTYCASE, MAP_WIDTH);
		i += 1;
	}
}

#include <time.h>
void	init_player(t_player *player, char id, t_game *game)
{
	char	*map[MAP_HEIGHT];
	int		x;
	int		y;

	map2d_from_game(game, map);
	player->id = id;
	srand(time(NULL));
	while (1)
	{
		x = rand() % MAP_WIDTH;
		y = rand() % MAP_HEIGHT;
		if (map[y][x] == MAP_EMPTYCASE)
		{
			map[y][x] = player->id;
			break ;
		}
	}
	player->pos.x = x;
	player->pos.y = y;
}

void	init(t_context *context, t_player *player, char id)
{
	int		owner;
	int		shmfd;
	sem_t	*sem_id;
	t_game	*game;

	shm_init(&shmfd, &owner);
	sem_attach(&sem_id, owner);
	game_alloc(&game, shmfd);
	sem_wait(sem_id);
	if (owner)
	{
		init_game(game);
	}
	init_player(player, id, game);
	sem_post(sem_id);
	context->owner = owner;
	context->shmfd = shmfd;
	context->sem_id = sem_id;
	context->game = game;
}

void	end(t_context *context)
{
	game_free(context->game);
	sem_detach(context->sem_id);
	if (context->owner)
	{
		sem_erase(context->sem_id);
		shm_erase(context->shmfd);
	}
}

void	display(t_game *game)
{
	int		i;

	write(1, game->m, sizeof(game->m));
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

int		loop(t_game *game, t_player *player)
{
	char	*map[MAP_HEIGHT];

	//if (game->state == GAMESTATE_INIT)
	//	return (0);
	if (game->state == GAMESTATE_OVER)
		return (1);
	map2d_from_game(game, map);
	if (isdead(player, map))
		return (1);
	ia(player, map);
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

	int		count = 0; //TEMP

	die = 0;
	init(&context, &player, argv[1][0]);
	while (!die /* && (!context.owner || context.game->state == GAMESTATE_OVER) */)
	{
		sem_wait(context.sem_id);
		die = loop(context.game, &player);
		if (context.owner)
		{
			display(context.game);
		}
		sem_post(context.sem_id);
		usleep(200000);

		//TEMP
		if (count ++ == 20)
			break ;
	}
	end(&context);
	return (EXIT_SUCCESS);
}
