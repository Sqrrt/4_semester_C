#ifndef LAB3_MEMFUNC_H
#define LAB3_MEMFUNC_H

#define BLOCK_SIZE 24


#include "lab3.h"
#include <pthread.h>

void *my_malloc(size_t size);
void my_free(void *p);
void *my_calloc(size_t number, size_t size);
void *my_realloc(void *p, size_t size);


#endif /*LAB3_MEMFUNC_H*/
