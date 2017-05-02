#include "list.h"

int	list_empty(t_list *head)
{
	return (head->next == head);
}
