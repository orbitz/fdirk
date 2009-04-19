#include <stdlib.h>
#include <string.h>
#include <linklist.h>

/*
Func: findLeftIdx (const struct linklist_t *list,
	unsigned int indx)
Desc: Finds indx - 1 from head
*/
static struct node *findLeftIdx (const struct linklist_t *list,
																 unsigned int indx)
{
	struct node *idx;
	unsigned int     c;

	idx = list->head;

	for (c = 0; c < indx; c++)
		idx = idx->next;

	return idx;
}

/*
Func: findRightIdx (const struct linklist_t *list,
	unsigned int indx)
Desc: Finds indx - 1 from tail
*/
static struct node *findRightIdx (const struct linklist_t *list,
																	unsigned int indx)
{
	struct node *idx;
	unsigned int     c;
	int     end = linkListGetNumNodes (list) - 1;

	idx = list->tail;
	indx++;
	for (c = 0; (end - c) > indx; c++)
		idx = idx->prev;

	idx = idx->prev;
	return idx;
}

/*
Func: addData (struct node *indx, const void *data,
	unsigned int size)
Desc: Adds data to place after indx
*/
static bool addData (struct node *indx, const void *data,
												unsigned int size)
{
	struct node *newnode = malloc (sizeof (struct node));

	if (newnode == NULL)
	{
		/*
		   Error 
		 */
		return false;
	}

	newnode->size = size;
	newnode->data = malloc (size);
	if (newnode->data == NULL)
	{
		/*
		   Error 
		 */
		free (newnode);
		return false;
	}

	memcpy (newnode->data, data, size);

	indx = indx->prev;
	newnode->next = indx->next;
	indx->next->prev = newnode;
	indx->next = newnode;
	newnode->prev = indx;

	return true;
}

/*
Func: addLeftSide (struct linklist_t *list, unsigned int indx,
	const void *data, unsigned int size)
Desc: Add the data coming from the head
*/
static bool addLeftSide (struct linklist_t *list, unsigned int indx,
														const void *data, unsigned int size)
{
	struct node *lidx = findLeftIdx (list, indx);
	return addData (lidx, data, size);
}

/*
Func: addRightSide (struct linklist_t *list, unsigned int indx,
	const void *data, unsigned int size)
Desc: Add the data coming from the tail
*/
static bool addRightSide (struct linklist_t *list, unsigned int indx,
														 const void *data, unsigned int size)
{
	struct node *lidx = findRightIdx (list, indx);
	return addData (lidx, data, size);
}

struct linklist_t *linkListCreate (struct linklist_t *list)
{
	if (list == NULL)
	{
		list = malloc (sizeof (struct linklist_t));
		if (list == NULL)
		{
			/*
			   Error 
			 */
			return NULL;
		}
	}

	memset (list, 0, sizeof (struct linklist_t));

	return list;
}

void linkListDestroy (struct linklist_t *list)
{
	struct node *prevnode, *currnode;

	if (linkListGetNumNodes (list) > 0)
	{
		currnode = list->head;
		do
		{
			prevnode = currnode;
			currnode = currnode->next;
			free (prevnode->data);
			free (prevnode);
		}
		while (currnode != NULL);
	}
}

unsigned int linkListGetNumNodes (const struct linklist_t *list)
{
	return list->numnodes;
}

bool linkListAddIdx (struct linklist_t * list, unsigned int indx,
												const void *data, unsigned int size)
{
	bool ret;
	if (indx > linkListGetNumNodes (list))
	{
		/*
		   Index out of bounds 
		 */
		return false;
	}

	if (indx == 0)
		return linkListAddHead (list, data, size);
	else if (indx == linkListGetNumNodes (list))
		return linkListAddTail (list, data, size);
	else
	{
		if (indx <= linkListGetNumNodes (list) / 2)
			ret = addLeftSide (list, indx, data, size);
		else
			ret = addRightSide (list, indx, data, size);

		if (ret == true)
			list->numnodes++;
		return ret;
	}

	return false;
}

bool linkListAddHead (struct linklist_t * list, const void *data,
												 unsigned int size)
{
	struct node *newnode = malloc (sizeof (struct node));

	if (newnode == NULL)
	{
		/*
		   Error 
		 */
		return false;
	}

	memset (newnode, 0, sizeof (struct node));
	newnode->size = size;
	newnode->data = malloc (size);
	if (newnode->data == NULL)
	{
		/*
		   Error 
		 */
		free (newnode);
		return false;
	}
	memcpy (newnode->data, data, size);

	newnode->next = list->head;
	if (newnode->next != NULL)
		newnode->next->prev = newnode;
	if (list->tail == NULL)
		list->tail = newnode;

	newnode->prev = NULL;
	list->head = newnode;
	list->numnodes++;
	return true;
}

bool linkListAddTail (struct linklist_t * list, const void *data,
												 unsigned int size)
{
	struct node *newnode = malloc (sizeof (struct node));

	if (newnode == NULL)
	{
		/*
		   Error 
		 */
		return false;
	}

	newnode->size = size;
	newnode->data = malloc (size);
	if (newnode->data == NULL)
	{
		/*
		   Error
		 */
		free (newnode);
		return false;
	}
	memcpy (newnode->data, data, size);
	newnode->prev = list->tail;
	if (list->tail != NULL)
		list->tail->next = newnode;
	newnode->next = NULL;
	list->tail = newnode;
	if (list->head == NULL)
		list->head = list->tail;
	list->numnodes++;
	return true;
}

bool linkListDelIdx (struct linklist_t * list, unsigned int indx)
{
	struct node *currnode;

	if (indx == 0)
		return linkListDelHead (list);
	else if (indx == linkListGetNumNodes (list) - 1)
		return linkListDelTail (list);
	else if (indx < linkListGetNumNodes (list))
	{
		if (indx < linkListGetNumNodes (list) / 2)
			currnode = findLeftIdx (list, indx);
		else
			currnode = findRightIdx (list, indx);

		currnode->prev->next = currnode->next;
		currnode->next->prev = currnode->prev;
		free (currnode->data);
		free (currnode);
		list->numnodes--;
		return true;
	}
	else
		return false;
}

bool linkListDelHead (struct linklist_t * list)
{
	struct node *currnode;

	if (list->head == NULL)
	{
		/*
		   Error, empty list 
		 */
		return false;
	}

	currnode = list->head;
	list->head = currnode->next;
	free (currnode->data);
	free (currnode);
	if (list->head == NULL)
		list->tail = NULL;
	list->numnodes--;
	return true;
}

bool linkListDelTail (struct linklist_t * list)
{
	struct node *currnode;

	if (list->tail == NULL)
	{
		/*
		   Error, empty list 
		 */
		return false;
	}

	currnode = list->tail;
	list->tail = currnode->prev;
	list->tail->next = NULL;
	free (currnode->data);
	free (currnode);
	list->numnodes--;
	return true;
}

int linkListGetIdx (const struct linklist_t *list, unsigned int indx,
										void *data)
{
	struct node *currnode;

	if (indx == 0)
		return linkListGetHead (list, data);
	else if (indx == linkListGetNumNodes (list) - 1)
		return linkListGetTail (list, data);
	else if (indx < linkListGetNumNodes (list))
	{
		if (indx < linkListGetNumNodes (list) / 2)
			currnode = findLeftIdx (list, indx);
		else
			currnode = findRightIdx (list, indx);

		memcpy (data, currnode->data, currnode->size);
		return currnode->size;
	}
	else
		return 0;
}

int linkListGetHead (const struct linklist_t *list, void *data)
{
	if (list->head != NULL)
	{
		memcpy (data, list->head->data, list->head->size);
		return list->head->size;
	}
	else
		return 0;
}

int linkListGetTail (const struct linklist_t *list, void *data)
{

	if (list->tail != NULL)
	{

		memcpy (data, list->tail->data, list->tail->size);
		return list->tail->size;
	}
	else
		return 0;
}

int linkListGetSizeIdx (const struct linklist_t *list, unsigned int indx)
{
	struct node *currnode;

	if (indx == 0)
		return linkListGetSizeHead (list);
	else if (indx == linkListGetNumNodes (list) - 1)
		return linkListGetSizeTail (list);
	else if (indx < linkListGetNumNodes (list))
	{
		if (indx < linkListGetNumNodes (list) / 2)
			currnode = findLeftIdx (list, indx);
		else
			currnode = findRightIdx (list, indx);

		return currnode->size;
	}

	return 0;
}

int linkListGetSizeHead (const struct linklist_t *list)
{
	if (list->head != NULL)
		return list->head->size;
	else
		return 0;
}

int linkListGetSizeTail (const struct linklist_t *list)
{
	if (list->tail != NULL)
		return list->tail->size;
	else
		return 0;
}

#ifdef DEBUG
void printLinkList (struct linklist_t *list)
{
	struct node *currnode = list->head;
	int     indx = 0;

	while (currnode != NULL)
	{
		printf ("NODE: %d\n  - Size: %d\n|\n|\n|", indx, currnode->size);
		indx++;
		currnode = currnode->next;
	}
}
#endif /* DEBUG */
