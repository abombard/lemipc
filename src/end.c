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
