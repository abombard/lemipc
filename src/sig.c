/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/10 15:49:08 by abombard          #+#    #+#             */
/*   Updated: 2017/05/10 15:49:09 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

static int	islastplayer(char **map)
{
	size_t	x;
	size_t	y;
	size_t	count;

	count = 0;
	x = 0;
	while (x < MAP_HEIGHT)
	{
		y = 0;
		while (y < MAP_WIDTH)
		{
			if (!isempty(map[x][y]))
				count += 1;
			y += 1;
		}
		x += 1;
	}
	return (count <= 1);
}

void		sighandler(int signum)
{
	t_context	context;

	(void)signum;
	init(&context, NULL);
	end(&context, islastplayer(context.map));
	exit(EXIT_SUCCESS);
}
