/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ateamwin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 17:51:47 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 17:51:50 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

int		ateamwin(char **map, char *winner)
{
	char			team;
	unsigned int	x;
	unsigned int	y;

	team = MAP_EMPTYCASE;
	y = 0;
	while (y < MAP_HEIGHT)
	{
		x = 0;
		while (x < MAP_WIDTH)
		{
			if (map[y][x] != MAP_EMPTYCASE)
			{
				if (team == MAP_EMPTYCASE)
					team = map[y][x];
				else if (team != map[y][x])
					return (0);
			}
			x += 1;
		}
		y += 1;
	}
	*winner = team;
	return (1);
}
