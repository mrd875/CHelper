#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "arraylist.h"
#include "common.h"
#include "internal/common.h"
#include "string.h"

/*THe minimum size of arrays.*/
#define ArrayListMinSize 32

/* The array list struct. */
struct ArrayList
{
    /* The array of data. */
    void **arr;
    /* The number of occupied elements in the array. */
    size_t length;
    /* The current size of the array. */
    size_t size;
    /* The set capacity of the list. 0 for no limit */
    size_t capacity;
    /* The function to free the data. */
    free_fn_t free_fn;
    /* The function to copy the data. */
    copy_fn_t copy_fn;
};

ArrayList ArrayListCreate(size_t capacity, free_fn_t free_fn, copy_fn_t copy_fn)
{
    ArrayList l;
    void **arr;
    size_t startSize;

    startSize = ArrayListMinSize;

    l = malloc(sizeof(struct ArrayList));
    assert(l != NULL);

    l->length = 0;
    l->capacity = capacity;
    l->free_fn = free_fn;
    l->copy_fn = copy_fn;

    arr = malloc((sizeof(void *)) * startSize);
    assert(arr != NULL);

    l->arr = arr;
    l->size = startSize;

    return l;
}

/*Copies the item in the list.*/
void *_ArrayListCopyItem(ArrayList l, void *i)
{
    assert(l != NULL);

    if (l->copy_fn == NULL)
        return NULL;

    if (i == NULL)
        return NULL;

    return l->copy_fn(i);
}

ArrayList ArrayListCopy(ArrayList l)
{
    ArrayList l2;
    size_t i, len;

    assert(l != NULL);

    len = ArrayListLength(l);

    l2 = ArrayListCreate(l->capacity, l->free_fn, l->copy_fn);

    for (i = 0; i < len; i++)
        ArrayListAdd(l2, _ArrayListCopyItem(l, l->arr[i]));

    return l2;
}

/*Frees the item in the list.*/
void _ArrayListFreeItem(ArrayList l, void *i)
{
    assert(l != NULL);

    if (l->free_fn == NULL)
        return;

    if (i == NULL)
        return;

    l->free_fn(i);
}

void ArrayListClear(ArrayList l)
{
    size_t i, len;

    assert(l != NULL);

    len = ArrayListLength(l);

    for (i = 0; i < len; i++)
        _ArrayListFreeItem(l, l->arr[i]);

    l->length = 0;
}

void ArrayListFree(ArrayList l)
{
    assert(l != NULL);

    ArrayListClear(l);
    free(l->arr);
    free(l);
}

/*Resizes the array in the list to be the new size.*/
void _ArrayListResize(ArrayList l, size_t newSize)
{
    void **arr;

    assert(l != NULL);
    assert(newSize >= ArrayListMinSize);

    arr = realloc(l->arr, newSize * sizeof(void *));
    assert(arr != NULL);

    l->arr = arr;
    l->size = newSize;
}

/* Expands the array in the list by double. */
void _ArrayListExpandArray(ArrayList l)
{
    assert(l != NULL);

    _ArrayListResize(l, l->size * 2);
}

/* Shrinks the array of the list by half. */
void _ArrayListShrinkArray(ArrayList l)
{
    assert(l != NULL);

    _ArrayListResize(l, l->size / 2);
}

size_t ArrayListLength(ArrayList l)
{
    assert(l != NULL);

    return l->length;
}

size_t ArrayListCapacity(ArrayList l)
{
    assert(l != NULL);

    return l->capacity;
}

bool ArrayListIsFull(ArrayList l)
{
    size_t cap;

    assert(l != NULL);

    cap = ArrayListCapacity(l);

    if (cap == 0)
        return false;

    if (ArrayListLength(l) < cap)
        return false;

    return true;
}

bool ArrayListIsEmpty(ArrayList l)
{
    assert(l != NULL);

    if (ArrayListLength(l) != 0)
        return false;

    return true;
}

bool ArrayListSetX(ArrayList l, void *a, size_t x)
{
    assert(l != NULL);

    assert(x < ArrayListLength(l));

    _ArrayListFreeItem(l, l->arr[x]);

    l->arr[x] = a;
    return true;
}

void *ArrayListGetX(ArrayList l, size_t x)
{
    assert(l != NULL);

    assert(x < ArrayListLength(l));

    return l->arr[x];
}

bool ArrayListAddX(ArrayList l, void *a, size_t x)
{
    size_t len, i;

    assert(l != NULL);

    len = ArrayListLength(l);

    assert(x <= len);

    if (ArrayListIsFull(l) == true)
        return false;

    if (len >= l->size)
        _ArrayListExpandArray(l);

    for (i = len; i > x; i--)
        l->arr[i] = l->arr[i - 1];

    l->arr[x] = a;

    l->length++;
    return true;
}

bool ArrayListRemoveX(ArrayList l, size_t x)
{
    size_t len, i;

    assert(l != NULL);

    len = ArrayListLength(l);

    assert(x < len);

    if (ArrayListIsEmpty(l) == true)
        return false;

    _ArrayListFreeItem(l, l->arr[x]);

    for (i = x; i < len - 1; i++)
        l->arr[i] = l->arr[i + 1];

    l->length--;

    if (l->size > ArrayListMinSize && l->size > ArrayListLength(l) * 2)
        _ArrayListShrinkArray(l);

    return true;
}

bool ArrayListAdd(ArrayList l, void *a)
{
    assert(l != NULL);

    return ArrayListAddX(l, a, ArrayListLength(l));
}

String ArrayListToStringInt(ArrayList l)
{
    String result, temp;
    size_t i, len;
    int *a;

    assert(l != NULL);

    len = ArrayListLength(l);

    result = StringCopy("[");

    for (i = 0; i < len; i++)
    {
        a = ArrayListGetX(l, i);

        if (a == NULL)
            result = StringAdd(result, "NULL");
        else
        {
            temp = IntToString(*a);
            result = StringAdd(result, temp);
            free(temp);
        }

        if (i < len - 1)
            result = StringAdd(result, ", ");
    }

    result = StringAdd(result, "]");

    return result;
}

void ArrayListPrintInt(ArrayList l)
{
    String temp;

    temp = ArrayListToStringInt(l);
    printf("%s\n", temp);

    free(temp);
}
