/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 17:52:47 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 17:52:48 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

int		isempty(char c)
{
	return (c == MAP_EMPTYCASE);
}

int		isally(t_player *player, char c)
{
	return (c == player->id);
}

int		isenemy(t_player *player, char c)
{
	return (!isempty(c) && !isally(player, c));
}

int		isoutofrange(unsigned int x, unsigned int y)
{
	return (x >= MAP_WIDTH || y >= MAP_HEIGHT);
}
