#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist.h"
#include "common.h"
#include "internal/common.h"
#include "string.h"

/*
    This is the internal LinkedList node structure. Doublely linked.
*/
typedef struct LinkedListNode *LinkedListNode;

struct LinkedListNode
{
    /* The node that is before this one. */
    LinkedListNode previous;

    /* The node that is after this one. */
    LinkedListNode next;

    /* The address to the data. */
    void *data;

    /* The function to free the data. */
    void (*free_routine)(void *data);

    /* The function to copy the data. */
    void *(*copy_routine)(void *data);

    /* Size of the data */
    int size;
};

struct LinkedList
{
    /* The head node. */
    LinkedListNode head;

    /* The tail node */
    LinkedListNode tail;

    /* The size */
    int size;

    /* The cap of the LinkedList */
    int capacity;

    /* The cursor. */
    struct
    {
        /* The node that this iterator points to */
        LinkedListNode current;

        /* The node that is next */
        LinkedListNode previous;
    } cursor;
};

/*
    Updates the cursor.
*/
void LinkedListCursorUpdate(LinkedList l)
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
    Clears the LinkedList iter
*/
void LinkedListCursorClear(LinkedList l)
{
    assert(l != NULL);

    l->cursor.previous = NULL;
    l->cursor.current = NULL;
}

/*
    Sets the cursor.
*/
void LinkedListCursorSetTo(LinkedList l, LinkedListNode ln)
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
    Creates a LinkedList node for the LinkedList.
*/
LinkedListNode LinkedListNodeCreate()
{
    LinkedListNode ln;

    debugp("Creating a LinkedList node");

    ln = malloc(sizeof(struct LinkedListNode));
    assert(ln != NULL);

    ln->copy_routine = NULL;
    ln->free_routine = NULL;
    ln->data = NULL;
    ln->next = NULL;
    ln->previous = NULL;
    ln->size = 0;

    return ln;
}

/*
    Frees the node's data.
*/
void LinkedListNodeFreeData(LinkedListNode ln)
{
    assert(ln != NULL);

    debugp("freeing LinkedList node data");

    if (ln->free_routine != NULL)
        ln->free_routine(ln->data);
}

/*
    Returns a copy of the node's data.
*/
void *LinkedListNodeCopyData(LinkedListNode ln)
{
    assert(ln != NULL);

    debugp("copying a LinkedList node data");

    if (ln->copy_routine == NULL)
        return NULL;

    return ln->copy_routine(ln->data);
}

/*
    Frees the node.
*/
void LinkedListNodeFree(LinkedListNode ln)
{
    assert(ln != NULL);

    debugp("Freeing a LinkedList node");

    LinkedListNodeFreeData(ln);

    free(ln);
}

LinkedList LinkedListCreate(int size)
{
    LinkedList l;

    debugp("Creating a LinkedList");

    l = malloc(sizeof(struct LinkedList));
    assert(l != NULL);

    l->capacity = size;
    l->size = 0;
    l->tail = NULL;
    l->head = NULL;
    LinkedListCursorClear(l);

    return l;
}

void LinkedListFree(LinkedList l)
{
    assert(l != NULL);

    debugp("Freeing LinkedList");

    LinkedListClear(l);

    free(l);
}

void LinkedListClear(LinkedList l)
{
    LinkedListNode ln, next;

    assert(l != NULL);

    debugp("Clearing LinkedList");

    ln = l->head;
    while (ln != NULL)
    {
        next = ln->next;

        LinkedListNodeFree(ln);

        ln = next;
    }

    LinkedListCursorClear(l);
    l->size = 0;
    l->head = NULL;
    l->tail = NULL;
}

int LinkedListSize(LinkedList l)
{
    assert(l != NULL);

    debugp("Getting LinkedList size");

    return l->size;
}

int LinkedListCapacity(LinkedList l)
{
    assert(l != NULL);

    debugp("Getting LinkedList cap");

    return l->capacity;
}

bool LinkedListFull(LinkedList l)
{
    int size;

    assert(l != NULL);

    debugp("Checking if full");

    size = LinkedListSize(l);

    if (size < 0)
        return false;

    if (LinkedListCapacity(l) < size)
        return false;

    return true;
}

bool LinkedListEmpty(LinkedList l)
{
    assert(l != NULL);

    debugp("Checking if empty");

    if (LinkedListSize(l) > 0)
        return false;

    return true;
}

void *LinkedListGetFirst(LinkedList l)
{
    LinkedListNode ln;

    assert(l != NULL);

    debugp("Getting head LinkedList data");

    ln = l->head;

    if (ln == NULL)
        return NULL;

    return ln->data;
}

void *LinkedListGetLast(LinkedList l)
{
    LinkedListNode ln;

    assert(l != NULL);

    debugp("Getting tail LinkedList data");

    ln = l->tail;

    if (ln == NULL)
        return NULL;

    return ln->data;
}

void *LinkedListGetX(LinkedList l, int x)
{
    LinkedListNode ln;
    int i;

    assert(l != NULL);
    assert(x >= 0);

    debugp("Getting x %d LinkedList data", x);

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
bool LinkedListNodeChange(LinkedListNode ln, void *data, DataArgs dataArgs)
{
    assert(ln != NULL);

    if (ln->data != NULL)
        LinkedListNodeFreeData(ln);

    ln->data = data;

    ln->free_routine = dataArgs.free_routine;
    ln->copy_routine = dataArgs.copy_routine;
    ln->size = dataArgs.size;
    return true;
}

/*
    Adds the data to the LinkedList at the node. This is NOT for the edges case of insertion but only for inserting when the LinkedListnode has prev.
*/
bool LinkedListNodeAdd(LinkedList l, LinkedListNode ln, void *data, DataArgs dataArgs)
{
    LinkedListNode newNode, prev;

    assert(ln != NULL);
    assert(l != NULL);
    assert(ln->previous != NULL);

    if (LinkedListFull(l) == true)
        return false;

    prev = ln->previous;

    newNode = LinkedListNodeCreate();

    if (LinkedListNodeChange(newNode, data, dataArgs) == false)
        return false;

    prev->next = newNode;
    newNode->previous = prev;

    newNode->next = ln;
    ln->previous = newNode;

    l->size++;
    LinkedListCursorUpdate(l);

    return true;
}

bool LinkedListAddFirst(LinkedList l, void *data, DataArgs dataArgs)
{
    LinkedListNode head, newNode;

    assert(l != NULL);

    if (LinkedListFull(l) == true)
        return false;

    newNode = LinkedListNodeCreate(l);

    if (LinkedListNodeChange(newNode, data, dataArgs) == false)
        return false;

    head = l->head;

    newNode->next = head;
    l->head = newNode;

    if (head == NULL)
        l->tail = newNode;
    else
        head->previous = newNode;

    l->size++;
    LinkedListCursorUpdate(l);

    return true;
}

bool LinkedListAddLast(LinkedList l, void *data, DataArgs dataArgs)
{
    LinkedListNode tail, newNode;

    assert(l != NULL);

    if (LinkedListFull(l) == true)
        return false;

    newNode = LinkedListNodeCreate(l);

    if (LinkedListNodeChange(newNode, data, dataArgs) == false)
        return false;

    tail = l->tail;

    newNode->previous = tail;
    l->tail = newNode;

    if (tail == NULL)
        l->head = newNode;
    else
        tail->next = newNode;

    l->size++;
    LinkedListCursorUpdate(l);

    return true;
}

bool LinkedListAddX(LinkedList l, void *data, DataArgs dataArgs, int x)
{
    int i;
    LinkedListNode ln;

    assert(l != NULL);
    assert(x >= 0);

    i = LinkedListSize(l);

    assert(x <= i);

    if (x == 0)
        return LinkedListAddFirst(l, data, dataArgs);

    if (x == i)
        return LinkedListAddLast(l, data, dataArgs);

    ln = l->head;
    for (i = 0; i < x && ln != NULL; i++)
    {
        ln = ln->next;
    }

    assert(ln != NULL);
    return LinkedListNodeAdd(l, ln, data, dataArgs);
}

bool LinkedListChangeX(LinkedList l, void *data, DataArgs dataArgs, int x)
{
    int i;
    LinkedListNode ln;

    assert(l != NULL);
    assert(x >= 0);
    assert(x < LinkedListSize(l));

    ln = l->head;
    for (i = 0; i < x && ln != NULL; i++)
    {
        ln = ln->next;
    }

    assert(ln != NULL);
    return LinkedListNodeChange(ln, data, dataArgs);
}

/*
    Deletes the node. This is not for edges cases but when the node has a next and prev.
*/
bool LinkedListNodeDelete(LinkedList l, LinkedListNode ln)
{
    LinkedListNode prev, next;

    assert(ln != NULL);
    assert(ln->next != NULL);
    assert(ln->previous != NULL);

    assert(LinkedListEmpty(l) == false);

    prev = ln->previous;
    next = ln->next;

    prev->next = next;
    next->previous = prev;

    l->size--;

    if (ln == l->cursor.current)
        LinkedListCursorSetTo(l, next);
    else
        LinkedListCursorUpdate(l);

    LinkedListNodeFree(ln);

    return true;
}

bool LinkedListDeleteFirst(LinkedList l)
{
    LinkedListNode head, next;

    assert(l != NULL);

    if (LinkedListEmpty(l) == true)
        return false;

    head = l->head;

    assert(head != NULL);

    next = head->next;

    LinkedListNodeFree(head);

    l->head = next;

    if (next == NULL)
        l->tail = NULL;
    else
        next->previous = NULL;

    l->size--;

    if (head == l->cursor.current)
        LinkedListCursorSetTo(l, next);
    else
        LinkedListCursorUpdate(l);

    return true;
}

bool LinkedListDeleteLast(LinkedList l)
{
    LinkedListNode tail, prev;

    assert(l != NULL);

    if (LinkedListEmpty(l) == true)
        return false;

    tail = l->tail;

    assert(tail != NULL);

    prev = tail->previous;

    LinkedListNodeFree(tail);

    l->tail = prev;

    if (prev == NULL)
        l->head = NULL;
    else
        prev->next = NULL;

    l->size--;

    if (tail == l->cursor.current)
        LinkedListCursorSetTo(l, NULL);
    else
        LinkedListCursorUpdate(l);

    return true;
}

bool LinkedListDeleteX(LinkedList l, int x)
{
    int i;
    LinkedListNode ln;

    assert(l != NULL);
    assert(x >= 0);

    i = LinkedListSize(l);

    assert(x < i);

    if (x == 0)
        return LinkedListDeleteFirst(l);

    if (x == i - 1)
        return LinkedListDeleteLast(l);

    ln = l->head;
    for (i = 0; i < x && ln != NULL; i++)
    {
        ln = ln->next;
    }

    assert(ln != NULL);
    return LinkedListNodeDelete(l, ln);
}

int LinkedListSearch(LinkedList l, bool (*comparator)(void *, void *), void *comparisonArg)
{
    LinkedListNode ln;
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

int LinkedListCount(LinkedList l, bool (*comparator)(void *, void *), void *comparisonArg)
{
    LinkedListNode ln;
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

bool LinkedListConcat(LinkedList the_LinkedList, LinkedList two_LinkedList)
{
    LinkedListNode two_head, two_tail, LinkedList_tail;

    assert(the_LinkedList != NULL);
    assert(two_LinkedList != NULL);

    two_head = two_LinkedList->head;
    two_tail = two_LinkedList->tail;
    LinkedList_tail = the_LinkedList->tail;

    the_LinkedList->size += two_LinkedList->size;

    two_LinkedList->head = NULL;
    LinkedListClear(two_LinkedList);
    LinkedListCursorClear(the_LinkedList);

    if (two_head == NULL)
        return true;

    if (LinkedList_tail == NULL)
    {
        the_LinkedList->head = two_head;
        the_LinkedList->tail = two_tail;

        return true;
    }

    LinkedList_tail->next = two_head;
    two_head->previous = LinkedList_tail;

    return true;
}

LinkedList LinkedListCopy(LinkedList l)
{
    LinkedList result;
    LinkedListNode ln;
    void *data;
    DataArgs dataArgs;

    assert(l != NULL);

    result = LinkedListCreate(l->capacity);

    for (ln = l->head; ln != NULL; ln = ln->next)
    {
        data = LinkedListNodeCopyData(ln);

        dataArgs.size = ln->size;
        dataArgs.free_routine = ln->free_routine;
        dataArgs.copy_routine = ln->copy_routine;

        LinkedListAddLast(result, data, dataArgs);
    }

    return result;
}

LinkedList LinkedListGetSubLinkedList(LinkedList l, int i, int size)
{
    LinkedList result;
    LinkedListNode ln;
    int x;
    void *data;
    DataArgs dataArgs;

    assert(l != NULL);
    assert(i >= 0);
    assert(i < LinkedListSize(l));

    result = LinkedListCreate(-1);

    for (x = 0, ln = l->head; x < i && ln != NULL; x++, ln = ln->next)
        ;

    for (x = 0; x < size && ln != NULL; x++, ln = ln->next)
    {
        data = LinkedListNodeCopyData(ln);
        dataArgs.copy_routine = ln->copy_routine;
        dataArgs.free_routine = ln->free_routine;
        dataArgs.size = ln->size;

        LinkedListAddLast(result, data, dataArgs);
    }

    return result;
}

void LinkedListForEach(LinkedList l, void (*func)(void *, int i))
{
    LinkedListNode ln;
    int i;

    assert(l != NULL);
    assert(func != NULL);

    for (ln = l->head, i = 0; ln != NULL; ln = ln->next, i++)
    {
        (*func)(ln->data, i);
    }
}

void LinkedListFilter(LinkedList l, bool (*comparator)(void *, void *), void *comparisonArg)
{
    int i;

    assert(l != NULL);
    assert(comparator != NULL);

    while ((i = LinkedListSearch(l, comparator, comparisonArg)) > -1)
    {
        LinkedListDeleteX(l, i);
    }
}

void FreeLinkedList(void *a)
{
    LinkedListFree((LinkedList)a);
}

void *CopyLinkedList(void *a)
{
    return LinkedListCopy((LinkedList)a);
}

DataArgs DataArgsLinkedList()
{
    DataArgs d;

    d.size = sizeof(struct LinkedList);
    d.free_routine = &FreeLinkedList;
    d.copy_routine = &CopyLinkedList;

    return d;
}

String LinkedListGetStringInt(LinkedList l)
{
    LinkedListNode ln;
    String result, temp;
    int *i;

    assert(l != NULL);

    result = StringCopy("[");

    ln = l->head;
    while (ln != NULL)
    {
        i = ln->data;
        ln = ln->next;

        if (i != NULL)
        {
            temp = IntToString(*i);
            result = StringAdd(result, temp);
            free(temp);
        }
        else
            result = StringAdd(result, "NULL");

        if (ln != NULL)
            result = StringAdd(result, ", ");
    }

    result = StringAdd(result, "]");

    return result;
}

String LinkedListGetStringString(LinkedList l)
{
    LinkedListNode ln;
    String i, result;

    assert(l != NULL);

    result = StringCopy("[");

    ln = l->head;
    while (ln != NULL)
    {
        i = ln->data;
        ln = ln->next;

        result = StringAdd(result, "\"");

        if (i != NULL)
            result = StringAdd(result, i);
        else
            result = StringAdd(result, "NULL");

        result = StringAdd(result, "\"");

        if (ln != NULL)
            result = StringAdd(result, ", ");
    }

    result = StringAdd(result, "]");

    return result;
}

void LinkedListDebugPrintString(LinkedList l)
{
    String s;

    s = LinkedListGetStringString(l);

    printf("%s\n", s);

    free(s);
}

void LinkedListDebugPrintInt(LinkedList l)
{
    String s;

    s = LinkedListGetStringInt(l);

    printf("%s\n", s);

    free(s);
}

bool LinkedListCursorNext(LinkedList l)
{
    assert(l != NULL);

    if (l->cursor.current == NULL)
    {
        if (l->cursor.previous != NULL)
            return false;

        LinkedListCursorSetTo(l, l->head);
        return true;
    }

    LinkedListCursorSetTo(l, l->cursor.current->next);
    return true;
}

bool LinkedListCursorPrevious(LinkedList l)
{
    assert(l != NULL);

    if (l->cursor.previous == NULL)
    {
        if (l->cursor.current == NULL)
            return false;

        LinkedListCursorSetTo(l, NULL);
        return true;
    }

    LinkedListCursorSetTo(l, l->cursor.previous);
    return true;
}

bool LinkedListCursorIsNull(LinkedList l)
{
    assert(l != NULL);

    if (l->cursor.current != NULL)
        return false;

    return true;
}

bool LinkedListCursorBeforeHead(LinkedList l)
{
    l->cursor.current = NULL;
    l->cursor.previous = NULL;

    return true;
}

bool LinkedListCursorAfterTail(LinkedList l)
{
    l->cursor.current = NULL;
    l->cursor.previous = l->tail;

    return true;
}

bool LinkedListCursorSearchNext(LinkedList l, bool (*comparator)(void *, void *), void *comparisonArg)
{
    assert(l != NULL);
    assert(comparator != NULL);

    LinkedListCursorNext(l);
    while (LinkedListCursorIsNull(l) == false && (*comparator)(LinkedListCursorGet(l), comparisonArg) == false)
    {
        LinkedListCursorNext(l);
    }

    if (LinkedListCursorIsNull(l) == true)
        return false;

    return true;
}

bool LinkedListCursorSearchBefore(LinkedList l, bool (*comparator)(void *, void *), void *comparisonArg)
{
    assert(l != NULL);
    assert(comparator != NULL);

    LinkedListCursorPrevious(l);
    while (LinkedListCursorIsNull(l) == false && (*comparator)(LinkedListCursorGet(l), comparisonArg) == false)
    {
        LinkedListCursorPrevious(l);
    }

    if (LinkedListCursorIsNull(l) == true)
        return false;

    return true;
}

bool LinkedListCursorHasNext(LinkedList l)
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

bool LinkedListCursorHasPrevious(LinkedList l)
{
    assert(l != NULL);

    if (l->cursor.previous == NULL)
        return false;

    return true;
}

void *LinkedListCursorGet(LinkedList l)
{
    assert(l != NULL);
    assert(LinkedListCursorIsNull(l) == false);

    return l->cursor.current->data;
}

bool LinkedListCursorChange(LinkedList l, void *data, DataArgs dataArgs)
{
    assert(l != NULL);
    assert(LinkedListCursorIsNull(l) == false);

    return LinkedListNodeChange(l->cursor.current, data, dataArgs);
}

bool LinkedListCursorAdd(LinkedList l, void *data, DataArgs dataArgs)
{
    assert(l != NULL);

    if (l->cursor.previous == NULL)
        return LinkedListAddFirst(l, data, dataArgs);

    if (l->cursor.previous == l->tail)
        return LinkedListAddLast(l, data, dataArgs);

    return LinkedListNodeAdd(l, l->cursor.current, data, dataArgs);
}

bool LinkedListCursorDelete(LinkedList l)
{
    assert(l != NULL);
    assert(LinkedListCursorIsNull(l) == false);

    if (l->cursor.current == l->head)
        return LinkedListDeleteFirst(l);

    if (l->cursor.current == l->tail)
        return LinkedListDeleteLast(l);

    return LinkedListNodeDelete(l, l->cursor.current);
}
