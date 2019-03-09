#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "stack.h"
#include "arraylist.h"
#include "common.h"
#include "internal/common.h"
#include "internal/arraylist.h"
#include "string.h"

struct Stack
{
    /*The data*/
    ArrayList data;
};

Stack StackCreate(free_fn_t free_fn, copy_fn_t copy_fn)
{
    Stack s;

    s = malloc(sizeof(struct Stack));
    assert(s != NULL);

    s->data = ArrayListCreate(0, free_fn, copy_fn);

    return s;
}

Stack StackCopy(Stack h)
{
    Stack s;

    assert(h != NULL);

    s = StackCreate(ArrayListGetFreeFn(h->data), ArrayListGetCopyFn(h->data));
    ArrayListFree(s->data);

    s->data = ArrayListCopy(h->data);

    return s;
}

void StackFree(Stack h)
{
    assert(h != NULL);

    StackClear(h);

    ArrayListFree(h->data);

    free(h);
}

void StackClear(Stack h)
{
    assert(h != NULL);

    ArrayListClear(h->data);
}

size_t StackLength(Stack h)
{
    assert(h != NULL);

    return ArrayListLength(h->data);
}

void *StackGet(Stack h)
{
    assert(h != NULL);

    return ArrayListGet(h->data);
}

bool StackRemove(Stack h)
{
    assert(h != NULL);

    return ArrayListRemove(h->data);
}

bool StackAdd(Stack h, void *data)
{
    assert(h != NULL);

    return ArrayListAdd(h->data, data);
}

bool StackRemoveNoFree(Stack h)
{
    assert(h != NULL);

    return ArrayListRemoveNoFree(h->data);
}
