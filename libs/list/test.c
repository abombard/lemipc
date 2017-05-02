#include "list.h"

#include <stdio.h>

typedef struct {
	int						value;
	t_list				list;
} integer_list_t;


static void			list_display (t_list				*list)
{
		t_list					*tmp;
		integer_list_t			*integer;

		printf ("list:\n");
		LIST_FOREACH (list, tmp)
		{
				integer = CONTAINER_OF (tmp, integer_list_t, list);
				printf ("%d", integer->value);
		}
		printf ("\n");
}

/* list_nth */

/* list slice */

#define NODE_COUNT			42

int main()
{
		t_list			list;
		int					list_len;
		integer_list_t	  	nodes[ NODE_COUNT ];
		unsigned int 		i;
		t_list			new;

		INIT_LIST_HEAD (&list);

		list_len = 0;
		for (i = 0; i < sizeof (nodes) / sizeof (nodes[ 0 ]); i ++)
		{
				INIT_LIST_HEAD (&nodes[ i ].list);
				nodes[ i ].value = i;

				//	printf ("add tail %d\n", i);

				list_add_tail (&nodes[ i ].list, &list);
				list_len ++;
		}

		list_display (&list);

		list_slice (&new, &list, 0, 4);
		list_display (&new);

		return 0;
}
