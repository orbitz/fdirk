#include <stdlib.h>
#include <stdbool.h>
#include <buffcontainer.h>

struct buffcontainernode_t
{
	char   *data;
	struct buffcontainernode_t *next;
};

struct buffcontainer_t
{
	unsigned int num;
	struct buffcontainernode_t *bc;
	struct buffcontainernode_t *last;
};

static struct buffcontainer_t head;

/*
 * Static methods
 */
static bool appendBuff (void *buf);
static struct buffcontainernode_t *findNode (void *buf);

void   *bc_malloc (size_t size)
{
	void   *data = malloc (size);

	if (!data)
		return NULL;

	if (!appendBuff (data))
	{
		free (data);
		return NULL;
	}

	head.num++;
	return data;
}

void   *bc_realloc (void *buf, size_t size)
{
	struct buffcontainernode_t *currnode;

	if (!buf)
		return bc_malloc (size);
	else
	{
		currnode = findNode (buf);
		if (currnode)
		{
			void   *temp = realloc (currnode->data, size);

			if (temp)
				currnode->data = temp;
			else
				return NULL;
		}
		else
			return NULL;
	}

	return currnode->data;
}

void bc_free (void *buf)
{
	struct buffcontainernode_t *currnode;

	if (head.bc && head.bc->data == buf)
	{
		if (head.last == head.bc)
			head.last = NULL;
		free (head.bc->data);
		currnode = head.bc;
		head.bc = currnode->next;
		free (currnode);
		head.num--;
	}
	else if (head.bc && head.bc->next)
	{
		currnode = head.bc;
		while (currnode->next)
		{
			if (currnode->next->data == buf)
			{
				struct buffcontainernode_t *node = currnode->next;
				currnode->next = currnode->next->next;
				free (node->data);
				free (node);
				head.num--;
				break;
			}
			else
				currnode = currnode->next;
		}
	}
}

void bc_free_all (void)
{
	struct buffcontainernode_t *currnode;
	struct buffcontainernode_t *prevnode = head.bc;

	if (head.bc)
		do
		{
			currnode = prevnode->next;
			free (prevnode->data);
			free (prevnode);
			prevnode = currnode;
		}
		while (currnode);

	head.bc = head.last = NULL;

	head.num = 0;

}

unsigned int bc_get_num_allocs (void)
{
	return head.num;
}

static bool appendBuff (void *buf)
{
	struct buffcontainernode_t *currnode;

	currnode = malloc (sizeof (struct buffcontainernode_t));

	if (!currnode)								/* malloc failed */
		return false;

	currnode->next = NULL;
	if (head.last)
	{
		head.last->next = currnode;
		head.last = head.last->next;
	}
	else
		head.bc = head.last = currnode;

	head.last->data = buf;

	return true;
}


static struct buffcontainernode_t *findNode (void *buf)
{
	struct buffcontainernode_t *currnode;

	if (head.last && head.last->data == buf)
		return head.last;
	else
	{
		currnode = head.bc;
		while (currnode)
		{
			if (currnode->data == buf)
				return currnode;
			currnode = currnode->next;
		}
	}

	return NULL;
}
