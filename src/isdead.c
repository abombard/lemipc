/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isdead.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 17:53:12 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 17:57:06 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"
#include "isdead.h"

static void		addenemy(t_enemies *enemies, char **map, t_pos p)
{
	unsigned int	i;

	i = 0;
	while (i < enemies->count)
	{
		if (enemies->arr[i].team == map[p.y][p.x])
		{
			enemies->arr[i].count += 1;
			break ;
		}
		i += 1;
	}
	if (i != enemies->count)
		return ;
	enemies->arr =
		realloc(enemies->arr, sizeof(t_enemies) * (enemies->count + 1));
	if (!enemies->arr)
	{
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	enemies->arr[enemies->count].team = map[p.y][p.x];
	enemies->arr[enemies->count].count = 1;
	enemies->count += 1;
}

static int		theygotcha(t_enemies *enemies)
{
	unsigned int	i;

	i = 0;
	while (i < enemies->count)
	{
		if (enemies->arr[i].count > 1)
			return (1);
		i += 1;
	}
	return (0);
}

int				isdead(t_player *player, char **map)
{
	static int		delta[][2] = {
		{ -1, -1 }, { -1, 0 }, { -1, 1 },
		{ 0, -1 }, { 0, 0 }, { 0, 1 },
		{ 1, -1 }, { 1, 0 }, { 1, 1 }
	};
	t_enemies		enemies;
	t_pos			p;
	unsigned int	i;

	enemies.arr = NULL;
	enemies.count = 0;
	i = 0;
	while (i < sizeof(delta) / sizeof(delta[0]))
	{
		p.x = (unsigned int)((int)player->pos.x + delta[i][0]);
		p.y = (unsigned int)((int)player->pos.y + delta[i][1]);
		if (!isoutofrange(p.x, p.y) && isenemy(player, map[p.y][p.x]))
		{
			addenemy(&enemies, map, p);
		}
		i += 1;
	}
	i = (unsigned int)theygotcha(&enemies);
	free(enemies.arr);
	return ((int)i);
}
