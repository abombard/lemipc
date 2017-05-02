/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_list.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/19 14:52:15 by abombard          #+#    #+#             */
/*   Updated: 2016/04/19 14:52:16 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_LIST_H
# define INTERNAL_LIST_H

# include "../list.h"

void			internal_list_add (t_list *new,
									t_list *prev,
									t_list *next);

void			internal_list_del(t_list *prev, t_list *next);
void			list_del_init(t_list *entry);

void			internal_list_splice(t_list *list, t_list *head);
void			list_splice_init(t_list *list, t_list *head);

#endif
