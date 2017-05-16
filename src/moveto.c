/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moveto.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 18:13:07 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 18:28:20 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

static t_pos	movetox(char **map, t_player *player, t_pos *target)
{
	t_pos			d;
	unsigned int	x1;
	unsigned int	y1;
	unsigned int	x2;
	unsigned int	y2;

	x1 = player->pos.x;
	y1 = player->pos.y;
	x2 = target->x;
	y2 = target->y;
	d.x = x1;
	d.y = y1;
	if (x2 < x1 && isempty(map[y1][x1 - 1]))
		d.x -= 1;
	else if (x2 > x1 && isempty(map[y1][x1 + 1]))
		d.x += 1;
	else if (y2 < y1 && isempty(map[y1 - 1][x1]))
		d.y -= 1;
	else if (y2 > y1 && isempty(map[y1 + 1][x1]))
		d.y += 1;
	return (d);
}

static t_pos	movetoy(char **map, t_player *player, t_pos *target)
{
	t_pos			d;
	unsigned int	x1;
	unsigned int	y1;
	unsigned int	x2;
	unsigned int	y2;

	x1 = player->pos.x;
	y1 = player->pos.y;
	x2 = target->x;
	y2 = target->y;
	d.x = x1;
	d.y = y1;
	if (y2 < y1 && isempty(map[y1 - 1][x1]))
		d.y -= 1;
	else if (y2 > y1 && isempty(map[y1 + 1][x1]))
		d.y += 1;
	else if (x2 < x1 && isempty(map[y1][x1 - 1]))
		d.x -= 1;
	else if (x2 > x1 && isempty(map[y1][x1 + 1]))
		d.x += 1;
	return (d);
}

void			moveto(char **map, t_player *player, t_pos *target)
{
	t_pos			d;

	map[player->pos.y][player->pos.x] = MAP_EMPTYCASE;
	d.x = player->pos.x;
	d.y = player->pos.y;
	if (!isoutofrange(target->x, target->y))
	{
		if (abs((int)target->x - (int)player->pos.x) >
			abs((int)target->y - (int)player->pos.y))
			d = movetox(map, player, target);
		else if (abs((int)target->x - (int)player->pos.x) <
				abs((int)target->y - (int)player->pos.y))
			d = movetoy(map, player, target);
	}
	if (!ft_memcmp(&d, &player->pos, sizeof(t_pos)))
		moverand(player, map, &d.x, &d.y);
	map[d.y][d.x] = player->id;
	player->pos = d;
}
