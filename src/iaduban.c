#define GNU_SOURCE
#include "lemipc.h"

#include <stdbool.h>
bool    get_target_coord(t_context *context, int *targetx, int *targety)
{
  fprintf(stderr, "GETTING TARGET COORD\n");
  int y = 0;
  int x = 0;
  char p;
  while (y < MAP_HEIGHT)
  {
    x = 0;
    while (x < MAP_WIDTH)
    {
      p = context->map[y][x];
      if (p != context->player.id && p != MAP_BORDERWIDTH && p != MAP_BORDERHEIGHT && p != MAP_EMPTYCASE)
      {
        *targetx = x;
        *targety = y;
        return true;
      }
      x++;
    }
    y++;
  }
  return false;
}

bool    send_attck_msg(t_context *context)
{
  int targetx;
  int targety;
  int x = 0;
  int y = 0;
  char *msg;

  fprintf(stderr, "CHEEEEEEEEEECK\n");
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
  fprintf(stderr, "LOOP\n");
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
    printf("received a msg: %s", msg_ptr);
  }
  char **tab;
  tab = ft_strsplit(msg_ptr, ';');
  int x_target;
  int y_target;
  x_target = ft_atoi(tab[0]);
  y_target = ft_atoi(tab[1]);
  takeoff_player(context);
  if (x_target > context->player.pos.x && context->map[context->player.pos.y][context->player.pos.x + 1] == MAP_EMPTYCASE)
  {
    fprintf(stderr, "MOVING\n");
    context->player.pos.x += 1;
  }
  else if (y_target > context->player.pos.y && context->map[context->player.pos.y + 1][context->player.pos.x] == MAP_EMPTYCASE)
  {
    fprintf(stderr, "MOVING\n");
    context->player.pos.y += 1;
  }
  else if (x_target < context->player.pos.x && context->map[context->player.pos.y][context->player.pos.x - 1] == MAP_EMPTYCASE)
  {
    fprintf(stderr, "MOVING\n");
    context->player.pos.x -= 1;
  }
  else if (y_target < context->player.pos.y && context->map[context->player.pos.y - 1][context->player.pos.x] == MAP_EMPTYCASE)
  {
    fprintf(stderr, "MOVING\n");
    context->player.pos.y -= 1;
  }
  else
    random_move(&context->player, context->map, &context->player.pos.x, &context->player.pos.y);

  
  place_player(context);
  fprintf(stderr, "END LOOP\n");


}
