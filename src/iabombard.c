#include "lemipc.h"

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
	return ((int)p1->d - (int)p2->d);
}

#include <math.h>
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

size_t	pcount_d(t_lp *ps, size_t pcount, size_t d)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (i < pcount)
	{
		if (ps[i].d < d)
			count += 1;
		else
			break ;
		i += 1;
	}
	return (count);
}

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
	if (isoutofrange(x2, y2))
	{
		fprintf(stderr, "Out of range ! %u %u\n", x2, y2);
		return ;
	}
	map[y1][x1] = MAP_EMPTYCASE;
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
	if (x1 == player->pos.x && y1 == player->pos.y)
		random_move(player, map, &x1, &x2);
	map[y1][x1] = player->id;
	player->pos.x = x1;
	player->pos.y = y1;
}

void	send_target(mqd_t mq, char *action, t_pos *target, int count)
{
	char			msg[1024];
	ssize_t			size;
	unsigned int	prio = 1;
	struct timespec	timeout;
	int				i;

	size = snprintf(msg, sizeof(msg), "%s %u %u", action, target->x, target->y);
	timeout.tv_sec = 1;
	timeout.tv_nsec = 0;
	i = 0;
	while (i < count)
	{
		mq_timedsend(mq, msg, (unsigned long)size, prio, &timeout);
		i += 1;
	}
}

int		recv_target(mqd_t mq, t_pos *target, char *action)
{
	char			msg[1024];
	ssize_t			size;
	unsigned int	prio = 1;
	struct timespec	timeout;

	timeout.tv_sec = 1;
	timeout.tv_nsec = 0;
	size = mq_timedreceive(mq, msg, sizeof(msg), &prio, &timeout);
	if (size <= 0)
		return (0);
	msg[size] = '\0';
	size = sscanf(msg, "%s %u %u", action, &target->x, &target->y);
	return (1);
}

void	iabombard(t_context *context)
{
	t_lp	*ally;
	size_t	acount;
	t_lp	*enemy;
	size_t	ecount;

	ally = find_all(context->map, &context->player, &isally, &acount);
	enemy = find_all(context->map, &context->player, &isenemy, &ecount);

	t_pos	target;
	char	action[1024];
	int		done = 0;

	if (recv_target(context->player.mq, &target, action))
	{
		if (!ft_strcmp(action, "help"))
		{
			done = 1;
		}
		else if (!ft_strcmp(action, "attack"))
		{
			if (distance(&context->player.pos, &target) < 5)
				done = 1;
			else
				send_target(context->player.mq, "attack", &target, 1);
		}
	}
	if (done)
	{

	}
	else if (ecount == 1)
	{
		target.x = enemy[0].x;
		target.y = enemy[0].y;
	}
	else
	{
		size_t	eclosecount = pcount_d(enemy, ecount, 10);
		size_t	aclosecount = pcount_d(ally, acount, 5);

		if (ecount && aclosecount > eclosecount)
		{
			target.x = enemy[0].x;
			target.y = enemy[0].y;

			send_target(context->player.mq, "attack", &context->player.pos, 1);
		}
		else if (acount)
		{
			target.x = ally[0].x + rand() % 3 - 1;
			target.y = ally[0].y + rand() % 3 - 1;

			send_target(context->player.mq, "help", &context->player.pos, acount > 2 ? 2 : 1);
		}
		else if (ecount)
		{
			target.x = context->player.pos.x + (context->player.pos.x - enemy[0].x) % 1;
			target.y = context->player.pos.y + (context->player.pos.y - enemy[0].y) % 1;
		}
		else
		{
			fprintf(stderr, "Game Over ?\n");
		}
	}

	moveto(context->map, &context->player, &target);

	/*
	printf("---------------------------------------------------\n");
	printf("%c pos %u %u\n", context->player.id, context->player.pos.x, context->player.pos.y);

	printf("pcount_d ally 5 %lu\n", pcount_d(ally, acount, 5));
	printf("pcount_d enemy 5 %lu\n", pcount_d(enemy, ecount, 5));
	printf("---------------------------------------------------\n");

	printf("-------------------------------------\n");
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

