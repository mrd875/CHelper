#include <stdio.h>
#include <assert.h>
#include <stddef.h>

#include "common.h"
#include "internal/common.h"

void PrintError(String calledFrom)
{
    assert(calledFrom != NULL);

    debugp("Printing error message");
    fprintf(stderr, "%s: %s\n", calledFrom, GetErrorMessage());
}
