#ifndef SEM_H
# define SEM_H

union u_semun
{
	int					val;
	struct semid_ds		*buf;
	unsigned short int	*array;
	struct seminfo		*__buf;
};

#endif
