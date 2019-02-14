#ifndef __CHELPLinkedList__
#define __CHELPLinkedList__

#include <stdbool.h>
#include "common.h"

/*
    Returns a newly created LinkedList. For unlimited size, set to -1. Be sure to free the LinkedList after.
*/
LinkedList LinkedListCreate(int size);

/*
    Frees the LinkedList, will also free all data inside the LinkedList for the caller.
*/
void LinkedListFree(LinkedList l);

/*
    Empties the LinkedList. All data inside is free'd.
*/
void LinkedListClear(LinkedList l);

/*
    Returns how many elements are in the LinkedList.
*/
int LinkedListSize(LinkedList l);

/*
    Returns the capacity of the LinkedList.
*/
int LinkedListCapacity(LinkedList l);

/*
    Returns if the LinkedList is full.
*/
bool LinkedListFull(LinkedList l);

/*
    Returns if the LinkedList is empty.
*/
bool LinkedListEmpty(LinkedList l);

/*
    Returns the address of the first element in the LinkedList, will return NULL if there isn't one.
*/
void *LinkedListGetFirst(LinkedList l);

/*
    Returns the address of the last element in the LinkedList, will return NULL if there isn't one.
*/
void *LinkedListGetLast(LinkedList l);

/*
    Returns the address of the x'th element in the LinkedList, will return NULL if there isn't one.
*/
void *LinkedListGetX(LinkedList l, int x);

/*
    Adds the given data to the start of the LinkedList, returns if it was successful. free and copy are pointers to functions that know how to free and copy the data.
*/
bool LinkedListAddFirst(LinkedList l, void *data, DataArgs dataArgs);

/*
    Adds the given data to the end of the LinkedList, returns if it was successful.
*/
bool LinkedListAddLast(LinkedList l, void *data, DataArgs dataArgs);

/*
    Adds the given data at the x'th spot of the LinkedList (pushes everything else after futher down the LinkedList), returns if it was successful.
*/
bool LinkedListAddX(LinkedList l, void *data, DataArgs dataArgs, int x);

/*
    Changes the data at the x'th spot in the LinkedList, will free the old data. Returns if it was successful.
*/
bool LinkedListChangeX(LinkedList l, void *data, DataArgs dataArgs, int x);

/*
    Deletes and frees the data found at the end of the LinkedList. Returns if successful.
*/
bool LinkedListDeleteLast(LinkedList l);

/*
    Deletes and frees the data found at the start of the LinkedList. Returns if successful.
*/
bool LinkedListDeleteFirst(LinkedList l);

/*
    Deletes and frees the data found at the x'th spot of the LinkedList. Returns if successful.
*/
bool LinkedListDeleteX(LinkedList l, int x);

/*
    Searches the LinkedList given a compare function and compare argument, will return the index of the found element, -1 if not found.
*/
int LinkedListSearch(LinkedList l, bool (*comparator)(void *, void *), void *comparisonArg);

/*
    Searches the LinkedList given a compare function and compare argument, will return the amount of times the element was found.
*/
int LinkedListCount(LinkedList l, bool (*comparator)(void *, void *), void *comparisonArg);

/*
    Concats the second LinkedList to the first. Returns if successful. Second LinkedList is emptied, not free'd!
*/
bool LinkedListConcat(LinkedList l, LinkedList l2);

/*
    Copies the LinkedList and returns it.
*/
LinkedList LinkedListCopy(LinkedList l);

/*
    Returns a sub LinkedList of l. All elements are copied! Be sure to free afterwards.
*/
LinkedList LinkedListGetSubLinkedList(LinkedList l, int i, int size);

/*
    Does the function on every element of the LinkedList.
*/
void LinkedListForEach(LinkedList l, void (*func)(void *, int i));

/*
    Filters the LinkedList.
*/
void LinkedListFilter(LinkedList l, bool (*comparator)(void *, void *), void *comparisonArg);

/*
    Returns a string of all elements as an int. Free the string afterwards.
*/
String LinkedListGetStringInt(LinkedList l);

/*
    Returns a string of all elements as an String. Free the string afterwards.
*/
String LinkedListGetStringString(LinkedList l);

/*
    For debugging purposes: Prints everyelement as a string.
*/
void LinkedListDebugPrintString(LinkedList l);

/*
    For debugging purposes: Prints everyelement as an int.
*/
void LinkedListDebugPrintInt(LinkedList l);

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
bool LinkedListCursorSearchNext(LinkedList l, bool (*comparator)(void *, void *), void *comparisonArg);

/*
    LinkedList iterator goes to before element that is true for the search. Returns if it was successful.
*/
bool LinkedListCursorSearchPrevious(LinkedList l, bool (*comparator)(void *, void *), void *comparisonArg);

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
bool LinkedListCursorChange(LinkedList l, void *data, DataArgs dataArgs);

/*
    Adds item to LinkedList before the LinkedListCursor. Returns if the LinkedList iterator was successful.
*/
bool LinkedListCursorAdd(LinkedList l, void *data, DataArgs dataArgs);

/*
    Deletes the entry at LinkedListCursor and goes after. Returns if successful.
*/
bool LinkedListCursorDelete(LinkedList l);

/*
    Data args LinkedList.
*/
DataArgs DataArgsLinkedList();

#endif /* __CHELPLinkedList__ */
