#ifndef BUFFCONTAINER_H__
#define BUFFCONTAINER_H__

#include <stdlib.h>

void   *bc_malloc(size_t size);
void   *bc_realloc(void *buf, size_t size);
void    bc_free(void *buf);
void    bc_free_all(void);
unsigned int bc_get_num_allocs(void);

#endif													/* BUFFCONTAINER_H__ */
