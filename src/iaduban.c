#define GNU_SOURCE
#include "lemipc.h"

#include <stdbool.h>

typedef struct	s_lp
{
	int	x;
	int	y;
	unsigned int	d;
	char			team;
}				t_lp;
static int		lpcmp(void const *a, void const *b)
{
	t_lp	*p1;
	t_lp	*p2;

	p1 = (t_lp *)a;
	p2 = (t_lp *)b;
	return ((int)p1->d - (int)p2->d);
}
#include <math.h>
static t_lp	*find_all(char **map, t_player *player, int (*test)(t_player *, char), size_t *count)
{
	t_lp			*ps;
	size_t			pcount;
	t_lp			p;

	ps = NULL;
	pcount = 0;
	p.y = 0;
	while (p.y < MAP_HEIGHT)
	{
		p.x = 0;
		while (p.x < MAP_WIDTH)
		{
			if (!(p.x == player->pos.x && p.y == player->pos.y) &&
				test(player, map[p.y][p.x]))
			{
				if (!(ps = realloc(ps, (pcount + 1) * sizeof(t_lp))))
				{
					perror("realloc");
					exit(EXIT_FAILURE);
				}
				p.team = map[p.y][p.x];

				int	x = (int)p.x - (int)player->pos.x;
				int	y = (int)p.y - (int)player->pos.y;
				p.d = (unsigned int)sqrt(x * x + y * y);
				ps[pcount] = p;
				pcount += 1;
			}
			p.x += 1;
		}
		p.y += 1;
	}
	qsort(ps, pcount, sizeof(t_lp), &lpcmp);
	*count = pcount;
	return (ps);
}

bool    get_target_coord(t_context *context, int *targetx, int *targety)
{
  int y = 0;
  int x = 0;
  char p;
  t_lp	*ally;
  size_t	acount;
  t_lp	*enemy;
  size_t	ecount;
  ally = find_all(context->map, &context->player, &isally, &acount);
  enemy = find_all(context->map, &context->player, &isenemy, &ecount);
  int g = 0;
  int w = 0;
 int  nbally = 0;
 int tmpnbally = 0;
 t_lp final;
 if (ecount == 0)
   return false;
 final.x = enemy[0].x;
 final.y = enemy[0].y;
 
  while (g < ecount)
  {
    w = 0;
    tmpnbally = 0;
    while (w < acount)
    {
      if ((enemy[g].x - 15) < ally[w].x && (enemy[g].x + 15) > ally[w].x && (enemy[g].y - 15) < ally[w].y && (enemy[g].y + 15) > ally[w].y )
      {
        tmpnbally++;
      }
      w++;
    }
    w = 0;
    while (w < ecount)
    {
      if ((enemy[g].x - 15) < enemy[w].x && (enemy[g].x + 15) > enemy[w].x && (enemy[g].y - 15) < enemy[w].y && (enemy[g].y + 15) > enemy[w].y )
      {
        tmpnbally--;
      }
      w++;
    }
    if (tmpnbally > nbally)
    {
      nbally = tmpnbally;
      final.x = enemy[g].x;
      final.y = enemy[g].y;
    }
    g++;
  }
  *targetx = (int)final.x;
  *targety = (int)final.y;
  return true;
}

bool    send_attck_msg(t_context *context)
{
  int targetx;
  int targety;
  int x = 0;
  int y = 0;
  char *msg;

  if (!get_target_coord(context, &targetx, &targety))
    return false;

  asprintf(&msg, "%d;%d", targetx, targety);

  struct timespec t;
  //clock_gettime(CLOCK_REALTIME, &t);
  t.tv_sec = 1;
  t.tv_nsec = 0;
  int ret = mq_timedsend(context->player.mq, msg, ft_strlen(msg), 0, &t);
  if (ret == -1)
    perror("mq_send");
  /*while (y < MAP_HEIGHT)
    {
    x = 0;
    while (x < MAP_WIDTH)
    {
    if (context->map[y][x] == context->player.id)
    {
    int ret = mq_send(context->player.mq, msg, ft_strlen(msg), 0);
    if (ret == -1)
    perror("mq_send");
    }
    x++;
    }
    y++;
    }*/
  return true;

}

void    takeoff_player(t_context *context)
{
  int x = 0;
  int y = 0;
  while (y < MAP_HEIGHT)
  {
    x = 0;
    while (x < MAP_WIDTH)
    {
      if (x == context->player.pos.x && y == context->player.pos.y)
        context->map[y][x] = MAP_EMPTYCASE;
      x++;
    }
    y++;
  }

}

void    place_player(t_context *context)
{
  int x = 0;
  int y = 0;
  while (y < MAP_HEIGHT)
  {
    x = 0;
    while (x < MAP_WIDTH)
    {
      if (x == context->player.pos.x && y == context->player.pos.y)
        context->map[y][x] = context->player.id;
      x++;
    }
    y++;
  }

}

void	iaduban(t_context *context)
{
  //  context->map[3][3] = 'E';
  if (!send_attck_msg(context))
    return;
  char msg_ptr[1024];
  struct timespec t;
  //clock_gettime(CLOCK_REALTIME, &t);
  t.tv_sec = 1;
  t.tv_nsec = 0;
  ssize_t ret;
  ret = mq_timedreceive(context->player.mq, msg_ptr, 1024, NULL, &t);
  if (ret == -1)
  {
    perror("mq_timedreceive");
  }
  else
  {
    msg_ptr[ret] = '\0';
  }
  char **tab;
  tab = ft_strsplit(msg_ptr, ';');
  int x_target;
  int y_target;
  x_target = ft_atoi(tab[0]);
  y_target = ft_atoi(tab[1]);
  takeoff_player(context);
   if (y_target > context->player.pos.y && context->map[context->player.pos.y + 1][context->player.pos.x] == MAP_EMPTYCASE)
  {
    context->player.pos.y += 1;
  }
   else if (x_target > context->player.pos.x && context->map[context->player.pos.y][context->player.pos.x + 1] == MAP_EMPTYCASE)
  {
    context->player.pos.x += 1;
  }
  else if (y_target < context->player.pos.y && context->map[context->player.pos.y - 1][context->player.pos.x] == MAP_EMPTYCASE)
  {
    context->player.pos.y -= 1;
  }
  else if (x_target < context->player.pos.x && context->map[context->player.pos.y][context->player.pos.x - 1] == MAP_EMPTYCASE)
  {
    context->player.pos.x -= 1;
  }
  else
    random_move(&context->player, context->map, &context->player.pos.x, &context->player.pos.y);


  place_player(context);


}
