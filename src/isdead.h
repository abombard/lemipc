#ifndef ISDEAD_H
# define ISDEAD_H

typedef struct	s_enemy
{
	char	team;
	int		count;
}				t_enemy;

typedef struct	s_enemies
{
	t_enemy	*arr;
	size_t	count;
}				t_enemies;

#endif
