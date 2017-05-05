#ifndef MQ_H
# define MQ_H

struct s_msgbuf
{
	long mtype;
	char mtext[128];
};

#endif
