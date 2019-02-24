#ifndef __CHELPERHEAP__
#define __CHELPERHEAP__

#include <stdbool.h>
#include <stdlib.h>

/* This is the Heap. */
typedef struct Heap *Heap;

#include "common.h"
#include "string.h"

/*Creates a new Heap*/
Heap HeapCreate(size_t capacity, bool reverse, free_fn_t free_fn, copy_fn_t copy_fn);

/*Frees the Heap*/
void HeapFree(Heap t);

/*CLears the Heap*/
void HeapClear(Heap t);

/*Gets the amount of elements in the Heap.*/
size_t HeapLength(Heap t);

#endif /* __CHELPERHEAP__ */
