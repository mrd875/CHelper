#ifndef __CHELPLINKEDLIST__
#define __CHELPLINKEDLIST__

#include <stdbool.h>
#include <stdlib.h>

/* This is the array list struct. */
typedef struct LinkedList *LinkedList;

#include "common.h"
#include "string.h"

/* Creates and returns an array list. Free after. */
LinkedList LinkedListCreate(size_t capacity, free_fn_t free_fn, copy_fn_t copy_fn);

/* Removes and frees all the elements in the list. */
void LinkedListClear(LinkedList l);

/* Frees all data in the list and the list. */
void LinkedListFree(LinkedList l);

/* Returns the number of elements in the list. */
size_t LinkedListLength(LinkedList l);

/* Returns the capacity of the list. */
size_t LinkedListCapacity(LinkedList l);

/* Returns if the list is full or not. */
bool LinkedListIsFull(LinkedList l);

/* Returns if the list is empty or not. */
bool LinkedListIsEmpty(LinkedList l);

/* Sets the cap of the list, can't go below the current length though.*/
bool LinkedListSetCapacity(LinkedList l, size_t size);

/* Sets the data at the index, frees the old data. */
bool LinkedListSetX(LinkedList l, void *a, size_t x);

/* Returns the data at the index. */
void *LinkedListGetX(LinkedList l, size_t x);

/* Adds data at the index, pushes everything else down. */
bool LinkedListAddX(LinkedList l, void *a, size_t x);

/* Removes and frees the data at the index. */
bool LinkedListRemoveX(LinkedList l, size_t x);

/* Adds the element to the end of the list. */
bool LinkedListAdd(LinkedList l, void *a);

/* Removes the last element from the end of the list. */
bool LinkedListRemove(LinkedList l);

/* Gets the last element in the list. */
void *LinkedListGet(LinkedList l);

/*Sets the data at the end of the list*/
bool LinkedListSet(LinkedList l, void *data);

/* Copies the array list. */
LinkedList LinkedListCopy(LinkedList l);

/*
    Searches the LinkedList given a compare function and compare argument, will return the index of the found element, -1 if not found.
*/
int LinkedListIndexOf(LinkedList l, compare_fn_t compare_fn, void *compare_arg);

/*
    Searches the LinkedList given a compare function and compare argument, will return the amount of times the element was found.
*/
size_t LinkedListCount(LinkedList l, compare_fn_t compare_fn, void *compare_arg);

/*
    Searches the LinkedList given a compare function and compare argument, will return if the array has the element.
*/
bool LinkedListContains(LinkedList l, compare_fn_t compare_fn, void *compare_arg);

/*
    Does the function on every element of the List.
*/
void LinkedListForEach(LinkedList l, foreach_fn_t foreach_fn);

/*Removes all the elements from the list that fit the compare.*/
bool LinkedListRemoveIf(LinkedList l, compare_fn_t compare_fn, void *compare_arg);

/*Adds all the copied elements from l2 to the end of l1.*/
bool LinkedListAddAll(LinkedList l, LinkedList l2);

/*Returns a list of all the elements filtered, copied. Free afterwards.*/
LinkedList LinkedListFilter(LinkedList l, compare_fn_t compare_fn, void *compare_arg);

/*
    Concats the second list to the first. Returns the concat'd list, make sure to free afterwards!
*/
LinkedList LinkedListConcat(LinkedList l, LinkedList l2);

/*
    Returns a sub list of l. All elements are copied! Be sure to free afterwards.
*/
LinkedList LinkedListGetSubLinkedList(LinkedList l, size_t i, size_t size);

/*Removes all elements from i to size -1*/
bool LinkedListRemoveRange(LinkedList l, size_t i, size_t size);

/*trims the array list.*/
bool LinkedListTrim(LinkedList l, size_t i, size_t size);

/* Returns a new string of the list parsed as ints. Free the string afterwards. */
String LinkedListToStringInt(LinkedList l);

/*Prints the list as ints.*/
void LinkedListPrintInt(LinkedList l);

/* Returns a new string of the list parsed as strings. Free the string afterwards. */
String LinkedListToStringString(LinkedList l);

/*Prints the list as strings.*/
void LinkedListPrintString(LinkedList l);

/*
    LinkedList cursor goes to just before the head in the LinkedList. Returns if successful.
*/
bool LinkedListCursorBeforeHead(LinkedList l);

/*
    LinkedList cursor goes to just after the tail in the LinkedList. Returns if successful.
*/
bool LinkedListCursorAfterTail(LinkedList l);

/*
    LinkedList cursor goes to next thing in the LinkedList. Returns if successful.
*/
bool LinkedListCursorNext(LinkedList l);

/*
    LinkedList iterator goes to next thing in the LinkedList. Returns if successful.
*/
bool LinkedListCursorPrevious(LinkedList l);

/*
    Returns if the iterator is on a null spot of the LinkedList (edges).
*/
bool LinkedListCursorIsNull(LinkedList l);

/*
    LinkedList iterator goes to next element that is true for the search. Returns if it was successful.
*/
bool LinkedListCursorSearchNext(LinkedList l, compare_fn_t compare_fn, void *compare_arg);

/*
    LinkedList iterator goes to before element that is true for the search. Returns if it was successful.
*/
bool LinkedListCursorSearchPrevious(LinkedList l, compare_fn_t compare_fn, void *compare_arg);

/*
    Returns if the LinkedList iterator has next.
*/
bool LinkedListCursorHasNext(LinkedList l);

/*
    Returns if the LinkedList iterator has before.
*/
bool LinkedListCursorHasPrevious(LinkedList l);

/*
    Returns the data at the LinkedListCursor.
*/
void *LinkedListCursorGet(LinkedList l);

/*
    Changes the LinkedListCursor's data. Returns if successful.
*/
bool LinkedListCursorSet(LinkedList l, void *data);

/*
    Adds item to LinkedList before the LinkedListCursor. Cursor remains on the current element. Returns if the LinkedList iterator was successful.
*/
bool LinkedListCursorAdd(LinkedList l, void *data);

/*
    Deletes the entry at LinkedListCursor and goes before. Returns if successful.
*/
bool LinkedListCursorRemove(LinkedList l);

/*
    Returns the index of the current position of the cursor.
*/
int LinkedListCursorIndexOf(LinkedList l);

/*Free list*/
void FreeLinkedList(void *a);

/*Copy list*/
void *CopyLinkedList(void *a);

#endif /* __CHELPERDICT__ */
