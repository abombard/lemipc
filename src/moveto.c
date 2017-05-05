#include "lemipc.h"

void	moveto(char **map, t_player *player, t_pos *target)
{
	unsigned int	x1;
	unsigned int	y1;
	unsigned int	x2;
	unsigned int	y2;

	x1 = player->pos.x;
	y1 = player->pos.y;
	x2 = target->x;
	y2 = target->y;
	map[y1][x1] = MAP_EMPTYCASE;
	if (!isoutofrange(x2, y2))
	{
		if (abs((int)x2 - (int)x1) > abs((int)y2 - (int)y1))
		{
			if (x2 < x1 && isempty(map[y1][x1 - 1]))
				x1 -= 1;
			else if (x2 > x1 && isempty(map[y1][x1 + 1]))
				x1 += 1;
			else if (y2 < y1 && isempty(map[y1 - 1][x1]))
				y1 -= 1;
			else if (y2 > y1 && isempty(map[y1 + 1][x1]))
				y1 += 1;
		}
		else if (abs((int)x2 - (int)x1) < abs((int)y2 - (int)y1))
		{
			if (y2 < y1 && isempty(map[y1 - 1][x1]))
				y1 -= 1;
			else if (y2 > y1 && isempty(map[y1 + 1][x1]))
				y1 += 1;
			else if (x2 < x1 && isempty(map[y1][x1 - 1]))
				x1 -= 1;
			else if (x2 > x1 && isempty(map[y1][x1 + 1]))
				x1 += 1;
		}
	}
	if (x1 == player->pos.x && y1 == player->pos.y)
		moverand(player, map, &x1, &y1);
	map[y1][x1] = player->id;
	player->pos.x = x1;
	player->pos.y = y1;
}
