#include "lemipc.h"
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf {
	long mtype;       /* message type, must be > 0 */
	char mtext[128];    /* message data */
};

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
	struct msgbuf	buf;

	ft_memcpy(buf.mtext, msg, msgsize);
	msgsnd(mqid, &buf, msgsize, IPC_NOWAIT);
}

ssize_t	mq_recv(int mqid, int type, char *msg, size_t msgsize)
{
	struct msgbuf	buf;
	ssize_t			size;

	size = msgrcv(mqid, &buf, msgsize, type, IPC_NOWAIT);
	if (size <= 0 || size >= msgsize)
		return (0);
	ft_memcpy(msg, buf.mtext, size);
	msg[size] = '\0';
	return (size);
}
