/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 18:10:19 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 18:10:22 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	loop(t_context *context)
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
		if (context->shm->state == GAMESTATE_OVER ||
			isdead(&context->player, context->map))
			die = 1;
		else if (context->shm->state == GAMESTATE_ON)
			context->algo == 1 ? iabombard(context) : iaduban(context);
		sem_post(context->semid);
		usleep(200000);
	}
}

void	launcher(t_context *context, char team, int *over)
{
	int		success;

	signal(SIGINT, &sighandler);
	sem_wait(context->semid);
	success = player_init(&context->player, context->map, team);
	sem_post(context->semid);
	if (!success)
		return ;
	loop(context);
	sem_wait(context->semid);
	player_erase(&context->player, context->map);
	*over = gameisover(context->map);
	sem_post(context->semid);
}

void	command(t_context *context, char *cmd)
{
	if (!ft_strcmp(cmd, "start"))
	{
		sem_wait(context->semid);
		context->shm->state = GAMESTATE_ON;
		sem_post(context->semid);
	}
	else if (!ft_strcmp(cmd, "stop"))
	{
		sem_wait(context->semid);
		context->shm->state = GAMESTATE_OVER;
		sem_post(context->semid);
	}
	else if (!ft_strcmp(cmd, "display"))
	{
		loop_display(context);
	}
	else
	{
		fprintf(stderr, "Invalid command: %s\n", cmd);
	}
}

int		main(int argc, char **argv)
{
	t_context	context;
	int			over;

	if (argc != 2 && argc != 3)
	{
		printf("Usage: %s <Team Id> [algo:1|2]\n", argv[0]);
		return (2);
	}
	srand((unsigned int)time(NULL));
	init(&context, argv[2]);
	over = 0;
	if (ft_strlen(argv[1]) > 1)
		command(&context, argv[1]);
	else
		launcher(&context, argv[1][0], &over);
	end(&context, over);
	return (0);
}
