#ifndef LEMIPC_H
# define LEMIPC_H

# include "libft.h"

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <time.h>

# include <sys/ipc.h>
# include <sys/msg.h>

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

# define IPCKEY		0xdeadbeef

void	shm_get(int *shmid, int *created);
void	shm_destroy(int shmid);

void	shm_attach(t_shm **shmaddr, int shmid);
void	shm_detach(void *shmaddr);

void	sem_get(int *semid, int creator);
void	sem_destroy(int semid);

void	sem_wait(int semid);
void	sem_post(int semid);

void	mq_get(int *mqid);
void	mq_destroy(int mqid);

void	mq_send(int mqid, int type, void *msg, size_t msgsize);
ssize_t	mq_recv(int mqid, int type, char *msg, size_t msgsize);

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
	int		mq;
	t_pos	pos;
	t_task	task;
}				t_player;

void	player_init(t_player *player, char *map[MAP_HEIGHT], char team);
void	player_erase(t_player *player, char **map, int *last_player);

/*
** Process context
*/
typedef struct	s_context
{
	int			algo;

	int			creator;
	int			shmid;
	t_shm		*shm;
	int			semid;
	int			mqid;

	char		*map[MAP_HEIGHT];

	t_player	player;
}				t_context;

void	iaduban(t_context *context);
void	iabombard(t_context *context);

int		isempty(char c);
int		isally(t_player *player, char c);
int		isenemy(t_player *player, char c);
int		isoutofrange(unsigned int x, unsigned int y);
void	random_move(t_player *player, char **map, unsigned int *x1, unsigned int *y1);

#endif
