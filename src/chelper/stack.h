#ifndef __CHELPERSTACK__
#define __CHELPERSTACK__

#include <stdbool.h>
#include <stdlib.h>

/* This is the Stack. */
typedef struct Stack *Stack;

#include "common.h"
#include "arraylist.h"
#include "string.h"

/*Creates a new Stack*/
Stack StackCreate(free_fn_t free_fn, copy_fn_t copy_fn);

/* Copies the Stack. */
Stack StackCopy(Stack h);

/*Frees the Stack*/
void StackFree(Stack h);

/*CLears the Stack*/
void StackClear(Stack h);

/*Gets the amount of elements in the Stack.*/
size_t StackLength(Stack h);

/*Gets the item in the Stack*/
void *StackGet(Stack h);

/*Removes the item from the Stack*/
bool StackRemove(Stack h);

/*Adds the item to the Stack*/
bool StackAdd(Stack h, void *data);

/*Removes from Stack, no free*/
bool StackRemoveNoFree(Stack h);

#endif /* __CHELPERSTACK__ */
