#include "lemipc.h"

static int		isempty(char c)
{
	return (c == MAP_EMPTYCASE);
}

static int		isally(t_player *player, char c)
{
	return (c == player->id);
}

static int		isenemy(t_player *player, char c)
{
	return (!isempty(c) && !isally(player, c));
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

static int		isoutofrange(unsigned int x, unsigned int y)
{
	return (x >= MAP_WIDTH || y >= MAP_HEIGHT);
}

void	random_move(t_player *player, char **map)
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

void	move_player(t_player *player, t_pos *target, char **map)
{
	if (isoutofrange(target->x, target->y))
	{
		fprintf(stderr, "Out of range ! %u %u\n", target->x, target->y);
		return ;
	}
	map[player->pos.y][player->pos.x] = MAP_EMPTYCASE;
	if (target->x < player->pos.x &&
		map[player->pos.y][player->pos.x - 1] == MAP_EMPTYCASE)
		player->pos.x -= 1;
	else if (target->x > player->pos.x &&
		map[player->pos.y][player->pos.x + 1] == MAP_EMPTYCASE)
		player->pos.x += 1;
	else if (target->y < player->pos.y &&
		map[player->pos.y - 1][player->pos.x] == MAP_EMPTYCASE)
		player->pos.y -= 1;
	else if (target->y > player->pos.y &&
		map[player->pos.y + 1][player->pos.x] == MAP_EMPTYCASE)
		player->pos.y += 1;
	else
		random_move(player, map);
	map[player->pos.y][player->pos.x] = player->id;
}

void	ia(t_context *context)
{
	t_lp	*ally;
	size_t	acount;
	t_lp	*enemy;
	size_t	ecount;

	char			msg[1024];
	ssize_t			size;
	unsigned int	prio = 1;
	struct timespec	timeout;

	ally = map_find_all(context->map, &context->player, &isally, &acount);
	enemy = map_find_all(context->map, &context->player, &isenemy, &ecount);

	size = mq_timedreceive(context->player.mq, msg, sizeof(msg), &prio, &timeout);
	if (size != -1)
	{
		t_pos	target;
		size = sscanf(msg, "%u %u", &target.x, &target.y);
		move_player(&context->player, &target, context->map);
	}

	if (ecount)
	{
		size = snprintf(msg, sizeof(msg), "%u %u", enemy[0].x, enemy[0].y);
		timeout.tv_sec = 1;
		timeout.tv_nsec = 0;
		mq_timedsend(context->player.mq, msg, (unsigned long)size, prio, &timeout);
	}

	/*
	unsigned int	i;

	for (i = 0; i < acount; i ++)
	{
		fprintf(stderr, "Ally %u: %u %u d:%u team:%c\n", i, ally[i].x, ally[i].y, ally[i].d, ally[i].team);
	}
	for (i = 0; i < ecount; i ++)
	{
		fprintf(stderr, "Enemy %u: %u %u d:%u team:%c\n", i, enemy[i].x, enemy[i].y, enemy[i].d, enemy[i].team);
	}
	*/

	free(ally);
	free(enemy);

	/*
	*/
}

