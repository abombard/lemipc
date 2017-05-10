/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iabombard.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 18:10:31 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 18:10:38 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"
#include <math.h>

static size_t	pcount_d(t_lp *ps, size_t pcount, size_t d)
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

static int		isbetween(t_pos *p1, t_pos *p2, t_pos *target)
{
	return ((p1->x <= target->x && target->x <= p2->x &&
			p1->y <= target->y && target->y <= p2->y) ||
			(p1->x >= target->x && target->x >= p2->x &&
			p1->y >= target->y && target->y >= p2->y));
}

static int		easytarget(t_pos *target, t_pos *ppos,
						t_parr *ally, t_parr *enemy)
{
	t_pos	a;
	t_pos	e;
	size_t	i;
	size_t	j;

	enemy->close = pcount_d(enemy->arr, enemy->total, 5);
	ally->close = pcount_d(ally->arr, ally->total, 8);
	i = 0;
	while (i < enemy->close)
	{
		e = *(t_pos *)&enemy->arr[i];
		j = 0;
		while (j < ally->close)
		{
			a = *(t_pos *)&ally->arr[j];
			if (isbetween(ppos, &a, &e))
			{
				*target = e;
				return (1);
			}
			j += 1;
		}
		i += 1;
	}
	return (0);
}

static int		ia2(t_context *context,
					t_parr ally, t_parr enemy, t_pos *target)
{
	char	action[128];

	if (recv_target(context->mqid, context->player.id, action, target))
		;
	else if (enemy.total &&
			(ally.close == ally.total || ally.close > enemy.close))
	{
		target->x = enemy.arr[0].x;
		target->y = enemy.arr[0].y;
		if (ally.close == ally.total)
			send_target(context->mqid, context->player.id, "attack", target);
	}
	else if (ally.total && ally.close < ally.total)
	{
		target->x = ally.arr[ally.close].x + rand() % 3 - 1;
		target->y = ally.arr[ally.close].y + rand() % 3 - 1;
	}
	else if (enemy.total)
	{
		target->x = context->player.pos.x +
			(context->player.pos.x - enemy.arr[0].x) % 1;
		target->y = context->player.pos.y +
			(context->player.pos.y - enemy.arr[0].y) % 1;
	}
	return (1);
}

void			iabombard(t_context *context)
{
	t_parr	ally;
	t_parr	enemy;
	t_pos	target;

	ally.arr = find_all(context->map, &context->player,
						&isally, &ally.total);
	enemy.arr = find_all(context->map, &context->player,
						&isenemy, &enemy.total);
	if (!easytarget(&target, &context->player.pos, &ally, &enemy))
	{
		enemy.close = pcount_d(enemy.arr, enemy.total, 10);
		ally.close = pcount_d(ally.arr, ally.total, 8);
		ia2(context, ally, enemy, &target);
	}
	moveto(context->map, &context->player, &target);
	free(ally.arr);
	free(enemy.arr);
}
