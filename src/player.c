#include "lemipc.h"

void	player_init(t_player *player, char *map[MAP_HEIGHT], char team)
{
	mqd_t	mq;
	int		created;
	int		x;
	int		y;

	mq_attach(team, &mq, &created);
	while (1)
	{
		x = rand() % MAP_WIDTH;
		y = rand() % MAP_HEIGHT;
		if (map[y][x] == MAP_EMPTYCASE)
		{
			map[y][x] = team;
			break ;
		}
	}
	player->prime = created;
	player->id = team;
	player->mq = mq;
	player->pos.x = (unsigned int)x;
	player->pos.y = (unsigned int)y;
	player->task.id = PLAYERTASK_UNDEFINED;
}

void	player_erase(t_player *player, char **map)
{
	mq_detach(player->mq);
	if (player->prime)
	{
		mq_erase(player->id);
	}
	map[player->pos.y][player->pos.x] = MAP_EMPTYCASE;
}
