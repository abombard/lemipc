/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moverand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 18:12:57 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 18:18:54 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	moverand(t_player *player, char **map,
		unsigned int *x1, unsigned int *y1)
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
