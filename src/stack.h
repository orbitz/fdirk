#ifndef STACK_H__
#define STACK_H__

#include <stdbool.h>
#include <linklist.h>

struct stack_t
{
	struct linklist_t list;
};

typedef struct stack_t Stack;

bool    stackCreate (Stack * stack);
void    stackDestroy (Stack * stack);
bool    stackPush (Stack * stack, const void *data, unsigned int size);
int     stackPop (Stack * stack, void *data);

int     stackGetNumElm (Stack * stack);
unsigned int     stackGetTopSize (Stack * stack);

#endif /* STACK_H__ */
