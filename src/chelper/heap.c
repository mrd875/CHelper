#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "heap.h"
#include "arraylist.h"
#include "common.h"
#include "internal/common.h"
#include "internal/arraylist.h"
#include "string.h"

/*Heap node*/
typedef struct HeapNode *HeapNode;

struct HeapNode
{
    /*The key for this node*/
    int key;
    /*The data at this node*/
    void *data;
    /*The owner of this node*/
    Heap owner;
};

struct Heap
{
    /*The data*/
    ArrayList data;
    /*How to free elements in the heap*/
    free_fn_t free_fn;
    /*How to copy elements in the heap*/
    copy_fn_t copy_fn;
    /*Is a reverse heap*/
    bool reverse;
};

/*Creates a heap node*/
HeapNode HeapNodeCreate(int key, void *data, Heap owner)
{
    HeapNode result;

    result = malloc(sizeof(struct HeapNode));
    assert(result != NULL);

    result->data = data;
    result->key = key;
    result->owner = owner;

    return result;
}

/*Free the heap node and its data*/
void HeapNodeFree(void *a)
{
    HeapNode h;
    assert(a != NULL);

    h = (HeapNode)a;

    assert(h->owner != NULL);

    if (h->owner->free_fn != NULL)
        h->owner->free_fn(h->data);

    free(h);
}

/*Copies a heap node and its data*/
void *HeapNodeCopy(void *a)
{
    HeapNode h, answer;
    void *data;
    assert(a != NULL);

    h = (HeapNode)a;

    assert(h->owner != NULL);

    data = NULL;

    if (h->owner->copy_fn != NULL)
        data = h->owner->copy_fn(h->data);

    answer = HeapNodeCreate(h->key, data, h->owner);

    return answer;
}

Heap HeapCreate(bool reverse, free_fn_t free_fn, copy_fn_t copy_fn)
{
    Heap h;

    h = malloc(sizeof(struct Heap));
    assert(h != NULL);

    h->copy_fn = copy_fn;
    h->free_fn = free_fn;
    h->reverse = reverse;

    h->data = ArrayListCreate(-1, &HeapNodeFree, &HeapNodeCopy);

    return h;
}

Heap HeapCopy(Heap h)
{
    Heap copy;

    assert(h != NULL);

    copy = HeapCreate(h->reverse, h->free_fn, h->copy_fn);
    ArrayListFree(copy->data);
    copy->data = ArrayListCopy(h->data);

    return copy;
}

void HeapFree(Heap h)
{
    assert(h != NULL);

    HeapClear(h);

    ArrayListFree(h->data);

    free(h);
}

void HeapClear(Heap h)
{
    assert(h != NULL);

    ArrayListClear(h->data);
}

size_t HeapLength(Heap h)
{
    assert(h != NULL);

    return ArrayListLength(h->data);
}

void *HeapGet(Heap h)
{
    HeapNode hn;
    assert(h != NULL);

    if (HeapLength(h) <= 0)
        return NULL;

    hn = ArrayListGetX(h->data, 0);

    return hn->data;
}

/*Gets next child, helper*/
int _HeapNextChild(HeapNode *arr, size_t node, size_t hsize, bool rev)
{
    size_t left, right;

    left = node * 2;
    right = left + 1;

    if (left > hsize)
        return -1;

    if (right > hsize)
        return (int)left;

    if ((arr[left - 1]->key > arr[right - 1]->key) != rev)
        return (int)left;

    return (int)right;
}

bool HeapRemove(Heap h)
{
    size_t remove, last, next;
    HeapNode *arr, temp;
    int key;

    assert(h != NULL);

    remove = HeapLength(h);
    if (remove <= 0)
        return false;

    arr = ArrayListGetArray(h->data);
    temp = arr[remove - 1];

    arr[remove - 1] = arr[0];
    arr[0] = temp;

    ArrayListRemove(h->data);

    remove--;

    if (remove <= 0)
        return true;

    arr = ArrayListGetArray(h->data);
    last = 1;
    next = _HeapNextChild(arr, 1, remove, h->reverse);
    key = temp->key;

    while (next != -1)
    {
        if ((key > arr[next - 1]->key) != h->reverse)
            break;

        arr[last - 1] = arr[next - 1];
        arr[next - 1] = temp;

        last = next;
        next = _HeapNextChild(arr, next, remove, h->reverse);
    }

    return true;
}

/*Updates the key*/
bool HeapChangeKey(Heap h, int key, int newK);

bool HeapAdd(Heap h, int key, void *data)
{
    size_t insert, current, last;
    HeapNode *arr, newNode;

    assert(h != NULL);

    insert = ArrayListLength(h->data);

    newNode = HeapNodeCreate(key, data, h);
    ArrayListAdd(h->data, newNode);

    arr = (HeapNode *)ArrayListGetArray(h->data);

    current = insert + 1;
    while (current > 1)
    {
        last = current;
        current = current / 2;

        if ((key > arr[current - 1]->key) == h->reverse)
            break;

        arr[last - 1] = arr[current - 1];
        arr[current - 1] = newNode;
    }

    return true;
}
