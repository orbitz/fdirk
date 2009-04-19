#include <stack.h>


bool stackCreate (Stack * stack)
{
	if (linkListCreate (&stack->list) == NULL)
		return false;

	return true;
}

void stackDestroy (Stack * stack)
{
	linkListDestroy (&stack->list);
}

bool stackPush (Stack * stack, const void *data, unsigned int size)
{
	return linkListAddHead (&stack->list, data, size);
}
int stackPop (Stack * stack, void *data)
{
	int     ret = linkListGetHead (&stack->list, data);
	linkListDelHead(&stack->list);
	return ret;
}

int stackGetNumElm (Stack * stack)
{
	return linkListGetNumNodes (&stack->list);
}

unsigned int stackGetTopSize (Stack * stack)
{
	return linkListGetSizeHead (&stack->list);
}
