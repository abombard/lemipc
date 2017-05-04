#include "lemipc.h"
#include <math.h>

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

int		lpcmp(void const *a, void const *b)
{
	t_lp	*p1;
	t_lp	*p2;

	p1 = (t_lp *)a;
	p2 = (t_lp *)b;
	return ((int)p1->d - (int)p2->d);
}

unsigned int	distance(t_pos *p1, t_pos *p2)
{
	int	x = (int)p1->x - (int)p2->x;
	int	y = (int)p1->y - (int)p2->y;
	return ((unsigned int)sqrt(x * x + y * y));

}

t_lp	*find_all(char **map, t_player *player, int (*test)(t_player *, char), size_t *count)
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
			if (!(p.x == player->pos.x && p.y == player->pos.y) &&
				test(player, map[p.y][p.x]))
			{
				if (!(ps = realloc(ps, (pcount + 1) * sizeof(t_lp))))
				{
					perror("realloc");
					exit(EXIT_FAILURE);
				}
				p.team = map[p.y][p.x];
				t_pos	tmp = { p.x, p.y };
				p.d = distance(&player->pos, &tmp);
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

