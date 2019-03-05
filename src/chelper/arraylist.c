#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "arraylist.h"
#include "common.h"
#include "internal/common.h"
#include "internal/arraylist.h"
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
    /*For cursors, current index that the cursor is on*/
    int cursor;
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
    l->cursor = -1;

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
    bool o;

    assert(l != NULL);

    len = ArrayListLength(l);

    l2 = ArrayListCreate(l->capacity, l->free_fn, l->copy_fn);

    for (i = 0; i < len; i++)
    {
        o = ArrayListAdd(l2, _ArrayListCopyItem(l, l->arr[i]));
        assert(o == true);
    }

    l2->cursor = l->cursor;

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

void ArrayListClearFree(ArrayList l)
{
    assert(l != NULL);

    l->free_fn = NULL;
}

void **ArrayListGetArray(ArrayList l)
{
    assert(l != NULL);

    return l->arr;
}

void ArrayListClear(ArrayList l)
{
    size_t i, len;

    assert(l != NULL);

    len = ArrayListLength(l);

    for (i = 0; i < len; i++)
        _ArrayListFreeItem(l, l->arr[i]);

    l->length = 0;
    l->cursor = -1;
}

void ArrayListFree(ArrayList l)
{
    assert(l != NULL);

    ArrayListClear(l);
    free(l->arr);
    free(l);
}

void FreeArrayList(void *a)
{
    ArrayListFree((ArrayList)a);
}

void *CopyArrayList(void *a)
{
    return ArrayListCopy((ArrayList)a);
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

bool ArrayListSetCapacity(ArrayList l, size_t size)
{
    assert(l != NULL);

    if (size != 0 && size < ArrayListLength(l))
    {
        SetErrorMessage("Set capacity below the length of list");
        return false;
    }

    l->capacity = size;
    return true;
}

bool ArrayListSetX(ArrayList l, void *a, size_t x)
{
    assert(l != NULL);

    if (x >= ArrayListLength(l))
    {
        SetErrorMessage("Index out of bounds");
        return false;
    }

    _ArrayListFreeItem(l, l->arr[x]);

    l->arr[x] = a;
    return true;
}

void *ArrayListGetX(ArrayList l, size_t x)
{
    assert(l != NULL);

    if (x >= ArrayListLength(l))
    {
        SetErrorMessage("Index out of bounds");
        return NULL;
    }

    return l->arr[x];
}

bool ArrayListAddX(ArrayList l, void *a, size_t x)
{
    size_t len, i;

    assert(l != NULL);

    len = ArrayListLength(l);

    if (x > len)
    {
        SetErrorMessage("Index out of bounds");
        return false;
    }

    if (ArrayListIsFull(l) == true)
    {
        SetErrorMessage("Adding to full list");
        return false;
    }

    if (len >= l->size)
        _ArrayListExpandArray(l);

    for (i = len; i > x; i--)
        l->arr[i] = l->arr[i - 1];

    l->arr[x] = a;

    if ((int)x >= l->cursor)
        l->cursor++;

    l->length++;
    return true;
}

bool ArrayListRemoveX(ArrayList l, size_t x)
{
    size_t len, i;

    assert(l != NULL);

    if (ArrayListIsEmpty(l) == true)
    {
        SetErrorMessage("Removing from empty list");
        return false;
    }

    len = ArrayListLength(l);

    if (x >= len)
    {
        SetErrorMessage("Index out of bounds");
        return false;
    }

    _ArrayListFreeItem(l, l->arr[x]);

    for (i = x; i < len - 1; i++)
        l->arr[i] = l->arr[i + 1];

    if ((int)x >= l->cursor)
        l->cursor--;

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

bool ArrayListRemove(ArrayList l)
{
    assert(l != NULL);

    return ArrayListRemoveX(l, ArrayListLength(l) - 1);
}

void *ArrayListGet(ArrayList l)
{
    assert(l != NULL);

    return ArrayListGetX(l, ArrayListLength(l) - 1);
}

bool ArrayListSet(ArrayList l, void *data)
{
    assert(l != NULL);

    return ArrayListSetX(l, data, ArrayListLength(l) - 1);
}

ArrayList ArrayListConcat(ArrayList l, ArrayList l2)
{
    ArrayList answer;
    bool o;
    assert(l != NULL);
    assert(l2 != NULL);

    answer = ArrayListCopy(l);
    answer->capacity = 0;

    o = ArrayListAddAll(answer, l2);

    assert(o == true);

    return answer;
}

ArrayList ArrayListGetSubArrayList(ArrayList l, size_t i, size_t size)
{
    ArrayList answer;
    assert(l != NULL);

    answer = ArrayListCopy(l);
    answer->capacity = 0;

    ArrayListTrim(answer, i, size);

    return answer;
}

ArrayList ArrayListFilter(ArrayList l, compare_fn_t compare_fn, void *compare_arg)
{
    ArrayList answer;
    assert(l != NULL);
    assert(compare_fn != NULL);

    answer = ArrayListCopy(l);
    answer->capacity = 0;

    ArrayListRemoveIf(answer, compare_fn, compare_arg);

    return answer;
}

int ArrayListIndexOf(ArrayList l, compare_fn_t compare_fn, void *compare_arg)
{
    size_t i, len;

    assert(l != NULL);
    assert(compare_fn != NULL);

    len = ArrayListLength(l);

    for (i = 0; i < len; i++)
    {
        if (compare_fn(l->arr[i], compare_arg) == false)
            continue;

        return i;
    }

    return -1;
}

size_t ArrayListCount(ArrayList l, compare_fn_t compare_fn, void *compare_arg)
{
    size_t count, i, len;
    assert(l != NULL);
    assert(compare_fn != NULL);

    count = 0;
    len = ArrayListLength(l);

    for (i = 0; i < len; i++)
    {
        if (compare_fn(l->arr[i], compare_arg) == false)
            continue;

        count++;
    }

    return count;
}

bool ArrayListContains(ArrayList l, compare_fn_t compare_fn, void *compare_arg)
{
    assert(l != NULL);
    assert(compare_fn != NULL);

    if (ArrayListIndexOf(l, compare_fn, compare_arg) == -1)
        return false;

    return true;
}

void ArrayListForEach(ArrayList l, foreach_fn_t foreach_fn)
{
    size_t i, len;

    assert(l != NULL);
    assert(foreach_fn != NULL);

    len = ArrayListLength(l);

    for (i = 0; i < len; i++)
    {
        foreach_fn(l->arr[i], i);
    }
}

bool ArrayListRemoveIf(ArrayList l, compare_fn_t compare_fn, void *compare_arg)
{
    int i;
    bool o;

    assert(l != NULL);
    assert(compare_fn != NULL);

    while ((i = ArrayListIndexOf(l, compare_fn, compare_arg)) != -1)
    {
        o = ArrayListRemoveX(l, i);

        assert(o == true);
    }

    return true;
}

bool ArrayListAddAll(ArrayList l, ArrayList l2)
{
    size_t i, len;
    bool o;
    assert(l != NULL);
    assert(l2 != NULL);

    len = ArrayListLength(l2);

    for (i = 0; i < len; i++)
    {
        o = ArrayListAdd(l, _ArrayListCopyItem(l2, l2->arr[i]));

        if (o == false)
            return false;
    }

    return true;
}

bool ArrayListRemoveRange(ArrayList l, size_t i, size_t size)
{
    size_t ii;
    bool o;
    assert(l != NULL);

    for (ii = 0; ii < size; ii++)
    {
        o = ArrayListRemoveX(l, i);

        if (o == false)
            return false;
    }

    return true;
}

bool ArrayListTrim(ArrayList l, size_t i, size_t size)
{
    size_t ii;
    bool o;
    assert(l != NULL);

    while (ArrayListRemoveX(l, i + size) == true)
        ;

    for (ii = 0; ii < i; ii++)
    {
        o = ArrayListRemoveX(l, 0);

        if (o == false)
            return false;
    }

    return true;
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

String ArrayListToStringString(ArrayList l)
{
    String result, a, temp;
    size_t i, len;

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
            temp = StringFormat("\"%s\"", a);
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

void ArrayListPrintString(ArrayList l)
{
    String temp;

    temp = ArrayListToStringString(l);
    printf("%s\n", temp);

    free(temp);
}

bool ArrayListCursorBeforeHead(ArrayList l)
{
    assert(l != NULL);

    l->cursor = -1;

    return true;
}

bool ArrayListCursorAfterTail(ArrayList l)
{
    assert(l != NULL);

    l->cursor = ArrayListLength(l);

    return true;
}

bool ArrayListCursorNext(ArrayList l)
{
    assert(l != NULL);

    if (l->cursor >= (int)ArrayListLength(l))
    {
        SetErrorMessage("Cursor is at the end.");
        return false;
    }

    l->cursor++;
    return true;
}

bool ArrayListCursorPrevious(ArrayList l)
{
    assert(l != NULL);

    if (l->cursor < 0)
    {
        SetErrorMessage("Cursor is at the beginning.");
        return false;
    }

    l->cursor--;
    return true;
}

bool ArrayListCursorIsNull(ArrayList l)
{
    assert(l != NULL);

    if (l->cursor >= 0 && l->cursor < (int)ArrayListLength(l))
        return false;

    return true;
}

bool ArrayListCursorHasNext(ArrayList l)
{
    assert(l != NULL);

    if ((size_t)(l->cursor + 1) >= ArrayListLength(l))
        return false;

    return true;
}

bool ArrayListCursorHasPrevious(ArrayList l)
{
    assert(l != NULL);

    if (l->cursor - 1 < 0)
        return false;

    return true;
}

bool ArrayListCursorSearchNext(ArrayList l, compare_fn_t compare_fn, void *compare_arg)
{
    assert(l != NULL);
    assert(compare_fn != NULL);

    while (ArrayListCursorHasNext(l) == true)
    {
        ArrayListCursorNext(l);

        if (compare_fn(ArrayListCursorGet(l), compare_arg) == true)
            return true;
    }

    return false;
}

bool ArrayListCursorSearchPrevious(ArrayList l, compare_fn_t compare_fn, void *compare_arg)
{
    assert(l != NULL);
    assert(compare_fn != NULL);

    while (ArrayListCursorHasPrevious(l) == true)
    {
        ArrayListCursorPrevious(l);

        if (compare_fn(ArrayListCursorGet(l), compare_arg) == true)
            return true;
    }

    return false;
}

void *ArrayListCursorGet(ArrayList l)
{
    assert(l != NULL);
    assert(ArrayListCursorIsNull(l) != true);

    return l->arr[l->cursor];
}

bool ArrayListCursorSet(ArrayList l, void *data)
{
    assert(l != NULL);
    assert(ArrayListCursorIsNull(l) != true);

    return ArrayListSetX(l, data, l->cursor);
}

bool ArrayListCursorAdd(ArrayList l, void *data)
{
    int index;

    assert(l != NULL);

    index = l->cursor;
    if (index < 0)
        index = 0;

    return ArrayListAddX(l, data, index);
}

bool ArrayListCursorRemove(ArrayList l)
{
    assert(l != NULL);
    assert(ArrayListCursorIsNull(l) != true);

    return ArrayListRemoveX(l, l->cursor);
}

int ArrayListCursorIndexOf(ArrayList l)
{
    assert(l != NULL);

    return l->cursor;
}
