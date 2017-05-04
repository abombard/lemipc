#include "lemipc.h"

void	player_init(t_player *player, char *map[MAP_HEIGHT], char team)
{
	int		created;
	int		x;
	int		y;

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
	player->id = team;
	player->pos.x = (unsigned int)x;
	player->pos.y = (unsigned int)y;
	player->task.id = PLAYERTASK_UNDEFINED;
}

void	player_erase(t_player *player, char **map, int *last_player)
{
	int		i;
	int		j;

	map[player->pos.y][player->pos.x] = MAP_EMPTYCASE;
	*last_player = 1;
	i = 0;
	while (i < MAP_HEIGHT)
	{
		j = 0;
		while (j < MAP_WIDTH)
		{
			if (!isempty(map[i][j]))
				*last_player = 0;
			j += 1;
		}
		i += 1;
	}
}
