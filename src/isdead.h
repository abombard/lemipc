/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isdead.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/05 17:53:04 by abombard          #+#    #+#             */
/*   Updated: 2017/05/05 17:53:06 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
