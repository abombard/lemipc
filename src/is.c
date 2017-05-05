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
