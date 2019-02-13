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

int *IntCopy(int i)
{
    int *result;

    debugp("Creating an int");

    result = malloc(sizeof(int));
    assert(result != NULL);

    *result = i;

    return result;
}

/*
    Copys the int, for list args.
*/
void *CopyInt(void *a)
{
    int i;

    i = *((int *)a);

    return (void *)IntCopy(i);
}

/*
    Copys the string, for list args.
*/
void *CopyString(void *a)
{
    return (void *)StringCopy((String)a);
}

DataArgs DataArgsNULL()
{
    DataArgs a;

    a.size = 0;
    a.free_routine = NULL;
    a.copy_routine = NULL;

    return a;
}

DataArgs DataArgsInt()
{
    DataArgs a;

    a.size = sizeof(int);
    a.free_routine = &free;
    a.copy_routine = &CopyInt;

    return a;
}

DataArgs DataArgsString()
{
    DataArgs a;

    a.size = sizeof(char); /*Make sure caller updates this...*/
    a.free_routine = &free;
    a.copy_routine = &CopyString;

    return a;
}
