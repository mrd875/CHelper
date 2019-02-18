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

/*
    Concats the second list to the first. Returns the concat'd list, make sure to free afterwards!
*/
ArrayList ArrayListConcat(ArrayList l, ArrayList l2);

/*
    Returns a sub list of l. All elements are copied! Be sure to free afterwards.
*/
ArrayList ArrayListGetSubArrayList(ArrayList l, size_t i, size_t size);

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

/* Sets the cap of the list, can't go below the current length though.*/
bool ArrayListSetCapacity(ArrayList l, size_t size);

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

/* Removes the last element from the end of the list. */
bool ArrayListRemove(ArrayList l);

/* Gets the last element in the list. */
void *ArrayListGet(ArrayList l);

/*
    Searches the Arraylist given a compare function and compare argument, will return the index of the found element, -1 if not found.
*/
int ArrayListIndexOf(ArrayList l, compare_fn_t compare_fn, void *compare_arg);

/*
    Searches the arraylist given a compare function and compare argument, will return the amount of times the element was found.
*/
size_t ArrayListCount(ArrayList l, compare_fn_t compare_fn, void *compare_arg);

/*
    Searches the arraylist given a compare function and compare argument, will return if the array has the element.
*/
bool ArrayListHas(ArrayList l, compare_fn_t compare_fn, void *compare_arg);

/*
    Does the function on every element of the List.
*/
void ArrayListForEach(ArrayList l, foreach_fn_t foreach_fn);

/*Removes all the elements from the list that fit the compare.*/
bool ArrayListRemoveIf(ArrayList l, compare_fn_t compare_fn, void *compare_arg);

/*Adds all the copied elements from l2 to the end of l1.*/
bool ArrayListAddAll(ArrayList l, ArrayList l2);

/*Returns a list of all the elements filtered, copied. Free afterwards.*/
ArrayList ArrayListFilter(ArrayList l, compare_fn_t compare_fn, void *compare_arg);

/*Removes all elements from i to size -1*/
bool ArrayListRemoveRange(ArrayList l, size_t i, size_t size);

/*trims the array list.*/
bool ArrayListTrim(ArrayList l, size_t i, size_t size);

/* Returns a new string of the list parsed as ints. Free the string afterwards. */
String ArrayListToStringInt(ArrayList l);

/*Prints the list as ints.*/
void ArrayListPrintInt(ArrayList l);

/* Returns a new string of the list parsed as strings. Free the string afterwards. */
String ArrayListToStringString(ArrayList l);

/*Prints the list as strings.*/
void ArrayListPrintString(ArrayList l);

#endif /* __CHELPERDICT__ */
