/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iaduban_movements.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 19:18:40 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 19:18:41 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void		takeoff_player(t_context *context)
{
	int x;
	int y;

	x = 0;
	y = 0;
	while (y < MAP_HEIGHT)
	{
		x = 0;
		while (x < MAP_WIDTH)
		{
			if (x == context->player.pos.x && y == context->player.pos.y)
				context->map[y][x] = MAP_EMPTYCASE;
			x++;
		}
		y++;
	}
}

void		place_player(t_context *context)
{
	int x;
	int y;

	x = 0;
	y = 0;
	while (y < MAP_HEIGHT)
	{
		x = 0;
		while (x < MAP_WIDTH)
		{
			if (x == context->player.pos.x && y == context->player.pos.y)
				context->map[y][x] = context->player.id;
			x++;
		}
		y++;
	}
}

void		move_x(t_context *context, int x_target)
{
	if (x_target > context->player.pos.x &&
			context->map[context->player.pos.y][context->player.pos.x + 1]
			== MAP_EMPTYCASE)
		context->player.pos.x += 1;
	else if (x_target < context->player.pos.x &&
			context->map[context->player.pos.y][context->player.pos.x - 1]
			== MAP_EMPTYCASE)
		context->player.pos.x -= 1;
	else
		moverand(&context->player, context->map,
				&context->player.pos.x, &context->player.pos.y);
}

void		move_y(t_context *context, int y_target)
{
	if (y_target > context->player.pos.y &&
			context->map[context->player.pos.y + 1][context->player.pos.x]
			== MAP_EMPTYCASE)
		context->player.pos.y += 1;
	else if (y_target < context->player.pos.y &&
			context->map[context->player.pos.y - 1][context->player.pos.x]
			== MAP_EMPTYCASE)
		context->player.pos.y -= 1;
	else
		moverand(&context->player, context->map,
				&context->player.pos.x, &context->player.pos.y);
}
