#ifndef LEMIPC_H
# define LEMIPC_H

# include "libft.h"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <sys/types.h>
# include <semaphore.h>
# include <mqueue.h>
# include <errno.h>
# include <time.h>

/*
** Game IPC shared between processes
*/

# define MAP_WIDTH		60
# define MAP_HEIGHT		20

# define TRUEMAP_HEIGHT	(MAP_HEIGHT + 2)
# define TRUEMAP_WIDTH	(MAP_WIDTH + 3)

# define MAP_SIZE		(TRUEMAP_HEIGHT * TRUEMAP_WIDTH)

# define MAP_BORDERWIDTH	'-'
# define MAP_BORDERHEIGHT	'|'
# define MAP_EMPTYCASE	' '

# define MQNAME_BASIS				"/lemipcteam"
# define MQNAME_BASIS_SIZE	(sizeof("/lemipcteam") - 1)
# define MQNAME_SIZE		(MQNAME_BASIS_SIZE + 1)

typedef enum	e_gamestate
{
	GAMESTATE_INIT = 0,
	GAMESTATE_ON,
	GAMESTATE_OVER
}				t_gamestate;

typedef struct	s_shm
{
	t_gamestate		state;
	char			m[MAP_SIZE];
}				t_shm;

# define IPC_OBJPATH	"/ipc_objpath"
# define IPC_SEMNAME	"/ipc_semname"

void	shm_init(int *shmfd, int *created);
void	shm_erase(void);

void	shm_alloc(t_shm **shm, int shmfd);
void	shm_free(t_shm *shm);

void	shm_link(char *map[MAP_HEIGHT], t_shm *shm);

void	sem_attach(sem_t **sem_id, int prime);
void	sem_detach(sem_t *sem_id);
void	sem_erase(void);

void	mq_attach(char team, mqd_t *mq, int *created);
void	mq_detach(mqd_t mq);
void	mq_erase(char id);

/*
** Player specific
*/

typedef struct	s_pos
{
	unsigned int	x;
	unsigned int	y;
}				t_pos;

typedef struct	s_task
{
	enum
	{
		PLAYERTASK_UNDEFINED = 0,
		PLAYERTASK_ATTACK
	}			id;
	union
	{
		t_pos	attack;
	}			un;
}				t_task;

typedef struct	s_player
{
	int		prime;
	char	id;
	mqd_t	mq;
	t_pos	pos;
	t_task	task;
}				t_player;

void	player_init(t_player *player, char *map[MAP_HEIGHT], char team);
void	player_erase(t_player *player, char **map);

/*
** Process context
*/
typedef struct	s_context
{
	int			prime;
	int			shmfd;
	t_shm		*shm;
	sem_t		*sem_id;

	char		*map[MAP_HEIGHT];

	t_player	player;
}				t_context;

void	iaduban(t_context *context);
void	iabombard(t_context *context);

# ifdef IADUBAN
#  define ia iaduban
# else
#  define ia iabombard
# endif

int		isempty(char c);
int		isally(t_player *player, char c);
int		isenemy(t_player *player, char c);
int		isoutofrange(unsigned int x, unsigned int y);
void	random_move(t_player *player, char **map, unsigned int *x1, unsigned int *y1);

#endif
