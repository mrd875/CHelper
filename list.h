/*
    Mason Demerais
    mrd875
    11202338
*/

#ifndef __LIST__
#define __LIST__

/*
    This is the structure of a List.
*/
typedef struct List *List;

/*
    This is a List iterator.
*/
typedef struct ListNode *ListIterator;

/*
    Initiates the List, returns a pointer to the caller, be sure to free the List with free_List().
*/
List *ListInit();

/*
    Frees the List, will also free all data inside the List for the caller.
*/
void ListFree(List *the_List);

/*
    Empties the List.
*/
void ListClear(List *the_List);

/*
    Returns how many elements are in the List.
*/
int ListSize(List *the_List);

/*
    Returns the cursor of the List.
*/
ListIterator *ListGetCurrentListIterator(List *the_List);

/*
    Returns a List iterator at the List's head.
*/
ListIterator *ListGetHeadListIterator(List *the_List);

/*
    Returns a List iterator at the List's tail.
*/
ListIterator *ListGetTailListIterator(List *the_List);

/*
    Returns a List iterator at the ListIterator's next.
*/
ListIterator *ListIteratorGetNext(ListIterator *the_iter);

/*
    Returns a List iterator at the ListIterator's before.
*/
ListIterator *ListIteratorGetBefore(ListIterator *the_iter);

/*
    Returns an iterator that is pointed at the found element next from the ListIterator. Returns NULL if not found.
*/
ListIterator *ListIteratorSearchNext(ListIterator *the_iter, int (*comparator)(void *, void *), void *comparisonArg);

/*
    Returns an iterator that is pointed at the found element before from the ListIterator. Returns NULL if not found.
*/
ListIterator *ListIteratorSearchBefore(ListIterator *the_iter, int (*comparator)(void *, void *), void *comparisonArg);

/*
    Returns 0 if the ListIterator is a head.
*/
int ListIteratorIsHead(ListIterator *the_iter);

/*
    Returns 0 if the ListIterator is a tail.
*/
int ListIteratorIsTail(ListIterator *the_iter);

/*
    Returns 0 if the ListIterator has a before.
*/
int ListIteratorHasBefore(ListIterator *the_iter);

/*
    Returns 0 if the ListIterator has a next.
*/
int ListIteratorHasNext(ListIterator *the_iter);

/*
    Returns the data at the ListIterator. Sets size.
*/
void *ListIteratorGetData(ListIterator *the_iter, int *size);

/*
    Changes the ListIterator's data. Returns the address if successful. NULL otherwise.
*/
void *ListIteratorChange(ListIterator *the_iter, void *data, int size, void (*free_routine)(void *data));

/*
    Adds item to List before the ListIterator. Returns the ListIterator of data added if successful, NULL otherwise.
*/
ListIterator *ListIteratorAddBefore(ListIterator *the_iter, void *data, int size, void (*free_routine)(void *data));

/*
    Adds item to List after the ListIterator. Returns the ListIterator of data added if successful, NULL otherwise.
*/
ListIterator *ListIteratorAddAfter(ListIterator *the_iter, void *data, int size, void (*free_routine)(void *data));

/*
    Deletes the entry at ListIterator and returns the before ListIterator. NULL if there isn't one.
*/
ListIterator *ListIteratorDeleteAndGoBefore(ListIterator *the_iter);

/*
    Deletes the entry at ListIterator and returns the next ListIterator. NULL if there isn't one.
*/
ListIterator *ListIteratorDeleteAndGoNext(ListIterator *the_iter);

/*
    Returns the address of the first element in the List, will return NULL if there isn't one. Size gets updated to the size of bytes of the data.
*/
void *ListGetFirst(List *the_List, int *size);

/*
    Returns the address of the last element in the List, will return NULL if there isn't one. Size gets updated to the size of bytes of the data.
*/
void *ListGetLast(List *the_List, int *size);

/*
    Returns the address of the x'th element in the List, will return NULL if there isn't one. Size gets updated to the size of bytes of the data.
*/
void *ListGetX(List *the_List, int *size, int x);

/*
    Adds the given data to the start of the List, returns the address of the data added if successful, NULL otherwise.
*/
void *ListAddFirst(List *the_List, void *data, int size, void (*free_routine)(void *data));

/*
    Adds the given data to the end of the List, returns the address of the data added if successful, NULL otherwise.
*/
void *ListAddLast(List *the_List, void *data, int size, void (*free_routine)(void *data));

/*
    Adds the given data at the x'th spot of the List (pushes everything else after futher down the List), returns the address of the data added if successful, NULL otherwise.
*/
void *ListAddX(List *the_List, void *data, int size, void (*free_routine)(void *data), int x);

/*
    Changes the data at the x'th spot in the List, will free the old data. Returns the address of the data changed if successful, NULL otherwise.
*/
void *ListChangeX(List *the_List, void *data, int size, void (*free_routine)(void *data), int x);

/*
    Deletes and frees the data found at the end of the List. Returns 0 if successful.
*/
int ListDeleteLast(List *the_List);

/*
    Deletes and frees the data found at the start of the List. Returns 0 if successful.
*/
int ListDeleteFirst(List *the_List);

/*
    Deletes and frees the data found at the x'th spot of the List. Returns 0 if successful.
*/
int ListDeleteX(List *the_List, int x);

/*
    Searches the List given a compare function and compare argument, will return the index of the found element, -1 if not found.
*/
int ListSearch(List *the_List, int (*comparator)(void *, void *), void *comparisonArg);

/*
    Searches the List given a compare function and compare argument, will return the amount of times the element was found.
*/
int ListCount(List *the_List, int (*comparator)(void *, void *), void *comparisonArg);

/*
    Concats the second List to the first. Returns the concat'd List. Destroys the second List.
*/
List *ListConcat(List *the_List, List *two_List);

/*
    Copies the List and returns it.
*/
List *ListCopy(List *the_List);

/*
    Does the function on every element of the List.
*/
void ListForEach(List *the_List, void (*func)(void *, int i));

/*
    Filters the List.
*/
void ListFilter(List *the_List, int (*comparator)(void *, void *), void *comparisonArg);

/*
    For debugging purposes: Prints everyelement as a string.
*/
void ListDebugPrintString(List *L);

/*
    For debugging purposes: Prints everyelement as an int.
*/
void ListDebugPrintInt(List *L);

/*
    Mallocs and inserts i and returns the address.
*/
int *qcint(int i);

/*
    Mallocs and inserts the string and returns the address.
*/
char *qcstring(char *s);

/*
    Runs the test cases.
*/
void test_List();

#endif /* __LIST__ */
