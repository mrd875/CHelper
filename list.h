#ifndef __CHELPLIST__
#define __CHELPLIST__

#include <stdbool.h>

/*
    This is the data type for lists.
*/
typedef struct List *List;

/*
    This is the data type for list iterators.
*/
typedef struct ListIterator *ListIterator;

/*
    Returns a newly created list. For unlimited size, set to -1.
*/
List ListCreate(int size);

/*
    Frees the list, will also free all data inside the list for the caller.
*/
void ListFree(List l);

/*
    Empties the List. All data inside is free'd.
*/
void ListClear(List l);

/*
    Returns how many elements are in the List.
*/
int ListSize(List l);

/*
    Returns the capacity of the list.
*/
int ListCapacity(List l);

/*
    Returns if the list is full.
*/
bool ListFull(List l);

/*
    Returns if the list is empty.
*/
bool ListEmpty(List l);

/*
    Returns the list iterator at the list's last operation. Be sure to free.
*/
ListIterator ListGetCursorListIterator(List l);

/*
    Returns a List iterator just before the list's head. Be sure to free.
*/
ListIterator ListGetHeadListIterator(List l);

/*
    Returns a List iterator just after the list's tail. Be sure to free.
*/
ListIterator ListGetTailListIterator(List l);

/*
    Returns the address of the first element in the List, will return NULL if there isn't one.
*/
void *ListGetFirst(List l);

/*
    Returns the address of the last element in the List, will return NULL if there isn't one.
*/
void *ListGetLast(List l);

/*
    Returns the address of the x'th element in the List, will return NULL if there isn't one.
*/
void *ListGetX(List l, int x);

/*
    Adds the given data to the start of the List, returns if it was successful. free and copy are pointers to functions that know how to free and copy the data.
*/
bool ListAddFirst(List l, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data));

/*
    Adds the given data to the end of the List, returns if it was successful.
*/
bool ListAddLast(List l, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data));

/*
    Adds the given data at the x'th spot of the List (pushes everything else after futher down the List), returns if it was successful.
*/
bool ListAddX(List l, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data), int x);

/*
    Changes the data at the x'th spot in the List, will free the old data. Returns if it was successful.
*/
bool ListChangeX(List l, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data), int x);

/*
    Deletes and frees the data found at the end of the List. Returns if successful.
*/
bool ListDeleteLast(List l);

/*
    Deletes and frees the data found at the start of the List. Returns if successful.
*/
bool ListDeleteFirst(List l);

/*
    Deletes and frees the data found at the x'th spot of the List. Returns if successful.
*/
bool ListDeleteX(List l, int x);

/*
    Searches the List given a compare function and compare argument, will return the index of the found element, -1 if not found.
*/
int ListSearch(List l, bool (*comparator)(void *, void *), void *comparisonArg);

/*
    Searches the List given a compare function and compare argument, will return the amount of times the element was found.
*/
int ListCount(List l, bool (*comparator)(void *, void *), void *comparisonArg);

/*
    Concats the second List to the first. Returns if successful. Second list is emptied, not free'd!
*/
bool ListConcat(List l, List l2);

/*
    Copies the List and returns it.
*/
List ListCopy(List l);

/*
    Does the function on every element of the List.
*/
void ListForEach(List l, void (*func)(void *, int i));

/*
    Filters the List.
*/
void ListFilter(List l, bool (*comparator)(void *, void *), void *comparisonArg);

/*
    For debugging purposes: Prints everyelement as a string.
*/
void ListDebugPrintString(List l);

/*
    For debugging purposes: Prints everyelement as an int.
*/
void ListDebugPrintInt(List l);

/*
    Frees the list iterator.
*/
void ListIteratorFree(ListIterator li);

/*
    List iterator goes to next thing in the list. Returns if successful.
*/
bool ListIteratorNext(ListIterator li);

/*
    List iterator goes to next thing in the list. Returns if successful.
*/
bool ListIteratorPrevious(ListIterator li);

/*
    Returns if the iterator is on a null spot of the list (edges).
*/
bool ListIteratorIsNull(ListIterator li);

/*
    List iterator goes to next element that is true for the search. Returns if it was successful.
*/
bool ListIteratorSearchNext(ListIterator li, bool (*comparator)(void *, void *), void *comparisonArg);

/*
    List iterator goes to before element that is true for the search. Returns if it was successful.
*/
bool ListIteratorSearchPrevious(ListIterator li, bool (*comparator)(void *, void *), void *comparisonArg);

/*
    Returns if the list iterator has next.
*/
bool ListIteratorHasNext(ListIterator li);

/*
    Returns if the list iterator has before.
*/
bool ListIteratorHasPrevious(ListIterator li);

/*
    Returns the data at the ListIterator.
*/
void *ListIteratorGet(ListIterator li);

/*
    Changes the ListIterator's data. Returns if successful.
*/
bool ListIteratorChange(ListIterator li, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data));

/*
    Adds item to List before the ListIterator. Returns if the list iterator was successful.
*/
bool ListIteratorAdd(ListIterator li, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data));

/*
    Deletes the entry at ListIterator and goes before. Returns if successful.
*/
bool ListIteratorDelete(ListIterator li);

#endif /* __CHELPLIST__ */
