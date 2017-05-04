#include "lemipc.h"

#include <math.h>
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
		random_move(player, map, &x1, &y1);
	map[y1][x1] = player->id;
	player->pos.x = x1;
	player->pos.y = y1;
}

void	send_target(t_player *player, char *action, t_pos *target, int count)
{
	char			msg[1024];
	ssize_t			size;
	int				i;

	size = snprintf(msg, sizeof(msg), "%s %u %u", action, target->x, target->y);
	i = 0;
	while (i < count)
	{
		mq_send(player->mq, player->id, msg, size);
		i += 1;
	}
}

int		recv_target(t_player *player, t_pos *target, char *action)
{
	char			msg[1024];
	ssize_t			size;

	size = mq_recv(player->mq, player->id, msg, sizeof(msg));
	if (size <= 0)
		return (0);
	fprintf(stderr, "recv msg %s\n", msg);
	size = sscanf(msg, "%s %u %u", action, &target->x, &target->y);
	return (1);
}

int		isbetween(t_pos *p1, t_pos *p2, t_pos *target)
{
	return ((p1->x <= target->x && target->x <= p2->x &&
			p1->y <= target->y && target->y <= p2->y) ||
			(p1->x >= target->x && target->x >= p2->x &&
			 p1->y >= target->y && target->y >= p2->y));
}

t_pos	easytarget(t_player *player, t_lp *ally, size_t acount, t_lp *enemy, size_t ecount)
{
	t_pos	a;
	t_pos	e;
	int		i;
	int		j;

	i = 0;
	while (i < ecount)
	{
		e.x = enemy[i].x;
		e.y = enemy[i].y;
		j = 0;
		while (j < acount)
		{
			a.x = ally[j].x;
			a.y = ally[j].y;
			if (isbetween(&player->pos, &a, &e))
				return (e);
			j += 1;
		}
		i += 1;
	}
	e.x = 0;
	e.y = 0;
	return (e);
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

	size_t	eclosecount = pcount_d(enemy, ecount, 5);
	size_t	aclosecount = pcount_d(ally, acount, 8);

	target = easytarget(&context->player, ally, aclosecount, enemy, eclosecount);

	eclosecount = pcount_d(enemy, ecount, 10);
	aclosecount = pcount_d(ally, acount, 8);

	if (target.x != 0 && target.y != 0)
	{
		//printf("He's between!\n");
	}
	else if (recv_target(&context->player, &target, action))
	{
		fprintf(stderr, "recv target ! %s %u %u\n", action, target.x, target.y);
	}
	else if (ecount && aclosecount == acount)
	{
		target.x = enemy[0].x;
		target.y = enemy[0].y;

		send_target(&context->player, "attack", &target, 1);
	}
	else if (ecount && aclosecount > eclosecount)
	{
		target.x = enemy[0].x;
		target.y = enemy[0].y;
	}
	else if (acount && aclosecount != acount)
	{
		target.x = ally[aclosecount].x + rand() % 3 - 1;
		target.y = ally[aclosecount].y + rand() % 3 - 1;
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

