/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gameisover.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 17:52:19 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 18:28:00 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

int		gameisover(char **map)
{
	int		i;
	int		j;

	i = 0;
	while (i < MAP_HEIGHT)
	{
		j = 0;
		while (j < MAP_WIDTH)
		{
			if (!isempty(map[i][j]))
				return (0);
			j += 1;
		}
		i += 1;
	}
	return (1);
}
