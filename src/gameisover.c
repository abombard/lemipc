#include "lemipc.h"

int		gameisover(char **map)
{
	int		i;
	int		j;

	i = 0;
	while (i < MAP_HEIGHT)
	{
		j = 0;
		while (j < MAP_WIDTH)
		{
			if (!isempty(map[i][j]))
				return (0);
			j += 1;
		}
		i += 1;
	}
	return (1);
}
