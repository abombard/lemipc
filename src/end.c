/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 17:52:08 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 17:52:10 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	end(t_context *context, int last_process)
{
	if (last_process)
	{
		mq_destroy(context->mqid);
		sem_destroy(context->semid);
		shm_detach(context->shm);
		shm_destroy(context->shmid);
	}
	else
	{
		shm_detach(context->shm);
	}
}
