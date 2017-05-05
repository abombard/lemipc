/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chan.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 17:52:28 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 17:52:30 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"

void	send_target(int mqid, int team, char *action, t_pos *target)
{
	char			msg[1024];
	ssize_t			size;

	size = snprintf(msg, sizeof(msg), "%s %u %u", action, target->x, target->y);
	mq_send(mqid, team, msg, size);
}

int		recv_target(int mqid, int team, char *action, t_pos *target)
{
	char			msg[1024];
	ssize_t			size;

	size = mq_recv(mqid, team, msg, sizeof(msg));
	if (size <= 0)
		return (0);
	size = sscanf(msg, "%s %u %u", action, &target->x, &target->y);
	return (1);
}
