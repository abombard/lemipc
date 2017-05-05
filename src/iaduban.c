#include "lemipc.h"
#include <stdbool.h>
#include <math.h>

void    err_and_exit(char *msg)
{
  perror(msg);
  exit(EXIT_FAILURE);
}

void get_distance(t_lp *p, char **map, t_player *player)
{
  p->team = map[p->y][p->x];
  int	x = (int)p->x - (int)player->pos.x;
  int	y = (int)p->y - (int)player->pos.y;
  p->d = (unsigned int)sqrt(x * x + y * y);

}

int    get_score(unsigned int g, unsigned int ecount, t_lp *enemy, t_context *context)
{
  unsigned int w = 0;
  int tmpnbally = 0;
  t_lp	*ally;
  size_t	acount;

  ally = find_all(context->map, &context->player, &isally, &acount);
  while (w < acount)
  {
    if ((enemy[g].x - 15) < ally[w].x && (enemy[g].x + 15) > ally[w].x && (enemy[g].y - 15) < ally[w].y && (enemy[g].y + 15) > ally[w].y )
      tmpnbally++;
    w++;
  }
  w = 0;
  while (w < ecount)
  {
    if ((enemy[g].x - 2) < enemy[w].x && (enemy[g].x + 2) > enemy[w].x && (enemy[g].y - 2) < enemy[w].y && (enemy[g].y + 2) > enemy[w].y )
      tmpnbally -= 5;
    w++;
  }
  return tmpnbally;
}

bool    get_target_coord(t_context *context, int *targetx, int *targety)
{
  unsigned int g = 0;
  t_lp	*enemy;
  size_t	ecount;
  int  nbally = -100;
  int tmpnbally = 0;

  enemy = find_all(context->map, &context->player, &isenemy, &ecount);
  if (ecount == 0)
    return false;
  *targetx = enemy[0].x;
  *targety = enemy[0].y;
  while (g < ecount)
  {
    tmpnbally = get_score(g, ecount, enemy, context);
    if (tmpnbally > nbally)
    {
      nbally = tmpnbally;
      *targetx = enemy[g].x;
      *targety = enemy[g].y;
    }
    g++;
  }
  return true;
}

bool    send_attck_msg(t_context *context)
{
  int targetx;
  int targety;
  char *msg;

  if (!get_target_coord(context, &targetx, &targety))
    return false;
  asprintf(&msg, "%d;%d", targetx, targety);
  mq_send(context->mqid, context->player.id, msg, ft_strlen(msg));
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

void    fill_coord(char *msg_ptr, int  *x_target, int *y_target)
{
  char **tab;
  tab = ft_strsplit(msg_ptr, ';');
  *x_target = ft_atoi(tab[0]);
  *y_target = ft_atoi(tab[1]);

}

void    move_x(t_context *context, int x_target)
{
  if (x_target > context->player.pos.x && context->map[context->player.pos.y][context->player.pos.x + 1] == MAP_EMPTYCASE)
    context->player.pos.x += 1;
  else if (x_target < context->player.pos.x && context->map[context->player.pos.y][context->player.pos.x - 1] == MAP_EMPTYCASE)
    context->player.pos.x -= 1;
  else
    moverand(&context->player, context->map, &context->player.pos.x, &context->player.pos.y);
}

void    move_y(t_context *context, int y_target)
{
  if (y_target > context->player.pos.y && context->map[context->player.pos.y + 1][context->player.pos.x] == MAP_EMPTYCASE)
    context->player.pos.y += 1;
  else if (y_target < context->player.pos.y && context->map[context->player.pos.y - 1][context->player.pos.x] == MAP_EMPTYCASE)
    context->player.pos.y -= 1;
  else
    moverand(&context->player, context->map, &context->player.pos.x, &context->player.pos.y);
}

void	iaduban(t_context *context)
{
  char msg_ptr[1024];
  ssize_t ret;
  int x_target;
  int y_target;
  if (!send_attck_msg(context))
    return;
  ret = mq_recv(context->mqid, context->player.id, msg_ptr, 1024);
  if (ret == 0)
  {
    perror("receive msg:");
    return ;
  }
  fill_coord(msg_ptr, &x_target, &y_target); 
  takeoff_player(context);
  if (abs(y_target - (int)context->player.pos.y) > abs(x_target - (int)context->player.pos.x))
    move_y(context, y_target);
  else
    move_x(context, x_target);
  place_player(context);
}
