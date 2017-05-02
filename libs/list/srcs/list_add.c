/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_add.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 15:33:55 by abombard          #+#    #+#             */
/*   Updated: 2016/03/31 15:35:15 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_list.h"

void	internal_list_add(t_list *new,
							t_list *prev,
							t_list *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

void	list_add(t_list *new, t_list *head)
{
	internal_list_add(new, head, head->next);
}

void	list_add_tail(t_list *new, t_list *head)
{
	internal_list_add(new, head->prev, head);
}
