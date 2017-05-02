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
# include <errno.h>

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

typedef enum	e_gamestate
{
	GAMESTATE_INIT = 0,
	GAMESTATE_ON,
	GAMESTATE_OVER
}				t_gamestate;

typedef struct	s_game
{
	t_gamestate		state;
	char			m[MAP_SIZE];
}				t_game;

# define IPC_OBJPATH	"/ipc_objpath"
# define IPC_SEMNAME	"/ipc_semname"

void	shm_init(int *shmfd, int *owner);
void	shm_erase(int shmfd);

void	sem_attach(sem_t **sem_id, int owner);
void	sem_detach(sem_t *sem_id);
void	sem_erase(sem_t *sem_id);

void	game_alloc(t_game **game, int shmfd);
void	game_free(t_game *game);

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

/*
** Process context
*/
typedef struct	s_context
{
	int			owner;
	int			shmfd;
	sem_t		*sem_id;
	t_game		*game;
}				t_context;

typedef struct	s_player
{
	char	id;
	t_pos	pos;
	t_task	task;
}				t_player;

#endif
