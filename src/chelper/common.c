#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "common.h"
#include "internal/common.h"

void PrintError(String calledFrom)
{
    assert(calledFrom != NULL);

    debugp("Printing error message");
    fprintf(stderr, "%s: %s\n", calledFrom, GetErrorMessage());
}

int *IntCopy(int i)
{
    int *result;

    debugp("Creating an int");

    result = malloc(sizeof(int));
    assert(result != NULL);

    *result = i;

    return result;
}

void *CopyInt(void *a)
{
    if (a == NULL)
        return NULL;

    return IntCopy(*(int *)a);
}

bool EqualInt(void *a, void *b)
{
    if (a == b)
        return true;

    if (a == NULL)
        return false;

    if (b == NULL)
        return false;

    if (*(int *)a != *(int *)b)
        return false;

    return true;
}

bool LessInt(void *a, void *b)
{
    if (a == NULL)
        return false;

    if (b == NULL)
        return false;

    if (*(int *)a >= *(int *)b)
        return false;

    return true;
}

bool GreatInt(void *a, void *b)
{
    if (a == NULL)
        return false;

    if (b == NULL)
        return false;

    if (*(int *)a <= *(int *)b)
        return false;

    return true;
}
