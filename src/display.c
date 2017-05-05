/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 17:52:37 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 17:52:38 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

static void		display(t_shm *shm)
{
	int		i;

	write(1, shm->m, sizeof(shm->m));
	i = 0;
	while (i <= TRUEMAP_HEIGHT)
	{
		printf("\033[F");
		i += 1;
	}
	printf("\n");
}

void			loop_display(t_context *context)
{
	int		die;
	char	winner;

	die = 0;
	while (!die)
	{
		sem_wait(context->semid);
		if (context->shm->state != GAMESTATE_INIT &&
			ateamwin(context->map, &winner))
			die = 1;
		display(context->shm);
		sem_post(context->semid);
		usleep(200000);
	}
	fprintf(stderr, "Winner %c\n", winner);
}
