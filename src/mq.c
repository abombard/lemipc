/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mq.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 17:57:29 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 17:57:33 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemipc.h"
#include "mq.h"

#include <sys/ipc.h>
#include <sys/msg.h>

void	mq_get(int *mqid)
{
	*mqid = msgget(IPCKEY, IPC_CREAT | 0644);
	if (*mqid == -1)
	{
		perror("msgget");
		exit(EXIT_FAILURE);
	}
}

void	mq_destroy(int mqid)
{
	if (msgctl(mqid, IPC_RMID, 0) == -1)
	{
		perror("msgctl IPC_RMID");
		exit(EXIT_FAILURE);
	}
}

void	mq_send(int mqid, int type, void *msg, size_t msgsize)
{
	struct s_msgbuf	buf;

	buf.mtype = type;
	ft_memcpy(buf.mtext, msg, msgsize);
	msgsnd(mqid, &buf, msgsize, IPC_NOWAIT);
}

ssize_t	mq_recv(int mqid, int type, char *msg, size_t msgsize)
{
	struct s_msgbuf	buf;
	ssize_t			size;

	size = msgrcv(mqid, &buf, msgsize, type, IPC_NOWAIT);
	if (size <= 0 || (size_t)size >= msgsize)
		return (0);
	ft_memcpy(msg, buf.mtext, (size_t)size);
	msg[size] = '\0';
	return (size);
}
