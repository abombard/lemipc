#include "lemipc.h"

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

void	random_move(t_player *player, char **map, unsigned int *x1, unsigned int *y1)
{
	int				timeout;
	unsigned int	x;
	unsigned int	y;

	timeout = 10;
	while (timeout)
	{
		x = player->pos.x;
		y = player->pos.y;
		x += rand() % 3 - 1;
		y += x == player->pos.x ? rand() % 3 - 1 : 0;
		if (!isoutofrange(x, y) && isempty(map[y][x]))
		{
			*x1 = x;
			*y1 = y;
			return ;
		}
		timeout -= 1;
	}
	*x1 = player->pos.x;
	*y1 = player->pos.y;
}

