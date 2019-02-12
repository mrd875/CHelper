#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

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

    /* The cursor. */
    struct
    {
        /* The node that this iterator points to */
        ListNode current;

        /* The node that is next */
        ListNode previous;
    } cursor;
};

/*
    Updates the cursor.
*/
void ListCursorUpdate(List l)
{
    assert(l != NULL);

    if (l->cursor.current == NULL && l->cursor.previous == NULL)
        return; /* At before the head. No need to update */

    if (l->cursor.previous != NULL && l->cursor.current == NULL)
    {
        l->cursor.previous = l->tail;
        return; /* At after the tail*/
    }

    if (l->cursor.current != NULL)
    {
        l->cursor.previous = l->cursor.current->previous;
        return;
    }
}

/*
    Clears the list iter
*/
void ListCursorClear(List l)
{
    assert(l != NULL);

    l->cursor.previous = NULL;
    l->cursor.current = NULL;
}

/*
    Sets the cursor.
*/
void ListCursorSetTo(List l, ListNode ln)
{
    assert(l != NULL);

    l->cursor.current = ln;

    if (ln != NULL)
    {
        l->cursor.previous = ln->previous;
        return;
    }

    if (l->cursor.previous != NULL)
    {
        l->cursor.previous = l->tail;
        return;
    }
}

/*
    Creates a list node for the list.
*/
ListNode ListNodeCreate()
{
    ListNode ln;

    debugp("Creating a list node");

    ln = malloc(sizeof(struct ListNode));
    assert(ln != NULL);

    ln->copy_routine = NULL;
    ln->free_routine = NULL;
    ln->data = NULL;
    ln->next = NULL;
    ln->previous = NULL;

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
    ListCursorClear(l);

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

    ListCursorClear(l);
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

void *ListGetFirst(List l)
{
    ListNode ln;

    assert(l != NULL);

    debugp("Getting head list data");

    ln = l->head;

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
    Adds the data to the list at the node. This is NOT for the edges case of insertion but only for inserting when the listnode has prev.
*/
bool ListNodeAdd(List l, ListNode ln, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data))
{
    ListNode newNode, prev;

    assert(ln != NULL);
    assert(l != NULL);
    assert(ln->previous != NULL);

    if (ListFull(l) == true)
        return false;

    prev = ln->previous;

    newNode = ListNodeCreate();

    if (ListNodeChange(newNode, data, free_routine, copy_routine) == false)
        return false;

    prev->next = newNode;
    newNode->previous = prev;

    newNode->next = ln;
    ln->previous = newNode;

    l->size++;
    ListCursorUpdate(l);

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
    ListCursorUpdate(l);

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
    ListCursorUpdate(l);

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
    return ListNodeAdd(l, ln, data, free_routine, copy_routine);
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
bool ListNodeDelete(List l, ListNode ln)
{
    ListNode prev, next;

    assert(ln != NULL);
    assert(ln->next != NULL);
    assert(ln->previous != NULL);

    assert(ListEmpty(l) == false);

    prev = ln->previous;
    next = ln->next;

    prev->next = next;
    next->previous = prev;

    l->size--;

    if (ln == l->cursor.current)
        ListCursorSetTo(l, next);
    else
        ListCursorUpdate(l);

    ListNodeFree(ln);

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

    l->size--;

    if (head == l->cursor.current)
        ListCursorSetTo(l, next);
    else
        ListCursorUpdate(l);

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

    l->size--;

    if (tail == l->cursor.current)
        ListCursorSetTo(l, NULL);
    else
        ListCursorUpdate(l);

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
    return ListNodeDelete(l, ln);
}

int ListSearch(List l, bool (*comparator)(void *, void *), void *comparisonArg)
{
    ListNode ln;
    int i;

    assert(l != NULL);
    assert(comparator != NULL);

    for (ln = l->head, i = 0; ln != NULL; ln = ln->next, i++)
    {
        if ((*comparator)(ln->data, comparisonArg) == false)
            continue;

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
    ListCursorClear(the_List);

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

List ListGetSubList(List l, int i, int size)
{
    List result;
    ListNode ln;
    int x;
    void *data;

    assert(l != NULL);
    assert(i >= 0);
    assert(i < ListSize(l));

    result = ListCreate(-1);

    for (x = 0, ln = l->head; x < i && ln != NULL; x++, ln = ln->next)
        ;

    for (x = 0; x < size && ln != NULL; x++, ln = ln->next)
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

bool ListCursorNext(List l)
{
    assert(l != NULL);

    if (l->cursor.current == NULL)
    {
        if (l->cursor.previous != NULL)
            return false;

        ListCursorSetTo(l, l->head);
        return true;
    }

    ListCursorSetTo(l, l->cursor.current->next);
    return true;
}

bool ListCursorPrevious(List l)
{
    assert(l != NULL);

    if (l->cursor.previous == NULL)
    {
        if (l->cursor.current == NULL)
            return false;

        ListCursorSetTo(l, NULL);
        return true;
    }

    ListCursorSetTo(l, l->cursor.previous);
    return true;
}

bool ListCursorIsNull(List l)
{
    assert(l != NULL);

    if (l->cursor.current != NULL)
        return false;

    return true;
}

bool ListCursorBeforeHead(List l)
{
    l->cursor.current = NULL;
    l->cursor.previous = NULL;

    return true;
}

bool ListCursorAfterTail(List l)
{
    l->cursor.current = NULL;
    l->cursor.previous = l->tail;

    return true;
}

bool ListCursorSearchNext(List l, bool (*comparator)(void *, void *), void *comparisonArg)
{
    assert(l != NULL);
    assert(comparator != NULL);

    ListCursorNext(l);
    while (ListCursorIsNull(l) == false && (*comparator)(ListCursorGet(l), comparisonArg) == false)
    {
        ListCursorNext(l);
    }

    if (ListCursorIsNull(l) == false)
        return false;

    return true;
}

bool ListCursorSearchBefore(List l, bool (*comparator)(void *, void *), void *comparisonArg)
{
    assert(l != NULL);
    assert(comparator != NULL);

    ListCursorPrevious(l);
    while (ListCursorIsNull(l) == false && (*comparator)(ListCursorGet(l), comparisonArg) == false)
    {
        ListCursorPrevious(l);
    }

    if (ListCursorIsNull(l) == false)
        return false;

    return true;
}

bool ListCursorHasNext(List l)
{
    assert(l != NULL);

    if (l->cursor.current == NULL)
    {
        if (l->cursor.previous != NULL)
            return false;

        if (l->head == NULL)
            return false;

        return true;
    }

    if (l->cursor.current->next == NULL)
        return false;

    return true;
}

bool ListCursorHasPrevious(List l)
{
    assert(l != NULL);

    if (l->cursor.previous == NULL)
        return false;

    return true;
}

void *ListCursorGet(List l)
{
    assert(l != NULL);
    assert(ListCursorIsNull(l) == false);

    return l->cursor.current->data;
}

bool ListCursorChange(List l, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data))
{
    assert(l != NULL);
    assert(ListCursorIsNull(l) == false);

    return ListNodeChange(l->cursor.current, data, free_routine, copy_routine);
}

bool ListCursorAdd(List l, void *data, void (*free_routine)(void *data), void *(*copy_routine)(void *data))
{
    assert(l != NULL);

    if (l->cursor.previous == NULL)
        return ListAddFirst(l, data, free_routine, copy_routine);

    if (l->cursor.previous == l->tail)
        return ListAddLast(l, data, free_routine, copy_routine);

    return ListNodeAdd(l, l->cursor.current, data, free_routine, copy_routine);
}

bool ListCursorDelete(List l)
{
    assert(l != NULL);
    assert(ListCursorIsNull(l) == false);

    if (l->cursor.current == l->head)
        return ListDeleteFirst(l);

    if (l->cursor.current == l->tail)
        return ListDeleteLast(l);

    return ListNodeDelete(l, l->cursor.current);
}
