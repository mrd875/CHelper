#ifndef __CHELPERHEAP__
#define __CHELPERHEAP__

#include <stdbool.h>
#include <stdlib.h>

/* This is the Heap. */
typedef struct Heap *Heap;

#include "common.h"
#include "arraylist.h"
#include "string.h"

/*Creates a new Heap*/
Heap HeapCreate(bool reverse, free_fn_t free_fn, copy_fn_t copy_fn);

/* Copies the heap. */
Heap HeapCopy(Heap h);

/*Frees the Heap*/
void HeapFree(Heap h);

/*CLears the Heap*/
void HeapClear(Heap h);

/*Gets the amount of elements in the Heap.*/
size_t HeapLength(Heap h);

/*Gets the item in the heap*/
void *HeapGet(Heap h);

/*Removes the item from the heap*/
bool HeapRemove(Heap h);

/*Updates the key*/
bool HeapChangeKey(Heap h, int key, int newK);

/*Adds the item to the heap*/
bool HeapAdd(Heap h, int key, void *data);

/*Returns an array of all the elements heap sorted copied.*/
ArrayList HeapToArrayList(Heap h);

#endif /* __CHELPERHEAP__ */
