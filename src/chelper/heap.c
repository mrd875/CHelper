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

/*Gets the item in the heap*/
void *HeapGet(Heap h);

/*Removes the item from the heap*/
bool HeapRemove(Heap h);

/*Updates the key*/
bool HeapChangeKey(Heap h, int key, int newK);

/*Adds the item to the heap*/
bool HeapAdd(Heap h, int key, void *data);
