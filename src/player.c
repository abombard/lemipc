/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 18:11:47 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 18:12:21 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

int		player_init(t_player *player, char *map[MAP_HEIGHT], char team)
{
	int		x;
	int		y;
	int		timeout;

	timeout = 500;
	while (--timeout)
	{
		x = rand() % MAP_WIDTH;
		y = rand() % MAP_HEIGHT;
		if (map[y][x] == MAP_EMPTYCASE)
		{
			map[y][x] = team;
			break ;
		}
	}
	if (!timeout)
	{
		fprintf(stderr, "Error: Couldnt position the new player on the map\n");
		return (0);
	}
	player->id = team;
	player->pos.x = (unsigned int)x;
	player->pos.y = (unsigned int)y;
	return (1);
}

void	player_erase(t_player *player, char **map)
{
	map[player->pos.y][player->pos.x] = MAP_EMPTYCASE;
}
