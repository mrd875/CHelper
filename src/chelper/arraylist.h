#ifndef __CHELPARRAYLIST__
#define __CHELPARRAYLIST__

#include <stdbool.h>
#include <stdlib.h>

/* This is the array list struct. */
typedef struct ArrayList *ArrayList;

#include "common.h"
#include "string.h"

/* Creates and returns an array list. Free after. */
ArrayList ArrayListCreate(size_t capacity, free_fn_t free_fn, copy_fn_t copy_fn);

/* Copies the array list. */
ArrayList ArrayListCopy(ArrayList l);

/* Removes and frees all the elements in the list. */
void ArrayListClear(ArrayList l);

/* Frees all data in the list and the list. */
void ArrayListFree(ArrayList l);

/* Returns the number of elements in the list. */
size_t ArrayListLength(ArrayList l);

/* Returns the capacity of the list. */
size_t ArrayListCapacity(ArrayList l);

/* Returns if the list is full or not. */
bool ArrayListIsFull(ArrayList l);

/* Returns if the list is empty or not. */
bool ArrayListIsEmpty(ArrayList l);

/* Sets the data at the index, frees the old data. */
bool ArrayListSetX(ArrayList l, void *a, size_t x);

/* Returns the data at the index. */
void *ArrayListGetX(ArrayList l, size_t x);

/* Adds data at the index, pushes everything else down. */
bool ArrayListAddX(ArrayList l, void *a, size_t x);

/* Removes and frees the data at the index. */
bool ArrayListRemoveX(ArrayList l, size_t x);

/* Adds the element to the end of the list. */
bool ArrayListAdd(ArrayList l, void *a);

/* Returns a new string of the list parsed as ints. Free the string afterwards. */
String ArrayListToStringInt(ArrayList l);

/*Prints the list as ints.*/
void ArrayListPrintInt(ArrayList l);

#endif /* __CHELPERDICT__ */
