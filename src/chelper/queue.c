#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "queue.h"
#include "arraylist.h"
#include "common.h"
#include "internal/common.h"
#include "internal/arraylist.h"
#include "string.h"

struct Queue
{
    /*The data*/
    ArrayList data;
};

Queue QueueCreate(free_fn_t free_fn, copy_fn_t copy_fn)
{
    Queue s;

    s = malloc(sizeof(struct Queue));
    assert(s != NULL);

    s->data = ArrayListCreate(0, free_fn, copy_fn);

    return s;
}

Queue QueueCopy(Queue h)
{
    Queue s;

    assert(h != NULL);

    s = QueueCreate(ArrayListGetFreeFn(h->data), ArrayListGetCopyFn(h->data));
    ArrayListFree(s->data);

    s->data = ArrayListCopy(h->data);

    return s;
}

void QueueFree(Queue h)
{
    assert(h != NULL);

    QueueClear(h);

    ArrayListFree(h->data);

    free(h);
}

void QueueClear(Queue h)
{
    assert(h != NULL);

    ArrayListClear(h->data);
}

size_t QueueLength(Queue h)
{
    assert(h != NULL);

    return ArrayListLength(h->data);
}

void *QueueGet(Queue h)
{
    assert(h != NULL);

    return ArrayListGet(h->data);
}

bool QueueRemove(Queue h)
{
    assert(h != NULL);

    return ArrayListRemove(h->data);
}

bool QueueAdd(Queue h, void *data)
{
    assert(h != NULL);

    return ArrayListAddX(h->data, data, 0);
}

bool QueueRemoveNoFree(Queue h)
{
    assert(h != NULL);

    return ArrayListRemoveNoFree(h->data);
}
