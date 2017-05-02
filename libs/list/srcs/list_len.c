/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_len.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/31 15:23:51 by abombard          #+#    #+#             */
/*   Updated: 2016/04/01 18:38:57 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_list.h"

size_t		list_len(t_list *head)
{
	t_list	*pos;
	int		ret;

	ret = 0;
	pos = head;
	while ((pos = pos->next) && pos != head)
	{
		ret += 1;
	}
	return (ret);
}
