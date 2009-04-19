#ifndef _LINKLIST_H_
#define _LINKLIST_H_

#include <stdio.h>
#include <stdbool.h>

struct node
{
	void   *data;
	unsigned int size;
	struct node *prev;
	struct node *next;
};

struct linklist_t
{
	int     numnodes;
	struct node *head;
	struct node *tail;
};

struct linklist_t *linkListCreate (struct linklist_t *list);
void    linkListDestroy (struct linklist_t *list);
unsigned int linkListGetNumNodes (const struct linklist_t *list);
bool linkListAddIdx (struct linklist_t *list, unsigned int indx,
												const void *data, unsigned int size);
bool linkListAddHead (struct linklist_t *list, const void *data,
												 unsigned int size);
bool linkListAddTail (struct linklist_t *list, const void *data,
												 unsigned int size);
bool linkListDelIdx (struct linklist_t *list, unsigned int indx);
bool linkListDelHead (struct linklist_t *list);
bool linkListDelTail (struct linklist_t *list);
int     linkListGetIdx (const struct linklist_t *list, unsigned int indx,
												void *data);
int     linkListGetHead (const struct linklist_t *list, void *data);
int     linkListGetTail (const struct linklist_t *list, void *data);
int     linkListGetSizeIdx (const struct linklist_t *list,
														unsigned int indx);
int     linkListGetSizeHead (const struct linklist_t *list);
int     linkListGetSizeTail (const struct linklist_t *list);
#ifdef DEBUG
void    printLinkList (struct linklist_t *list);
#endif /* DEBUG */

#endif /* _LINKLIST_H_ */
