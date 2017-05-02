#include "lemipc.h"

static void	mqname_create(char id, char name[MQNAME_SIZE + 1])
{
	ft_memcpy(name, MQNAME_BASIS, MQNAME_BASIS_SIZE);
	name[MQNAME_BASIS_SIZE] = id;
	name[MQNAME_SIZE] = '\0';
}

void	mq_attach(char team, mqd_t *mq, int *created)
{
	mqd_t			mqid;
	char			mqname[MQNAME_SIZE + 1];
	struct mq_attr	attr;

	mqname_create(team, mqname);
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 1024;
	attr.mq_curmsgs = 0;
	mqid = mq_open(mqname, O_CREAT | O_EXCL | O_RDWR, 0644, &attr);
	if (mqid == (mqd_t)-1)
	{
		if (errno != EEXIST)
		{
			perror("mq_open");
			exit(EXIT_FAILURE);
		}
		mqid = mq_open(mqname, O_CREAT | O_RDWR);
		if (mqid == (mqd_t)-1)
		{
			perror("mq_open");
			exit(EXIT_FAILURE);
		}
		*created = 0;
	}
	else
	{
		*created = 1;
	}
	*mq = mqid;
}

void	mq_detach(mqd_t mq)
{
	if (mq_close(mq))
	{
		perror("mq_close");
		exit(EXIT_FAILURE);
	}
}

void	mq_erase(char id)
{
	char	mqname[MQNAME_SIZE + 1];

	mqname_create(id, mqname);
	if (mq_unlink(mqname))
	{
		perror("mq_unlink");
		exit(EXIT_FAILURE);
	}
}
