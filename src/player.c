#include "lemipc.h"

int		player_init(t_player *player, char *map[MAP_HEIGHT], char team)
{
	int		created;
	int		x;
	int		y;
	int		timeout;

	timeout = 100;
	while (timeout)
	{
		x = rand() % MAP_WIDTH;
		y = rand() % MAP_HEIGHT;
		if (map[y][x] == MAP_EMPTYCASE)
		{
			map[y][x] = team;
			break ;
		}
		timeout -= 1;
	}
	if (!timeout)
	{
		fprintf(stderr, "Error: Couldnt position the new player on the map\n");
		return (0);
	}
	player->id = team;
	player->pos.x = (unsigned int)x;
	player->pos.y = (unsigned int)y;
	return (1);
}

void	player_erase(t_player *player, char **map)
{
	map[player->pos.y][player->pos.x] = MAP_EMPTYCASE;
}
