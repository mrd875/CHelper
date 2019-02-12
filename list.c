#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "list.h"
#include "common.h"
#include "internal/common.h"

/*
    This is the internal list node structure. Doublely linked.
*/
typedef struct ListNode *ListNode;

struct ListNode
{
    /* The node that is before this one. */
    ListNode previous;

    /* The node that is after this one. */
    ListNode next;

    /* The list that this node belongs to */
    List owner;

    /* The address to the data. */
    void *data;

    /* The function to free the data. */
    void (*free_routine)(void *data);

    /* The function to copy the data. */
    void *(*copy_routine)(void *data);
};

struct List
{
    /* The head node. */
    ListNode head;

    /* The tail node */
    ListNode tail;

    /* The size */
    int size;

    /* The cap of the list */
    int capacity;

    /* The address to the data. */
    ListNode cur;
};

struct ListIterator
{
    /* The node that this iterator points to */
    ListNode cur;

    /* The node that is next */
    ListNode next;

    /* The node that is prev */
    ListNode previous;
};

/*
    Creates a list node for the list.
*/
ListNode ListNodeCreate(List l)
{
    ListNode ln;

    assert(l != NULL);

    debugp("Creating a list node");

    ln = malloc(sizeof(struct ListNode));
    assert(ln != NULL);

    ln->copy_routine = NULL;
    ln->free_routine = NULL;
    ln->data = NULL;
    ln->next = NULL;
    ln->previous = NULL;
    ln->owner = l;

    return ln;
}

/*
    Frees the node's data.
*/
void ListNodeFreeData(ListNode ln)
{
    assert(ln != NULL);

    debugp("freeing list node data");

    if (ln->free_routine != NULL)
        ln->free_routine(ln->data);
}

/*
    Returns a copy of the node's data.
*/
void *ListNodeCopyData(ListNode ln)
{
    assert(ln != NULL);

    debugp("copying a list node data");

    if (ln->copy_routine == NULL)
        return NULL;

    return ln->copy_routine(ln->data);
}

/*
    Frees the node.
*/
void ListNodeFree(ListNode ln)
{
    assert(ln != NULL);

    debugp("Freeing a list node");

    ListNodeFreeData(ln);

    free(ln);
}

List ListCreate(int size)
{
    List l;

    debugp("Creating a list");

    l = malloc(sizeof(struct List));
    assert(l != NULL);

    l->capacity = size;
    l->size = 0;
    l->tail = NULL;
    l->head = NULL;
    l->cur = NULL;

    return l;
}

void ListFree(List l)
{
    assert(l != NULL);

    debugp("Freeing list");

    ListClear(l);

    free(l);
}

void ListClear(List l)
{
    ListNode ln, next;

    assert(l != NULL);

    debugp("Clearing list");

    ln = l->head;
    while (ln != NULL)
    {
        next = ln->next;

        ListNodeFree(ln);

        ln = next;
    }

    l->cur = NULL;
    l->size = 0;
    l->head = NULL;
    l->tail = NULL;
}

int ListSize(List l)
{
    assert(l != NULL);

    debugp("Getting list size");

    return l->size;
}

int ListCapacity(List l)
{
    assert(l != NULL);

    debugp("Getting list cap");

    return l->capacity;
}

bool ListFull(List l)
{
    int size;

    assert(l != NULL);

    debugp("Checking if full");

    size = ListSize(l);

    if (size < 0)
        return false;

    if (ListCapacity(l) < size)
        return false;

    return true;
}

bool ListEmpty(List l)
{
    assert(l != NULL);

    debugp("Checking if empty");

    if (ListSize(l) > 0)
        return false;

    return true;
}

/*
    Updates the iterator to be the node.
*/
void ListIteratorUpdate(ListIterator li, ListNode ln)
{
    assert(ln != NULL);
    assert(li != NULL);

    li->cur = ln;
    li->next = ln->next;
    li->previous = ln->previous;
}

/*
    Creates an iterator from a node.
*/
ListIterator ListIteratorCreate()
{
    ListIterator li;

    debugp("Creating list iterator");

    li = malloc(sizeof(struct ListIterator));
    assert(li != NULL);

    li->cur = NULL;
    li->next = NULL;
    li->previous = NULL;

    return li;
}

ListIterator ListGetCursorListIterator(List l)
{
    ListIterator li;
    ListNode ln;

    assert(l != NULL);

    debugp("Getting cursor list");

    ln = l->cur;

    if (ln == NULL)
        return NULL;

    li = ListIteratorCreate(ln);

    return li;
}

ListIterator ListGetHeadListIterator(List l)
{
    ListIterator li;
    ListNode ln;

    assert(l != NULL);

    debugp("Getting head list");

    ln = l->head;

    if (ln == NULL)
        return NULL;

    li = ListIteratorCreate();

    li->next = ln;

    return li;
}

ListIterator ListGetTailListIterator(List l)
{
    ListIterator li;
    ListNode ln;

    assert(l != NULL);

    debugp("Getting head list");

    ln = l->tail;

    if (ln == NULL)
        return NULL;

    li = ListIteratorCreate();

    li->previous = ln;

    return li;
}

void *ListGetFirst(List l)
{
    ListNode ln;

    assert(l != NULL);

    debugp("Getting head list data");

    ln = l->head;

    l->cur = ln;

    if (ln == NULL)
        return NULL;

    return ln->data;
}

void *ListGetLast(List l)
{
    ListNode ln;

    assert(l != NULL);

    debugp("Getting tail list data");

    ln = l->tail;

    l->cur = ln;

    if (ln == NULL)
        return NULL;

    return ln->data;
}

void *ListGetX(List l, int x)
{
    ListNode ln;
    int i;

    assert(l != NULL);
    assert(x >= 0);

    debugp("Getting x %d list data", x);

    ln = l->head;
    for (i = 0; i < x && ln != NULL; i++)
    {
        ln = ln->next;
    }

    l->cur = ln;

    if (ln == NULL)
        return NULL;

    return ln->data;
}

/*
    Changes the data at the node.
*/
bool ListNodeChange(ListNode ln, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data))
{
    assert(ln != NULL);

    if (ln->data != NULL)
        ListNodeFreeData(ln);

    ln->data = data;
    ln->free_routine = free_routine;
    ln->copy_routine = copy_routine;

    return true;
}

/*
    Adds the data to the list at the node. This is NOT for the edges case of insertion but only for inserting when the listnode has prev and next.
*/
bool ListNodeAdd(ListNode ln, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data))
{
    ListNode newNode, prev, next;
    List owner;

    assert(ln != NULL);
    assert(ln->next != NULL);
    assert(ln->previous != NULL);

    owner = ln->owner;

    if (ListFull(owner) == true)
        return false;

    prev = ln->previous;
    next = ln->next;

    newNode = ListNodeCreate(owner);

    if (ListNodeChange(newNode, data, free_routine, copy_routine) == false)
        return false;

    prev->next = newNode;
    newNode->previous = prev;

    newNode->next = next;
    next->previous = newNode;

    owner->size++;
    owner->cur = newNode;

    return true;
}

bool ListAddFirst(List l, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data))
{
    ListNode head, newNode;

    assert(l != NULL);

    if (ListFull(l) == true)
        return false;

    newNode = ListNodeCreate(l);

    if (ListNodeChange(newNode, data, free_routine, copy_routine) == false)
        return false;

    head = l->head;

    newNode->next = head;
    l->head = newNode;

    if (head == NULL)
        l->tail = newNode;
    else
        head->previous = newNode;

    l->size++;
    l->cur = newNode;

    return true;
}

bool ListAddLast(List l, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data))
{
    ListNode tail, newNode;

    assert(l != NULL);

    if (ListFull(l) == true)
        return false;

    newNode = ListNodeCreate(l);

    if (ListNodeChange(newNode, data, free_routine, copy_routine) == false)
        return false;

    tail = l->tail;

    newNode->previous = tail;
    l->tail = newNode;

    if (tail == NULL)
        l->head = newNode;
    else
        tail->next = newNode;

    l->size++;
    l->cur = newNode;

    return true;
}

bool ListAddX(List l, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data), int x)
{
    int i;
    ListNode ln;

    assert(l != NULL);
    assert(x >= 0);

    i = ListSize(l);

    assert(x <= i);

    if (x == 0)
        return ListAddFirst(l, data, free_routine, copy_routine);

    if (x == i)
        return ListAddLast(l, data, free_routine, copy_routine);

    ln = l->head;
    for (i = 0; i < x && ln != NULL; i++)
    {
        ln = ln->next;
    }

    assert(ln != NULL);
    return ListNodeAdd(ln, data, free_routine, copy_routine);
}

bool ListChangeX(List l, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data), int x)
{
    int i;
    ListNode ln;

    assert(l != NULL);
    assert(x >= 0);
    assert(x < ListSize(l));

    ln = l->head;
    for (i = 0; i < x && ln != NULL; i++)
    {
        ln = ln->next;
    }

    assert(ln != NULL);
    return ListNodeChange(ln, data, free_routine, copy_routine);
}

/*
    Deletes the node. This is not for edges cases but when the node has a next and prev.
*/
bool ListNodeDelete(ListNode ln)
{
    ListNode newNode, prev, next;
    List owner;

    assert(ln != NULL);
    assert(ln->next != NULL);
    assert(ln->previous != NULL);

    owner = ln->owner;

    assert(ListEmpty(owner) == false);

    prev = ln->previous;
    next = ln->next;

    ListNodeFree(ln);

    prev->next = next;
    next->previous = prev;

    owner->size--;
    owner->cur = next;

    return true;
}

bool ListDeleteFirst(List l)
{
    ListNode head, next;

    assert(l != NULL);

    if (ListEmpty(l) == true)
        return false;

    head = l->head;

    assert(head != NULL);

    next = head->next;

    ListNodeFree(head);

    l->head = next;

    if (next == NULL)
        l->tail = NULL;
    else
        next->previous = NULL;

    l->cur = next;
    l->size--;

    return true;
}

bool ListDeleteLast(List l)
{
    ListNode tail, prev;

    assert(l != NULL);

    if (ListEmpty(l) == true)
        return false;

    tail = l->tail;

    assert(tail != NULL);

    prev = tail->previous;

    ListNodeFree(tail);

    l->tail = prev;

    if (prev == NULL)
        l->head = NULL;
    else
        prev->next = NULL;

    l->cur = prev;
    l->size--;

    return true;
}

bool ListDeleteX(List l, int x)
{
    int i;
    ListNode ln;

    assert(l != NULL);
    assert(x >= 0);

    i = ListSize(l);

    assert(x < i);

    if (x == 0)
        return ListDeleteFirst(l);

    if (x == i - 1)
        return ListDeleteLast(l);

    ln = l->head;
    for (i = 0; i < x && ln != NULL; i++)
    {
        ln = ln->next;
    }

    assert(ln != NULL);
    return ListNodeDelete(ln);
}

int ListSearch(List l, bool (*comparator)(void *, void *), void *comparisonArg)
{
    ListNode ln;
    int i;

    assert(l != NULL);
    assert(comparator != NULL);

    l->cur = NULL;

    for (ln = l->head, i = 0; ln != NULL; ln = ln->next, i++)
    {
        if ((*comparator)(ln->data, comparisonArg) == false)
            continue;

        l->cur = ln;
        return i;
    }

    return -1;
}

int ListCount(List l, bool (*comparator)(void *, void *), void *comparisonArg)
{
    ListNode ln;
    int i;

    assert(l != NULL);
    assert(comparator != NULL);

    i = 0;

    for (ln = l->head; ln != NULL; ln = ln->next)
    {
        if ((*comparator)(ln->data, comparisonArg) == false)
            continue;

        i++;
    }

    return i;
}

bool ListConcat(List the_List, List two_List)
{
    ListNode two_head, two_tail, List_tail;

    assert(the_List != NULL);
    assert(two_List != NULL);

    two_head = two_List->head;
    two_tail = two_List->tail;
    List_tail = the_List->tail;

    the_List->size += two_List->size;

    two_List->head = NULL;
    ListClear(two_List);

    if (two_head == NULL)
        return true;

    if (List_tail == NULL)
    {
        the_List->head = two_head;
        the_List->tail = two_tail;

        return true;
    }

    List_tail->next = two_head;
    two_head->previous = List_tail;

    return true;
}

List ListCopy(List l)
{
    List result;
    ListNode ln;
    void *data;

    assert(l != NULL);

    result = ListCreate(l->capacity);

    for (ln = l->head; ln != NULL; ln = ln->next)
    {
        data = ListNodeCopyData(ln);

        ListAddLast(result, data, ln->free_routine, ln->copy_routine);
    }

    return result;
}

void ListForEach(List l, void (*func)(void *, int i))
{
    ListNode ln;
    int i;

    assert(l != NULL);
    assert(func != NULL);

    for (ln = l->head, i = 0; ln != NULL; ln = ln->next, i++)
    {
        (*func)(ln->data, i);
    }
}

void ListFilter(List l, bool (*comparator)(void *, void *), void *comparisonArg)
{
    int i;

    assert(l != NULL);
    assert(comparator != NULL);

    while ((i = ListSearch(l, comparator, comparisonArg)) > -1)
    {
        ListDeleteX(l, i);
    }
}

void ListDebugPrintString(List l)
{
    ListNode ln;
    char *i;

    assert(l != NULL);

    ln = l->head;

    printf("[");

    while (ln != NULL)
    {
        i = ln->data;

        if (i != NULL)
            printf("'%s'", i);
        else
            printf("NULL");

        ln = ln->next;

        if (ln != NULL)
            printf(", ");
    }

    printf("]\n");
}

void ListDebugPrintInt(List l)
{
    ListNode ln;
    int *i;

    assert(l != NULL);

    ln = l->head;

    printf("[");

    while (ln != NULL)
    {
        i = ln->data;

        if (i != NULL)
            printf("%d", *i);
        else
            printf("NULL");

        ln = ln->next;

        if (ln != NULL)
            printf(", ");
    }

    printf("]\n");
}

void ListIteratorFree(ListIterator li)
{
    assert(li != NULL);

    free(li);
}

bool ListIteratorNext(ListIterator li)
{
    assert(li != NULL);

    if (li->cur == NULL && li->next == NULL)
        return false;

    if (li->cur != NULL && li->cur->next)
    {
        ListIteratorUpdate(li, li->cur->next);
        return true;
    }

    if (li->next != NULL)
    {
        ListIteratorUpdate(li, li->next);
        return true;
    }

    li->previous = li->cur;
    li->cur = NULL;
    return true;
}

bool ListIteratorPrevious(ListIterator li)
{
    assert(li != NULL);

    if (li->cur == NULL && li->previous == NULL)
        return false;

    if (li->cur != NULL && li->cur->previous != NULL)
    {
        ListIteratorUpdate(li, li->cur->previous);
        return true;
    }

    if (li->previous != NULL)
    {
        ListIteratorUpdate(li, li->previous);
        return true;
    }

    li->next = li->cur;
    li->cur = NULL;
    return true;
}

bool ListIteratorIsNull(ListIterator li)
{
    if (li != NULL)
        return false;

    return true;
}

bool ListIteratorSearchNext(ListIterator li, bool (*comparator)(void *, void *), void *comparisonArg)
{
    while ((*comparator)(ListIteratorGet(li), comparisonArg) == false)
    {
    }
}

/*
    List iterator goes to before element that is true for the search. Returns if it was successful.
*/
bool ListIteratorSearchBefore(ListIterator li, bool (*comparator)(void *, void *), void *comparisonArg);

/*
    Returns if the list iterator has next.
*/
bool ListIteratorHasNext(ListIterator li);

/*
    Returns if the list iterator has before.
*/
bool ListIteratorHasBefore(ListIterator li);

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