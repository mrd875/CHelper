#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "common.h"
#include "../common.h"
#include "../string.h"

String CurrentErrorMsg = NULL;

void SetErrorMessage(String msg)
{
    assert(msg != NULL);

    if (CurrentErrorMsg != NULL)
        free(CurrentErrorMsg);

    CurrentErrorMsg = StringCopy(msg);
}

String GetErrorMessage()
{
    return CurrentErrorMsg;
}
