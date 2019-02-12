#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "common.h"
#include "internal/common.h"
#include "string.h"

void PrintError(String calledFrom)
{
    assert(calledFrom != NULL);

    debugp("Printing error message");
    fprintf(stderr, "%s: %s\n", calledFrom, GetErrorMessage());
}

int *QuickCreateInt(int i)
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
    int i;

    i = *((int *)a);

    return (void *)QuickCreateInt(i);
}
