/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lemipc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 18:51:37 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 18:52:50 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# define MAP_WIDTH		100
# define MAP_HEIGHT		40

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

typedef struct	s_shm
{
	t_gamestate		state;
	char			m[MAP_SIZE];
}				t_shm;

# define IPCKEY		(key_t)0xdeadbeef

void			shm_get(int *shmid, int *created);
void			shm_destroy(int shmid);

void			shm_attach(t_shm **shmaddr, int shmid);
void			shm_detach(void *shmaddr);

void			sem_get(int *semid, int creator);
void			sem_destroy(int semid);

void			sem_wait(int semid);
void			sem_post(int semid);

void			mq_get(int *mqid);
void			mq_destroy(int mqid);

void			mq_send(int mqid, int type, void *msg, size_t msgsize);
ssize_t			mq_recv(int mqid, int type, char *msg, size_t msgsize);

/*
** Player specific
*/

typedef struct	s_pos
{
	unsigned int	x;
	unsigned int	y;
}				t_pos;

typedef struct	s_player
{
	int		prime;
	char	id;
	t_pos	pos;
}				t_player;

int				player_init(t_player *player, char **map, char team);
void			player_erase(t_player *player, char **map);

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

void			sighandler(int signum);

void			init(t_context *context, char *algo);
void			end(t_context *context, int last_process);

int				isempty(char c);
int				isally(t_player *player, char c);
int				isenemy(t_player *player, char c);
int				isoutofrange(unsigned int x, unsigned int y);
int				isdead(t_player *player, char **map);

int				ateamwin(char **map, char *winner);
int				gameisover(char **map);

void			moveto(char **map, t_player *player, t_pos *target);
void			moverand(t_player *player, char **map,
						unsigned int *x1, unsigned int *y1);

void			send_target(int mqid, int team, char *action, t_pos *target);
int				recv_target(int mqid, int team, char *action, t_pos *target);

typedef struct	s_lp
{
	unsigned int	x;
	unsigned int	y;
	unsigned int	d;
	char			team;
}				t_lp;

t_lp			*find_all(char **map, t_player *player,
						int (*test)(t_player *, char), size_t *count);

typedef struct	s_parr
{
	t_lp	*arr;
	size_t	total;
	size_t	close;
}				t_parr;

void			iabombard(t_context *context);

void			move_y(t_context *context, int y_target);
void			move_x(t_context *context, int x_target);
void			place_player(t_context *context);
void			takeoff_player(t_context *context);
void			iaduban(t_context *context);

void			loop_display(t_context *context);

#endif
