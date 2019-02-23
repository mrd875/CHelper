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
    /*Owner*/
    LinkedList owner;
};

struct LinkedList
{
    /* The head node. */
    LinkedListNode head;
    /* The tail node */
    LinkedListNode tail;
    /* The function pointer to free data */
    free_fn_t free_fn;
    /* The function pointer to copy data */
    copy_fn_t copy_fn;
    /* The size */
    size_t length;
    /* The cap of the LinkedList */
    size_t capacity;
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
void _LinkedListCursorUpdate(LinkedList l)
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
void _LinkedListCursorClear(LinkedList l)
{
    assert(l != NULL);

    l->cursor.previous = NULL;
    l->cursor.current = NULL;
}

/*
    Sets the cursor.
*/
void _LinkedListCursorSetTo(LinkedList l, LinkedListNode ln)
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
LinkedListNode LinkedListNodeCreate(LinkedList l)
{
    LinkedListNode ln;

    debugp("Creating a LinkedList node");

    ln = malloc(sizeof(struct LinkedListNode));
    assert(ln != NULL);

    ln->data = NULL;
    ln->next = NULL;
    ln->previous = NULL;
    ln->owner = l;

    return ln;
}

/*
    Frees the node's data.
*/
void LinkedListNodeFreeData(LinkedListNode ln)
{
    assert(ln != NULL);
    assert(ln->owner != NULL);

    debugp("freeing LinkedList node data");

    if (ln->owner->free_fn != NULL)
        ln->owner->free_fn(ln->data);
}

/*
    Returns a copy of the node's data.
*/
void *LinkedListNodeCopyData(LinkedListNode ln)
{
    assert(ln != NULL);
    assert(ln->owner != NULL);

    debugp("copying a LinkedList node data");

    if (ln->owner->copy_fn == NULL)
        return NULL;

    return ln->owner->copy_fn(ln->data);
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

LinkedList LinkedListCreate(size_t capacity, free_fn_t free_fn, copy_fn_t copy_fn)
{
    LinkedList l;

    debugp("Creating a LinkedList");

    l = malloc(sizeof(struct LinkedList));
    assert(l != NULL);

    l->capacity = capacity;
    l->length = 0;
    l->tail = NULL;
    l->head = NULL;
    l->copy_fn = copy_fn;
    l->free_fn = free_fn;
    _LinkedListCursorClear(l);

    return l;
}

LinkedList LinkedListCopy(LinkedList l)
{
    LinkedList result;
    LinkedListNode ln;
    void *data;

    assert(l != NULL);

    result = LinkedListCreate(l->capacity, l->free_fn, l->copy_fn);

    for (ln = l->head; ln != NULL; ln = ln->next)
    {
        data = LinkedListNodeCopyData(ln);

        LinkedListAdd(result, data);
    }

    return result;
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

    _LinkedListCursorClear(l);
    l->length = 0;
    l->head = NULL;
    l->tail = NULL;
}

size_t LinkedListLength(LinkedList l)
{
    assert(l != NULL);

    debugp("Getting LinkedList size");

    return l->length;
}

size_t LinkedListCapacity(LinkedList l)
{
    assert(l != NULL);

    debugp("Getting LinkedList cap");

    return l->capacity;
}

bool LinkedListIsFull(LinkedList l)
{
    size_t cap;

    assert(l != NULL);

    debugp("Checking if full");

    cap = LinkedListCapacity(l);

    if (cap == 0)
        return false;

    if (LinkedListLength(l) < cap)
        return false;

    return true;
}

bool LinkedListIsEmpty(LinkedList l)
{
    assert(l != NULL);

    debugp("Checking if empty");

    if (LinkedListLength(l) > 0)
        return false;

    return true;
}

bool LinkedListSetCapacity(LinkedList l, size_t size)
{
    assert(l != NULL);

    if (size != 0 && size < LinkedListLength(l))
    {
        SetErrorMessage("Set capacity below the length of list");
        return false;
    }

    l->capacity = size;
    return true;
}

/*
    Changes the data at the node.
*/
bool LinkedListNodeSet(LinkedListNode ln, void *data)
{
    assert(ln != NULL);

    if (ln->data != NULL)
        LinkedListNodeFreeData(ln);

    ln->data = data;
    return true;
}

bool LinkedListSetX(LinkedList l, void *data, size_t x)
{
    size_t i;
    LinkedListNode ln;

    assert(l != NULL);

    if (x >= LinkedListLength(l))
    {
        SetErrorMessage("Index out of bounds");
        return false;
    }

    ln = l->head;
    for (i = 0; i < x && ln != NULL; i++)
    {
        ln = ln->next;
    }

    assert(ln != NULL);
    return LinkedListNodeSet(ln, data);
}

bool LinkedListSet(LinkedList l, void *data)
{
    LinkedListNode ln;

    assert(l != NULL);

    ln = l->tail;

    if (ln == NULL)
    {
        SetErrorMessage("Index out of bounds");
        return false;
    }

    return LinkedListNodeSet(ln, data);
}

void *LinkedListGet(LinkedList l)
{
    LinkedListNode ln;

    assert(l != NULL);

    debugp("Getting tail LinkedList data");

    ln = l->tail;

    if (ln == NULL)
    {
        SetErrorMessage("Get from empty list");
        return NULL;
    }

    return ln->data;
}

void *LinkedListGetX(LinkedList l, size_t x)
{
    LinkedListNode ln;
    size_t i;

    assert(l != NULL);

    if (x >= LinkedListLength(l))
    {
        SetErrorMessage("Index out of bounds");
        return NULL;
    }

    debugp("Getting x %d LinkedList data", x);

    ln = l->head;
    for (i = 0; i < x && ln != NULL; i++)
    {
        ln = ln->next;
    }

    assert(ln != NULL);

    return ln->data;
}

/*
    Adds the data to the LinkedList at the node. This is NOT for the edges case of insertion but only for inserting when the LinkedListnode has prev.
*/
bool LinkedListNodeAdd(LinkedListNode ln, void *data)
{
    LinkedListNode newNode, prev;
    LinkedList l;

    assert(ln != NULL);
    assert(ln->previous != NULL);
    assert(ln->owner != NULL);

    l = ln->owner;

    if (LinkedListIsFull(l) == true)
    {
        SetErrorMessage("Adding to a full list.");
        return false;
    }

    prev = ln->previous;

    newNode = LinkedListNodeCreate(l);

    if (LinkedListNodeSet(newNode, data) == false)
        return false;

    prev->next = newNode;
    newNode->previous = prev;

    newNode->next = ln;
    ln->previous = newNode;

    l->length++;
    _LinkedListCursorUpdate(l);

    return true;
}

/*Adds to the head of the list*/
bool LinkedListAddFirst(LinkedList l, void *data)
{
    LinkedListNode head, newNode;

    assert(l != NULL);

    if (LinkedListIsFull(l) == true)
    {
        SetErrorMessage("Adding to a full list.");
        return false;
    }

    newNode = LinkedListNodeCreate(l);

    if (LinkedListNodeSet(newNode, data) == false)
        return false;

    head = l->head;

    newNode->next = head;
    l->head = newNode;

    if (head == NULL)
        l->tail = newNode;
    else
        head->previous = newNode;

    l->length++;
    _LinkedListCursorUpdate(l);

    return true;
}

bool LinkedListAdd(LinkedList l, void *data)
{
    LinkedListNode tail, newNode;

    assert(l != NULL);

    if (LinkedListIsFull(l) == true)
    {
        SetErrorMessage("Adding to a full list.");
        return false;
    }

    newNode = LinkedListNodeCreate(l);

    if (LinkedListNodeSet(newNode, data) == false)
        return false;

    tail = l->tail;

    newNode->previous = tail;
    l->tail = newNode;

    if (tail == NULL)
        l->head = newNode;
    else
        tail->next = newNode;

    l->length++;
    _LinkedListCursorUpdate(l);

    return true;
}

bool LinkedListAddX(LinkedList l, void *data, size_t x)
{
    size_t i;
    LinkedListNode ln;

    assert(l != NULL);

    i = LinkedListLength(l);

    if (x > i)
    {
        SetErrorMessage("Index out of bounds");
        return false;
    }

    if (x == 0)
        return LinkedListAddFirst(l, data);

    if (x == i)
        return LinkedListAdd(l, data);

    ln = l->head;
    for (i = 0; i < x && ln != NULL; i++)
    {
        ln = ln->next;
    }

    assert(ln != NULL);
    return LinkedListNodeAdd(ln, data);
}

/*
    Deletes the node. This is not for edges cases but when the node has a next and prev.
*/
bool LinkedListNodeRemove(LinkedListNode ln)
{
    LinkedListNode prev, next;
    LinkedList l;

    assert(ln != NULL);
    assert(ln->next != NULL);
    assert(ln->previous != NULL);
    assert(ln->owner != NULL);

    l = ln->owner;

    if (LinkedListIsEmpty(l) == true)
    {
        SetErrorMessage("Delete from empty list.");
        return false;
    }

    prev = ln->previous;
    next = ln->next;

    prev->next = next;
    next->previous = prev;

    l->length--;

    if (ln == l->cursor.current)
        _LinkedListCursorSetTo(l, prev);
    else
        _LinkedListCursorUpdate(l);

    LinkedListNodeFree(ln);

    return true;
}

/*Deletes the first item from the list*/
bool LinkedListRemoveFirst(LinkedList l)
{
    LinkedListNode head, next;

    assert(l != NULL);

    if (LinkedListIsEmpty(l) == true)
    {
        SetErrorMessage("Delete from empty list.");
        return false;
    }

    head = l->head;

    assert(head != NULL);

    next = head->next;

    LinkedListNodeFree(head);

    l->head = next;

    if (next == NULL)
        l->tail = NULL;
    else
        next->previous = NULL;

    l->length--;

    if (head == l->cursor.current)
        _LinkedListCursorSetTo(l, NULL);
    else
        _LinkedListCursorUpdate(l);

    return true;
}

bool LinkedListRemove(LinkedList l)
{
    LinkedListNode tail, prev;

    assert(l != NULL);

    if (LinkedListIsEmpty(l) == true)
    {
        SetErrorMessage("Delete from empty list.");
        return false;
    }

    tail = l->tail;

    assert(tail != NULL);

    prev = tail->previous;

    LinkedListNodeFree(tail);

    l->tail = prev;

    if (prev == NULL)
        l->head = NULL;
    else
        prev->next = NULL;

    l->length--;

    if (tail == l->cursor.current)
        _LinkedListCursorSetTo(l, prev);
    else
        _LinkedListCursorUpdate(l);

    return true;
}

bool LinkedListRemoveX(LinkedList l, size_t x)
{
    size_t i;
    LinkedListNode ln;

    assert(l != NULL);

    i = LinkedListLength(l);

    if (x >= i)
    {
        SetErrorMessage("Index out of bounds.");
        return false;
    }

    if (x == 0)
        return LinkedListRemoveFirst(l);

    if (x == i - 1)
        return LinkedListRemove(l);

    ln = l->head;
    for (i = 0; i < x && ln != NULL; i++)
    {
        ln = ln->next;
    }

    assert(ln != NULL);
    return LinkedListNodeRemove(ln);
}

LinkedList LinkedListConcat(LinkedList l, LinkedList l2)
{
    LinkedList answer;
    bool o;
    assert(l != NULL);
    assert(l2 != NULL);

    answer = LinkedListCopy(l);
    answer->capacity = 0;

    o = LinkedListAddAll(answer, l2);

    assert(o == true);

    return answer;
}

LinkedList LinkedListGetSubLinkedList(LinkedList l, size_t i, size_t size)
{
    LinkedList answer;
    assert(l != NULL);

    answer = LinkedListCopy(l);
    answer->capacity = 0;

    LinkedListTrim(answer, i, size);

    return answer;
}

LinkedList LinkedListFilter(LinkedList l, compare_fn_t compare_fn, void *compare_arg)
{
    LinkedList answer;
    assert(l != NULL);
    assert(compare_fn != NULL);

    answer = LinkedListCopy(l);
    answer->capacity = 0;

    LinkedListRemoveIf(answer, compare_fn, compare_arg);

    return answer;
}

int LinkedListIndexOf(LinkedList l, compare_fn_t compare_fn, void *compare_arg)
{
    LinkedListNode ln;
    int i;

    assert(l != NULL);
    assert(compare_fn != NULL);

    for (ln = l->head, i = 0; ln != NULL; ln = ln->next, i++)
    {
        if ((*compare_fn)(ln->data, compare_arg) == false)
            continue;

        return i;
    }

    return -1;
}

size_t LinkedListCount(LinkedList l, compare_fn_t compare_fn, void *compare_arg)
{
    LinkedListNode ln;
    size_t i;

    assert(l != NULL);
    assert(compare_fn != NULL);

    i = 0;

    for (ln = l->head; ln != NULL; ln = ln->next)
    {
        if ((*compare_fn)(ln->data, compare_arg) == false)
            continue;

        i++;
    }

    return i;
}

bool LinkedListContains(LinkedList l, compare_fn_t compare_fn, void *compare_arg)
{
    assert(l != NULL);
    assert(compare_fn != NULL);

    if (LinkedListIndexOf(l, compare_fn, compare_arg) == -1)
        return false;

    return true;
}

void LinkedListForEach(LinkedList l, foreach_fn_t foreach_fn)
{
    LinkedListNode ln;
    size_t i;

    assert(l != NULL);
    assert(foreach_fn != NULL);

    for (ln = l->head, i = 0; ln != NULL; ln = ln->next, i++)
    {
        foreach_fn(ln->data, i);
    }
}

bool LinkedListRemoveIf(LinkedList l, compare_fn_t compare_fn, void *compare_arg)
{
    int i;
    bool o;

    assert(l != NULL);
    assert(compare_fn != NULL);

    while ((i = LinkedListIndexOf(l, compare_fn, compare_arg)) != -1)
    {
        o = LinkedListRemoveX(l, i);

        assert(o == true);
    }

    return true;
}

bool LinkedListAddAll(LinkedList l, LinkedList l2)
{
    LinkedListNode ln;
    bool o;
    void *data;

    assert(l != NULL);
    assert(l2 != NULL);

    for (ln = l2->head; ln != NULL; ln = ln->next)
    {
        data = LinkedListNodeCopyData(ln);

        o = LinkedListAdd(l, data);

        if (o == false)
            return false;
    }

    return true;
}

bool LinkedListRemoveRange(LinkedList l, size_t i, size_t size)
{
    size_t ii;
    bool o;
    assert(l != NULL);

    for (ii = 0; ii < size; ii++)
    {
        o = LinkedListRemoveX(l, i);

        if (o == false)
            return false;
    }

    return true;
}

bool LinkedListTrim(LinkedList l, size_t i, size_t size)
{
    size_t ii;
    bool o;
    assert(l != NULL);

    while (LinkedListRemoveX(l, i + size) == true)
        ;

    for (ii = 0; ii < i; ii++)
    {
        o = LinkedListRemoveX(l, 0);

        if (o == false)
            return false;
    }

    return true;
}

void FreeLinkedList(void *a)
{
    LinkedListFree((LinkedList)a);
}

void *CopyLinkedList(void *a)
{
    return LinkedListCopy((LinkedList)a);
}

String LinkedListToStringInt(LinkedList l)
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

String LinkedListToStringString(LinkedList l)
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

void LinkedListPrintString(LinkedList l)
{
    String s;

    assert(l != NULL);

    s = LinkedListToStringString(l);

    printf("%s\n", s);

    free(s);
}

void LinkedListPrintInt(LinkedList l)
{
    String s;

    assert(l != NULL);

    s = LinkedListToStringInt(l);

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

        _LinkedListCursorSetTo(l, l->head);
        return true;
    }

    _LinkedListCursorSetTo(l, l->cursor.current->next);
    return true;
}

bool LinkedListCursorPrevious(LinkedList l)
{
    assert(l != NULL);

    if (l->cursor.previous == NULL)
    {
        if (l->cursor.current == NULL)
            return false;

        _LinkedListCursorSetTo(l, NULL);
        return true;
    }

    _LinkedListCursorSetTo(l, l->cursor.previous);
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
    assert(l != NULL);

    l->cursor.current = NULL;
    l->cursor.previous = NULL;

    return true;
}

bool LinkedListCursorAfterTail(LinkedList l)
{
    assert(l != NULL);

    l->cursor.current = NULL;
    l->cursor.previous = l->tail;

    return true;
}

bool LinkedListCursorSearchNext(LinkedList l, compare_fn_t compare_fn, void *comparisonArg)
{
    assert(l != NULL);
    assert(compare_fn != NULL);

    LinkedListCursorNext(l);
    while (LinkedListCursorIsNull(l) == false && compare_fn(LinkedListCursorGet(l), comparisonArg) == false)
    {
        LinkedListCursorNext(l);
    }

    if (LinkedListCursorIsNull(l) == true)
        return false;

    return true;
}

bool LinkedListCursorSearchPrevious(LinkedList l, compare_fn_t compare_fn, void *comparisonArg)
{
    assert(l != NULL);
    assert(compare_fn != NULL);

    LinkedListCursorPrevious(l);
    while (LinkedListCursorIsNull(l) == false && compare_fn(LinkedListCursorGet(l), comparisonArg) == false)
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

    if (LinkedListCursorIsNull(l) == true)
    {
        return NULL;
    }

    return l->cursor.current->data;
}

bool LinkedListCursorSet(LinkedList l, void *data)
{
    assert(l != NULL);

    if (LinkedListCursorIsNull(l) == true)
    {
        return false;
    }

    return LinkedListNodeSet(l->cursor.current, data);
}

bool LinkedListCursorAdd(LinkedList l, void *data)
{
    assert(l != NULL);

    if (l->cursor.previous == NULL)
        return LinkedListAddFirst(l, data);

    if (l->cursor.previous == l->tail)
        return LinkedListAdd(l, data);

    return LinkedListNodeAdd(l->cursor.current, data);
}

bool LinkedListCursorRemove(LinkedList l)
{
    assert(l != NULL);

    if (LinkedListCursorIsNull(l) == true)
    {
        return false;
    }

    if (l->cursor.current == l->head)
        return LinkedListRemoveFirst(l);

    if (l->cursor.current == l->tail)
        return LinkedListRemove(l);

    return LinkedListNodeRemove(l->cursor.current);
}

int LinkedListCursorIndexOf(LinkedList l)
{
    int i;
    LinkedListNode ln;

    assert(l != NULL);

    for (i = 0, ln = l->head; ln != NULL; i++, ln = ln->next)
    {
        if (ln == l->cursor.current)
            return i;
    }

    return -1;
}
