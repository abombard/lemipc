/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lp.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 18:13:43 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 18:14:12 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"
#include <math.h>

static int			lpcmp(void const *a, void const *b)
{
	t_lp	*p1;
	t_lp	*p2;

	p1 = (t_lp *)a;
	p2 = (t_lp *)b;
	return ((int)p1->d - (int)p2->d);
}

static unsigned int	compute_distance(t_pos *p1, t_pos *p2)
{
	int		x;
	int		y;

	x = (int)p1->x - (int)p2->x;
	y = (int)p1->y - (int)p2->y;
	return ((unsigned int)sqrt(x * x + y * y));
}

static t_lp			*psgrow(t_lp *arr, size_t size)
{
	arr = realloc(arr, (size + 1) * sizeof(t_lp));
	if (!arr)
	{
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	return (arr);
}

static t_lp			newp(t_player *player, char **map,
						unsigned int x, unsigned int y)
{
	t_lp	p;
	t_pos	pos;

	p.x = x;
	p.y = y;
	p.team = map[y][x];
	pos.x = x;
	pos.y = y;
	p.d = compute_distance(&player->pos, &pos);
	return (p);
}

t_lp				*find_all(char **map, t_player *player,
		int (*test)(t_player *, char), size_t *count)
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
				ps = psgrow(ps, pcount);
				ps[pcount++] = newp(player, map, p.x, p.y);
			}
			p.x += 1;
		}
		p.y += 1;
	}
	qsort(ps, pcount, sizeof(t_lp), &lpcmp);
	*count = pcount;
	return (ps);
}
