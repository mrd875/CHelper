#ifndef __CHELPARRAYLIST__
#define __CHELPARRAYLIST__

#include <stdbool.h>
#include <stdlib.h>

#include "common.h"

typedef struct ArrayList *ArrayList;

ArrayList ArrayListCreate(size_t capacity, free_fn_t free_fn, copy_fn_t copy_fn);

ArrayList ArrayListCopy(ArrayList l);

void ArrayListClear(ArrayList l);

void ArrayListFree(ArrayList l);

size_t ArrayListLength(ArrayList l);

size_t ArrayListCapacity(ArrayList l);

bool ArrayListIsFull(ArrayList l);

bool ArrayListIsEmpty(ArrayList l);

bool ArrayListSetX(ArrayList l, void *a, size_t x);

void *ArrayListGetX(ArrayList l, size_t x);

bool ArrayListAddX(ArrayList l, void *a, size_t x);

bool ArrayListRemoveX(ArrayList l, size_t x);

bool ArrayListAddEnd(ArrayList l, void *a);

#endif /* __CHELPERDICT__ */
